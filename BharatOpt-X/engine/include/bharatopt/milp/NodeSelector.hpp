#pragma once

#include "MILPNode.hpp"
#include <vector>
#include <memory>

namespace bharatopt {

class NodeSelector {
public:
    virtual ~NodeSelector() = default;
    
    virtual void push(const MILPNode& node) = 0;
    virtual MILPNode pop() = 0;
    virtual bool empty() const = 0;
    virtual size_t size() const = 0;
};

class DepthFirstSelector : public NodeSelector {
public:
    void push(const MILPNode& node) override {
        nodes_.push_back(node);
    }
    
    MILPNode pop() override {
        MILPNode node = nodes_.back();
        nodes_.pop_back();
        return node;
    }
    
    bool empty() const override { return nodes_.empty(); }
    size_t size() const override { return nodes_.size(); }
    
private:
    std::vector<MILPNode> nodes_;
};

class BreadthFirstSelector : public NodeSelector {
public:
    void push(const MILPNode& node) override {
        nodes_.push_back(node); // Inefficient queue for simplicity
    }
    
    MILPNode pop() override {
        MILPNode node = nodes_.front();
        nodes_.erase(nodes_.begin());
        return node;
    }
    
    bool empty() const override { return nodes_.empty(); }
    size_t size() const override { return nodes_.size(); }
    
private:
    std::vector<MILPNode> nodes_;
};

class BestBoundSelector : public NodeSelector {
public:
    // Assumes minimization (lowest bound first)
    // If maximization, we would negate the bound or change operator<
    // For simplicity, let's assume we maintain a sorted list (or priority queue)
    
    BestBoundSelector(bool is_maximize) : is_maximize_(is_maximize) {}
    
    void push(const MILPNode& node) override {
        nodes_.push_back(node);
    }
    
    MILPNode pop() override {
        int best_idx = 0;
        for (size_t i = 1; i < nodes_.size(); ++i) {
            if (is_maximize_) {
                if (nodes_[i].lp_bound > nodes_[best_idx].lp_bound) best_idx = i;
            } else {
                if (nodes_[i].lp_bound < nodes_[best_idx].lp_bound) best_idx = i;
            }
        }
        MILPNode node = nodes_[best_idx];
        nodes_.erase(nodes_.begin() + best_idx);
        return node;
    }
    
    bool empty() const override { return nodes_.empty(); }
    size_t size() const override { return nodes_.size(); }
    
private:
    std::vector<MILPNode> nodes_;
    bool is_maximize_;
};

} // namespace bharatopt
