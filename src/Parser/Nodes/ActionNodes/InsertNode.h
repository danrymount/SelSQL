//
// Created by sapiest on 21.10.2019.
//

#ifndef SELSQL_INSERTNODE_H
#define SELSQL_INSERTNODE_H
#include <utility>

#include "../ValuesNodes/BaseValueNode.h"
#include "BaseActionNode.h"
class InsertNode : public BaseActionNode {
   public:
    InsertNode(std::string _name, ColumnsAndValuesNode* child) : BaseActionNode(Action::INSERT, std::move(_name)) {
        std::vector<BaseNode*> t;
        t.emplace_back(child);
        BaseActionNode::setChildren(t);
    }

    void accept(TreeVisitor* v) override { v->visit(this); }
};

#endif  // SELSQL_INSERTNODE_H
