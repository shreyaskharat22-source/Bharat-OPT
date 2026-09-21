from fastapi import FastAPI, HTTPException
from fastapi.middleware.cors import CORSMiddleware
from fastapi.staticfiles import StaticFiles
from fastapi.responses import RedirectResponse
from pydantic import BaseModel
from typing import List, Optional
import sys
import os

# Add the build directory to the python path to find the pybind11 module
# Since services/api is 2 levels deep, build is at ../../build/engine
current_dir = os.path.dirname(os.path.abspath(__file__))
engine_build_dir = os.path.join(current_dir, '..', '..', 'build', 'engine')
sys.path.append(engine_build_dir)

try:
    import bharatopt_py as opt
except ImportError as e:
    print(f"Failed to import bharatopt_py. Is it built in {engine_build_dir}?")
    print(e)
    sys.exit(1)

app = FastAPI(title="BharatOpt-X API")

# Allow CORS for React frontend
app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"], # For dev only
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)

static_dir = os.path.join(current_dir, "static")
if not os.path.exists(static_dir):
    os.makedirs(static_dir)

app.mount("/static", StaticFiles(directory=static_dir), name="static")

# Pydantic models for incoming JSON
class BoundsRequest(BaseModel):
    lower: float = 0.0
    upper: float = 1e30

class VariableRequest(BaseModel):
    name: str
    type: str # "Continuous", "Integer", "Binary"
    bounds: BoundsRequest

class ConstraintRequest(BaseModel):
    name: str
    type: str # "LessEq", "GreaterEq", "Equal"
    rhs: float
    coefficients: List[float] # ordered by variable index

class ObjectiveRequest(BaseModel):
    sense: str # "Minimize", "Maximize"
    offset: float = 0.0
    coefficients: List[float] # ordered by variable index

class LPModelRequest(BaseModel):
    variables: List[VariableRequest]
    constraints: List[ConstraintRequest]
    objective: ObjectiveRequest
    solver_type: str = "AUTO"

@app.get("/")
def read_root():
    return RedirectResponse(url="/static/index.html")

@app.post("/solve")
def solve_model(req: LPModelRequest):
    try:
        model = opt.Model("WebModel")
        
        # Add Variables
        var_map = []
        for v in req.variables:
            v_type = opt.VariableType.Continuous
            if v.type == "Integer": v_type = opt.VariableType.Integer
            elif v.type == "Binary": v_type = opt.VariableType.Binary
            
            bounds = opt.Bounds(v.bounds.lower, v.bounds.upper)
            vid = model.add_variable(v.name, v_type, bounds)
            var_map.append(vid)

        # Add Objective
        sense = opt.ObjectiveSense.Minimize if req.objective.sense == "Minimize" else opt.ObjectiveSense.Maximize
        model.objective().set_sense(sense)
        model.objective().set_offset(req.objective.offset)
        for i, coeff in enumerate(req.objective.coefficients):
            model.objective().set_coefficient(i, coeff)
            
        # Add Constraints
        for c in req.constraints:
            ctype = opt.ConstraintType.LessEq
            if c.type == "GreaterEq": ctype = opt.ConstraintType.GreaterEq
            elif c.type == "Equal": ctype = opt.ConstraintType.Equal
            
            cid = model.add_constraint(c.name, ctype, c.rhs)
            for i, coeff in enumerate(c.coefficients):
                if coeff != 0.0:
                    model.set_constraint_coefficient(cid, i, coeff)

        # Solve using Simplex
        solver = opt.SimplexSolver()
        res = solver.solve(model)
        
        # Serialize Iteration History
        iterations = []
        for it in res.iteration_history:
            iterations.append({
                "iteration_number": it.iteration_number,
                "is_phase_1": it.is_phase_1,
                "entering_variable": it.entering_variable,
                "leaving_variable": it.leaving_variable,
                "pivot_element": it.pivot_element,
                "objective_value": it.objective_value,
                "basic_variables": it.basic_variables,
                "reduced_costs": it.reduced_costs,
                "current_solution": it.current_solution
            })
            
        return {
            "status": str(res.status).split('.')[-1],
            "objective_value": res.objective_value,
            "primal_variables": res.primal_variables,
            "iterations": iterations
        }
    except Exception as e:
        raise HTTPException(status_code=500, detail=str(e))
