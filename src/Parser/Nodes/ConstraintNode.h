//
// Created by sapiest on 18.10.2019.
//

#ifndef SELSQL_CONSTRAINTNODE_H
#define SELSQL_CONSTRAINTNODE_H

#include "../../Utils/Headers/CommonUtils.h"
#include "BaseNode.h"
class ConstraintNode : public BaseNode {
   public:
    explicit ConstraintNode(Constraint _constraint) : constraint(_constraint) {}

    Constraint getConstraint() { return constraint; }

    void accept(TreeVisitor* v) override { v->visit(this); }

   private:
    Constraint constraint;
};

#endif  // SELSQL_CONSTRAINTNODE_H
