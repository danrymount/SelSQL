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
    explicit CreateNode(std::map<NodeType, BaseNode*> _children)
                                                                                                        : BaseNode(NodeType::CREATE),
                                                                                                          children(std::move(_children)) {
    }
    NodeType getNodeType() override { return BaseNode::getNodeType(); }

    ~CreateNode() override {
        for (auto& ch : children) {
            delete ch.second;
        }
    }

   private:
    std::map<NodeType, BaseNode*> children;
};

#endif  // SELSQL_CREATENODE_H
