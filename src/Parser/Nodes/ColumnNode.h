//
// Created by sapiest on 21.10.2019.
//

#ifndef SELSQL_COLUMNNODE_H
#define SELSQL_COLUMNNODE_H
#include <utility>

#include "BaseNode.h"
class ColumnNode : public BaseNode {
   public:
    // explicit ColumnNode(std::string _name) : name(std::move(_name)) {}
    explicit ColumnNode(IdentNode* _column) : column(_column) {}

    ColumnNode(IdentNode* _alias, IdentNode* _column) : alias(_alias), column(_column) {}

    // std::string getName() { return name; }

    IdentNode* getAlias() { return alias; }
    IdentNode* getColumn() { return column; }

    void accept(TreeVisitor* v) override { v->visit(this); }

   private:
    IdentNode* alias = nullptr;
    IdentNode* column = nullptr;
};

#endif  // SELSQL_COLUMNNODE_H
