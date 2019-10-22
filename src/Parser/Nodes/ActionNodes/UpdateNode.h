//
// Created by sapiest on 21.10.2019.
//

#ifndef SELSQL_UPDATENODE_H
#define SELSQL_UPDATENODE_H
#include "../ExpressionsNodes/BaseExprNode.h"
class UpdateNode : public BaseActionNode {
   public:

    explicit UpdateNode(std::string _name, BaseNode* _child)
                                                                                                        : BaseActionNode(Action::UPDATE,
                                                                                                                         std::move(_name), _child){}

    void accept(TreeVisitor* v) override { v->visit(this); }
};

#endif  // SELSQL_UPDATENODE_H
