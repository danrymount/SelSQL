//
// Created by sapiest on 18.10.2019.
//

#ifndef SELSQL_BASENODE_H
#define SELSQL_BASENODE_H

#include "../Headers/TreeVisitor.h"

class BaseNode {
   public:
    BaseNode() = default;

    virtual ~BaseNode() = default;

    virtual void accept(TreeVisitor* v) = 0;
};

#endif  // SELSQL_BASENODE_H
