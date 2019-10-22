//
// Created by sapiest on 20.10.2019.
//

#ifndef SELSQL_DROPNODE_H
#define SELSQL_DROPNODE_H

#include <string>
#include <vector>
#include "../BaseNode.h"
#include "../VariableNode.h"
#include "BaseActionNode.h"
class DropNode : public BaseActionNode {
   public:
    explicit DropNode(std::string _name) : BaseActionNode(Action::DROP, std::move(_name)) {}

    void accept(TreeVisitor* v) override { v->visit(this); }
};

#endif  // SELSQL_DROPNODE_H
