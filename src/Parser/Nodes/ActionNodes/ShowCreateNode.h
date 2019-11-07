//
// Created by sapiest on 20.10.2019.
//

#ifndef SELSQL_SHOWCREATENODE_H
#define SELSQL_SHOWCREATENODE_H

#include "BaseActionNode.h"
class ShowCreateNode : public BaseActionNode {
   public:
    explicit ShowCreateNode(BaseNode* _source) : BaseActionNode(_source) {}

    void accept(TreeVisitor* v) override { setMessage(v->visitTemplate(this)); }
};
#endif  // SELSQL_SHOWCREATENODE_H
