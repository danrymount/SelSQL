//
// Created by sapiest on 21.10.2019.
//

#ifndef SELSQL_UPDATENODE_H
#define SELSQL_UPDATENODE_H
#include "../ExpressionsNodes/BaseExprNode.h"
class UpdateNode : public BaseActionNode {
   public:
    explicit UpdateNode(BaseNode* _source, BaseNode* _child) : BaseActionNode(_source, _child) {}

    void accept(TreeVisitor* v) override { setMessage(v->visitTemplate(this)); }
};

#endif  // SELSQL_UPDATENODE_H
