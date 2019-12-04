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

    void setId(long _id) { id = _id; }
    long getId() { return id; }

   private:
    BaseNode* child = nullptr;
    BaseNode* source = nullptr;
    long id;
};

#endif  // SELSQL_BASEACTIONNODE_H
