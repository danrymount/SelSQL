//
// Created by sapiest on 23.10.2019.
//

#ifndef SELSQL_INDENTEXPRNODE_H
#define SELSQL_INDENTEXPRNODE_H
#include <utility>

#include "BaseExprNode.h"
class IndentExprNode : public BaseExprNode {
   public:
    explicit IndentExprNode(std::string _name) : name(std::move(_name)) {}

    std::string getValue() { return name; }

    std::string getBaseValue() override { return name; }

    void accept(TreeVisitor* v) override { v->visit(this); }

   private:
    std::string name;
};
#endif  // SELSQL_INDENTEXPRNODE_H
