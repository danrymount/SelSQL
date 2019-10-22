//
// Created by sapiest on 21.10.2019.
//

#ifndef SELSQL_UPDATENODE_H
#define SELSQL_UPDATENODE_H
#include "../ExpressionsNodes/BaseExprNode.h"
class UpdateNode : public BaseActionNode {
   public:
    UpdateNode(std::string _name, BaseNode* _child, BaseExprNode* _expr)
                                                                                                        : BaseActionNode(Action::UPDATE,
                                                                                                                         std::move(_name),
                                                                                                                         _child),
                                                                                                          expr(_expr) {}

    explicit UpdateNode(std::string _name, BaseExprNode* _expr)
                                                                                                        : BaseActionNode(Action::UPDATE,
                                                                                                                         std::move(_name)),
                                                                                                          expr(_expr) {}

    void accept(TreeVisitor* v) override { v->visit(this); }

    BaseExprNode* getExpr() { return expr; }

   private:
    BaseExprNode* expr = nullptr;
};

#endif  // SELSQL_UPDATENODE_H
