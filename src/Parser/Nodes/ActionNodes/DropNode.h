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
    DropNode(std::string _name, std::vector<BaseNode*> _children)
                                                                                                        : BaseActionNode(Action::DROP,
                                                                                                                         std::move(_name),
                                                                                                                         std::move(_children)) {
    }

    void accept(TreeVisitor* v) override { v->visit(this); }
};

#endif  // SELSQL_DROPNODE_H
