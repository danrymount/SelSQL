//
// Created by sapiest on 21.10.2019.
//

#ifndef SELSQL_DELETENODE_H
#define SELSQL_DELETENODE_H
#include "../ExpressionsNodes/BaseExprNode.h"
class DeleteNode : public BaseActionNode {
   public:
    explicit DeleteNode(BaseNode* _source, BaseNode* _child) : BaseActionNode(_source, _child) {}

    void accept(TreeVisitor* v) override { setMessage(v->visitTemplate(this)); }
};
#endif  // SELSQL_DELETENODE_H
