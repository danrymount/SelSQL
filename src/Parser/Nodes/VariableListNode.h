//
// Created by sapiest on 23.10.2019.
//

#ifndef SELSQL_VARIABLELISTNODE_H
#define SELSQL_VARIABLELISTNODE_H
#include <utility>

#include "BaseNode.h"
class VariableListNode : public BaseNode {
   public:
    explicit VariableListNode(std::vector<VariableNode*> _variables) : variables(std::move(_variables)) {}

    ~VariableListNode() override {
        for (auto& var : variables) {
            delete var;
        }
    }

    std::vector<VariableNode*> getVariables() { return variables; }

    void accept(TreeVisitor* v) override { v->visit(this); }

   private:
    std::vector<VariableNode*> variables;
};

#endif  // SELSQL_VARIABLELISTNODE_H
