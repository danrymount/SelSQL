//
// Created by sapiest on 21.10.2019.
//

#ifndef SELSQL_COLUMNNODE_H
#define SELSQL_COLUMNNODE_H
#include <utility>

#include "BaseNode.h"
class ColumnNode : public BaseNode {
   public:
    explicit ColumnNode(std::string _name) : name(std::move(_name)) {}

    std::string getName() { return name; }

    void accept(TreeVisitor* v) override { v->visit(this); }

   private:
    std::string name;
};

#endif  // SELSQL_COLUMNNODE_H
