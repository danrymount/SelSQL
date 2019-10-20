//
// Created by sapiest on 20.10.2019.
//

#ifndef SELSQL_TREEVISITOR_H
#define SELSQL_TREEVISITOR_H

#include "Visitor.h"
class TreeVisitor : public Visitor {
   public:
    TreeVisitor() = default;

    void visit(RootNode* node) override;
    void visit(CreateNode* node) override;
    void visit(DropNode* node) override;
    void visit(ShowCreateNode* node) override;
    void visit(ConstraintNode* node) override;
    void visit(VariableNode* node) override;
};

#endif  // SELSQL_TREEVISITOR_H
