//
// Created by sapiest on 21.10.2019.
//

#ifndef SELSQL_DELETENODE_H
#define SELSQL_DELETENODE_H
#include "../ExpressionsNodes/BaseExprNode.h"
class DeleteNode : public BaseActionNode {
   public:
//    DeleteNode(std::string _name, BaseNode* _child, BaseExprNode* _expr)
//                                                                                                        : BaseActionNode(Action::DELETE_ACT,
//                                                                                                                         std::move(_name),
//                                                                                                                         _child),
//                                                                                                          expr(_expr) {}

    explicit DeleteNode(std::string _name, BaseExprNode* _expr)
                                                                                                        : BaseActionNode(Action::DELETE_ACT,
                                                                                                                         std::move(_name)),
                                                                                                          expr(_expr) {}

    void accept(TreeVisitor* v) override { v->visit(this); }

    BaseExprNode* getExpr() { return expr; }

   private:
    BaseExprNode* expr = nullptr;
};
#endif  // SELSQL_DELETENODE_H
