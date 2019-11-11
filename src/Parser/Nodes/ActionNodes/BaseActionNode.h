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
    BaseActionNode(Action _action, BaseNode* _source, BaseNode* _child)
                                                                                                        : action(_action),
                                                                                                          source(_source),
                                                                                                          child(_child) {
    }

    BaseActionNode(Action _action, BaseNode* _source) : action(_action), source(_source) {}
    BaseActionNode() = default;

    ~BaseActionNode() override {
        if (child != nullptr) {
            delete child;
        }
        delete source;
    }

    void setChildren(BaseNode* _child) { child = _child; }
    BaseNode* getChild() { return child; }
    BaseNode* getSource() { return source; }
    // std::string getTableName() { return tableName; }
    Action getAction() { return action; }

   private:
    BaseNode* child = nullptr;
    BaseNode* source = nullptr;
    Action action;
};

#endif  // SELSQL_BASEACTIONNODE_H
