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
    BaseActionNode(Action _action, std::string _name, std::vector<VariableNode*> _children)
                                                                                                        : action(_action),
                                                                                                          tableName(std::move(_name)),
                                                                                                          children(std::move(_children)) {
    }

    BaseActionNode(Action _action, std::string _name) : action(_action), tableName(std::move(_name)) {}

    ~BaseActionNode() override {
        for (auto& ch : children) {
            delete ch;
        }
    }

    std::vector<VariableNode*> getChildren() { return children; }
    std::string getTableName() { return tableName; }
    Action getAction() { return action; }

   private:
    std::vector<VariableNode*> children;
    std::string tableName;
    Action action;
};

#endif  // SELSQL_BASEACTIONNODE_H