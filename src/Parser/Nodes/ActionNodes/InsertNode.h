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
    InsertNode(BaseNode* _source, BaseNode* _children) : BaseActionNode(_source, _children) {}

    void accept(TreeVisitor* v) override { v->visit(this); }
};

#endif  // SELSQL_INSERTNODE_H
