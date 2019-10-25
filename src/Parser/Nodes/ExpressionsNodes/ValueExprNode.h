//
// Created by sapiest on 23.10.2019.
//

#ifndef SELSQL_VALUEEXPRNODE_H
#define SELSQL_VALUEEXPRNODE_H
#include "BaseExprNode.h"
class ValueExprNode : public BaseExprNode {
   public:
    explicit ValueExprNode(std::string _name) : name(std::move(_name)) {}

    std::string getName() { return name; }

    std::string getBaseValue() override {  return name; }

    void accept(TreeVisitor* v) override { v->visit(this); }

   private:
    std::string name;
};

#endif  // SELSQL_VALUEEXPRNODE_H
