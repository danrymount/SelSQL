//
// Created by sapiest on 21.10.2019.
//

#ifndef SELSQL_SELECTNODE_H
#define SELSQL_SELECTNODE_H
#include "../ExpressionsNodes/BaseExprNode.h"
class SelectNode : public BaseActionNode {
   public:
    SelectNode(BaseNode* _source, BaseNode* _child) : BaseActionNode(_source, _child) {}

    void accept(TreeVisitor* v) override { setMessage(v->visitTemplate(this)); }
};

#endif  // SELSQL_SELECTNODE_H
