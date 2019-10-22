//
// Created by sapiest on 20.10.2019.
//

#ifndef SELSQL_TREEVISITOR_H
#define SELSQL_TREEVISITOR_H

#include <memory>
#include "../../Utils/Structures/BigResponse.h"
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
    void visit(ExprNode* node) override;
    void visit(AndLogicNode* node) override;
    void visit(OrLogicNode* node) override;
    void visit(NotLogicNode* node) override;
    void visit(AddNode* node) override;
    void visit(DivNode* node) override;
    void visit(SubNode* node) override;
    void visit(MultNode* node) override;

    std::shared_ptr<BigResponse> getResponse() { return response; }

   private:
    std::shared_ptr<BigRequest> request;
    std::shared_ptr<BigResponse> response;
};

#endif  // SELSQL_TREEVISITOR_H
