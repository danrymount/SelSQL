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
    explicit VariablesListNode(std::vector<VariableNode*>& _vars)
                                                                                                        : BaseNode(NodeType::VARIABLES_LIST),
                                                                                                          varsList(_vars) {
    }

    VariablesListNode() : BaseNode(NodeType::VARIABLES_LIST) {}

    ~VariablesListNode() override {
        for (auto& v : varsList) {
            delete v;
        }
    }

    void push(VariableNode* var) { varsList.emplace_back(var); }

    std::vector<VariableNode*> getVarList() { return varsList; }

    NodeType getNodeType() override { return BaseNode::getNodeType(); }

   private:
    std::vector<VariableNode*> varsList;
};

#endif  // SELSQL_VARIABLESLISTNODE_H
