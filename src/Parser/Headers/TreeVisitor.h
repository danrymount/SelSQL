//
// Created by sapiest on 20.10.2019.
//

#ifndef SELSQL_TREEVISITOR_H
#define SELSQL_TREEVISITOR_H

#include <memory>
#include "../../Utils/Structures/Message.h"
#include "Visitor.h"
class TreeVisitor : public Visitor {
   public:
    TreeVisitor() = default;

    void visit(RootNode* node) override;
    void visit(CreateNode* node) override;
    void visit(DropNode* node) override;
    void visit(ShowCreateNode* node) override;
    void visit(ConstraintNode* node) override;
    void visit(VariableNode* node) override;

    void visit(IntValueNode* node) override;
    void visit(CharValueNode* node) override;
    void visit(FloatValueNode* node) override;
    void visit(NullValueNode* node) override;

    void visit(InsertNode* node) override;
    void visit(SelectNode* node) override;
    void visit(UpdateNode* node) override;
    void visit(DeleteNode* node) override;

    void visit(ColumnsAndValuesNode* node) override;
    void visit(ColumnNode* node) override;
    void visit(IdentNode* node) override;

    void visit(ExprNode* node) override;
    void visit(AndLogicNode* node) override;
    void visit(OrLogicNode* node) override;
    void visit(NotLogicNode* node) override;

    void visit(AddNode* node) override;
    void visit(DivNode* node) override;
    void visit(SubNode* node) override;
    void visit(MultNode* node) override;

    void visit(EqualsNode* node) override;
    void visit(NoEqualsNode* node) override;
    void visit(MoreEqNode* node) override;
    void visit(MoreNode* node) override;
    void visit(LessEqNode* node) override;
    void visit(LessNode* node) override;
    void visit(VariableListNode* node) override;
    void visit(ColumnsAndExprNode* node) override;
    void visit(IndentExprNode* node) override;

    void visit(SourceJoinNode* node) override;
    void visit(JoinNode* node) override;
    void visit(TableNode* node) override;

    void visit(UpdateExprNode* node) override;
    void visit(ValueExprNode* node) override;
    void visit(UpdatesAndExprNode* node) override;
    void visit(AssignUpdateNode* node) override;

    Message getMessage() { return message; }


protected:
    Message message;
};

#endif  // SELSQL_TREEVISITOR_H
