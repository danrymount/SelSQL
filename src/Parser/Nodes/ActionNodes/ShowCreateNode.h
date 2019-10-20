//
// Created by sapiest on 20.10.2019.
//

#ifndef SELSQL_SHOWCREATENODE_H
#define SELSQL_SHOWCREATENODE_H

#include "BaseActionNode.h"
class ShowCreateNode : public BaseActionNode {
   public:
    ShowCreateNode(std::string _name) : BaseActionNode(NodeType::SHOWCREATE, std::move(_name)) {}

    void accept(TreeVisitor v) override { v.visit(this); }
};
#endif  // SELSQL_SHOWCREATENODE_H
