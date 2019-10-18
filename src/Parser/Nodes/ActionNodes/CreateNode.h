//
// Created by sapiest on 18.10.2019.
//

#ifndef SELSQL_CREATENODE_H
#define SELSQL_CREATENODE_H

#include <map>
#include <string>
#include <utility>
#include "../BaseNode.h"
class CreateNode : public BaseNode {
   public:
    CreateNode(std::string _name, std::vector<VariableNode*> _children)
                                                                                                        : BaseNode(NodeType::CREATE),
                                                                                                          children(std::move(_children)),
                                                                                                          tableName(std::move(_name)) {
    }
    NodeType getNodeType() override { return BaseNode::getNodeType(); }

    ~CreateNode() override {
        for (auto& ch : children) {
            delete ch;
        }
    }

   private:
    std::vector<VariableNode*> children;
    std::string tableName;
};

#endif  // SELSQL_CREATENODE_H
