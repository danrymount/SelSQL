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
    BaseActionNode(BaseNode* _source, BaseNode* _child) : source(_source), child(_child) {}

    explicit BaseActionNode(BaseNode* _source) : source(_source) {}

    ~BaseActionNode() override {
        delete child;
        delete source;
    }

    void setChildren(BaseNode* _child) { child = _child; }
    void setMessage(Message _message) { message = std::move(_message); }

    BaseNode* getChild() { return child; }
    BaseNode* getSource() { return source; }
    Message getMessage() { return message; }

   private:
    BaseNode* child = nullptr;
    BaseNode* source = nullptr;
    Message message;
};

#endif  // SELSQL_BASEACTIONNODE_H
