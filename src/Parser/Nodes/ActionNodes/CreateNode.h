//
// Created by sapiest on 18.10.2019.
//

#ifndef SELSQL_CREATENODE_H
#define SELSQL_CREATENODE_H

#include <map>
#include <string>
#include <utility>
#include "../BaseNode.h"
#include "BaseActionNode.h"
class CreateNode : public BaseActionNode {
   public:
    CreateNode(BaseNode* _source, BaseNode* _child) : BaseActionNode(_source, _child) {}

    void accept(TreeVisitor* v) override { v->visit(this); }
};

#endif  // SELSQL_CREATENODE_H
