//
// Created by sapiest on 20.10.2019.
//

#ifndef SELSQL_SHOWCREATENODE_H
#define SELSQL_SHOWCREATENODE_H

#include "BaseActionNode.h"
class ShowCreateNode : public BaseActionNode {
   public:
    explicit ShowCreateNode(BaseNode* _source) : BaseActionNode(Action::SHOW_CREATE, _source) {}

    void accept(TreeVisitor* v) override { v->visit(this); }
};
#endif  // SELSQL_SHOWCREATENODE_H
