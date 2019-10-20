//
// Created by sapiest on 20.10.2019.
//

#ifndef SELSQL_BASEACTIONNODE_H
#define SELSQL_BASEACTIONNODE_H
#include <functional>
#include <utility>

#include "../BaseNode.h"
#include "../VariableNode.h"
class BaseActionNode : public BaseNode {
   public:
    BaseActionNode(NodeType _nodeType, std::string _name, std::vector<VariableNode*> _children)
                                                                                                        : BaseNode(_nodeType),
                                                                                                          tableName(std::move(_name)),
                                                                                                          children(std::move(_children)) {
    }

    BaseActionNode(NodeType _nodeType, std::string _name) : BaseNode(_nodeType), tableName(std::move(_name)) {}

    NodeType getNodeType() override { return BaseNode::getNodeType(); }

    ~BaseActionNode() override {
        for (auto& ch : children) {
            delete ch;
        }
    }

    std::vector<VariableNode*> getChildren() { return children; }
    std::string getTableName() { return tableName; }

   private:
    std::vector<VariableNode*> children;
    std::string tableName;
};

#endif  // SELSQL_BASEACTIONNODE_H
