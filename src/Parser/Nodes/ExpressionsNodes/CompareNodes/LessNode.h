//
// Created by sapiest on 22.10.2019.
//

#ifndef SELSQL_LESSNODE_H
#define SELSQL_LESSNODE_H
#include "CmpNode.h"
class LessNode : public CmpNode {
   public:
    LessNode() = default;
    void accept(TreeVisitor* v) override { v->visit(this); }

};
#endif  // SELSQL_LESSNODE_H
