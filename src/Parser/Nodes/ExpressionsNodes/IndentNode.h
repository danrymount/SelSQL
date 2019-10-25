//
// Created by sapiest on 22.10.2019.
//

#ifndef SELSQL_INDENTNODE_H
#define SELSQL_INDENTNODE_H
#include <utility>

#include "../BaseNode.h"
class IdentNode : public BaseExprNode {
   public:
    explicit IdentNode(std::string _name) : name(std::move(_name)) {}

    std::string getName() { return name; }

    std::string getBaseValue() override {  return name; }

    void accept(TreeVisitor* v) override { v->visit(this); }

   private:
    std::string name;
};
#endif  // SELSQL_INDENTNODE_H
