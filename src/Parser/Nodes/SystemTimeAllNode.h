//
// Created by sapiest on 18.12.2019.
//

#ifndef SELSQL_SYSTEMTIMEALLNODE_H
#define SELSQL_SYSTEMTIMEALLNODE_H

#include "BaseNode.h"
class SystemTimeAllNode : public BaseNode {
   public:
    void accept(TreeVisitor* v) override { v->visit(this); }

   private:
};

#endif  // SELSQL_SYSTEMTIMEALLNODE_H
