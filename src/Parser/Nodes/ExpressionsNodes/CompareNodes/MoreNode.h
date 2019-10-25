//
// Created by sapiest on 22.10.2019.
//

#ifndef SELSQL_MORENODE_H
#define SELSQL_MORENODE_H
#include "CmpNode.h"
class MoreNode : public CmpNode {
   public:
    MoreNode() = default;
    void accept(TreeVisitor* v) override { v->visit(this); }
};
#endif  // SELSQL_MORENODE_H
