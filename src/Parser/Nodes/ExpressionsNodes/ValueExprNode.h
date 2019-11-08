//
// Created by sapiest on 23.10.2019.
//

#ifndef SELSQL_VALUEEXPRNODE_H
#define SELSQL_VALUEEXPRNODE_H
#include <utility>

#include "BaseExprNode.h"
class ValueExprNode : public BaseExprNode {
   public:
    explicit ValueExprNode(std::string _name) { setValue(std::move(_name)); }

    void accept(TreeVisitor* v) override { v->visit(this); }
};

#endif  // SELSQL_VALUEEXPRNODE_H
