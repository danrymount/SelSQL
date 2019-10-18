//
// Created by sapiest on 18.10.2019.
//

#ifndef SELSQL_VARIABLESLISTNODE_H
#define SELSQL_VARIABLESLISTNODE_H
#include <vector>
#include "BaseNode.h"
#include "VariableNode.h"
class VariablesListNode : public BaseNode {
   public:
    explicit VariablesListNode(std::vector<Variable*>& _vars) : BaseNode(NodeType::VARIABLES_LIST), varsList(_vars) {}

    VariablesListNode() : BaseNode(NodeType::VARIABLES_LIST) {}

    ~VariablesListNode() override {
        for (auto& v : varsList) {
            delete v;
        }
    }

    void push(Variable* var) { varsList.emplace_back(var); }

    std::vector<Variable*> getVarList() { return varsList; }

    NodeType getNodeType() override { return BaseNode::getNodeType(); }

   private:
    std::vector<Variable*> varsList;
};

#endif  // SELSQL_VARIABLESLISTNODE_H
