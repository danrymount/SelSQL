//
// Created by sapiest on 23.10.2019.
//

#ifndef SELSQL_VARIABLELISTNODE_H
#define SELSQL_VARIABLELISTNODE_H
#include <utility>

#include "BaseNode.h"
class VariableListNode : public BaseNode {
   public:
    explicit VariableListNode(std::vector<VariableNode*> _variables) : variablesNodes(std::move(_variables)) {}

    std::vector<VariableNode*> getVariablesNodes() { return variablesNodes; }

    void addVariable(std::pair<std::string, Variable> var) { variables.emplace_back(std::move(var)); }

    std::vector<std::pair<std::string, Variable>> getVariables() { return variables; }

    void accept(TreeVisitor* v) override { v->visit(this); }

   private:
    std::vector<VariableNode*> variablesNodes;
    std::vector<std::pair<std::string, Variable>> variables;
};

#endif  // SELSQL_VARIABLELISTNODE_H
