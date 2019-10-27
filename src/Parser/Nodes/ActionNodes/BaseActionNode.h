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
    BaseActionNode(Action _action, std::string _name, BaseNode* _child)
                                                                                                        : action(_action),
                                                                                                          tableName(std::move(_name)),
                                                                                                          child(_child) {
    }

    BaseActionNode(Action _action, std::string _name) : action(_action), tableName(std::move(_name)) {}

    ~BaseActionNode() override {
        if (child != NULL) {
            delete child;
        }
    }

    void setChildren(BaseNode* _child) { child = _child; }
    BaseNode* getChild() { return child; }
    std::string getTableName() { return tableName; }
    Action getAction() { return action; }

   private:
    BaseNode* child;
    std::string tableName;
    Action action;
};

#endif  // SELSQL_BASEACTIONNODE_H
