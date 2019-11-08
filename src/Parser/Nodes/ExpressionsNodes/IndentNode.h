//
// Created by sapiest on 22.10.2019.
//

#ifndef SELSQL_INDENTNODE_H
#define SELSQL_INDENTNODE_H
#include <utility>

#include "BaseExprNode.h"
class IdentNode : public BaseExprNode {
   public:
    explicit IdentNode(std::string _name) { setValue(std::move(_name)); }

    void accept(TreeVisitor* v) override { v->visit(this); }
};
#endif  // SELSQL_INDENTNODE_H
