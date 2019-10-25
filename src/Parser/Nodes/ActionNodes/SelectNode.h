//
// Created by sapiest on 21.10.2019.
//

#ifndef SELSQL_SELECTNODE_H
#define SELSQL_SELECTNODE_H
#include "../ExpressionsNodes/BaseExprNode.h"
class SelectNode : public BaseActionNode {
   public:
    SelectNode(std::string _name, BaseNode* _child)
                                                                                                        : BaseActionNode(Action::SELECT,
                                                                                                                         std::move(_name),
                                                                                                                         _child){}

    void accept(TreeVisitor* v) override { v->visit(this); }
};

#endif  // SELSQL_SELECTNODE_H
