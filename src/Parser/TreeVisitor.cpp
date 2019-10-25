//
// Created by sapiest on 20.10.2019.
//

#include "Headers/TreeVisitor.h"
#include <iostream>
#include "../Logic/Actions/Headers/CreateAction.h"
#include "../Logic/Actions/Headers/DeleteAction.h"
#include "../Logic/Actions/Headers/DropAction.h"
#include "../Logic/Actions/Headers/InsertAction.h"
#include "../Logic/Actions/Headers/SelectAction.h"
#include "../Logic/Actions/Headers/ShowCreateAction.h"
#include "../Logic/Actions/Headers/UpdateAction.h"
#include "Headers/CreateVisitor.h"
#include "Headers/DeleteVisitor.h"
#include "Headers/InsertVisitor.h"
#include "Headers/SelectVisitor.h"
#include "Headers/UpdateVisitor.h"
#include "Nodes/ActionNodes/CreateNode.h"
#include "Nodes/ActionNodes/DeleteNode.h"
#include "Nodes/ActionNodes/DropNode.h"
#include "Nodes/ActionNodes/InsertNode.h"
#include "Nodes/ActionNodes/SelectNode.h"
#include "Nodes/ActionNodes/ShowCreateNode.h"
#include "Nodes/ActionNodes/UpdateNode.h"
#include "Nodes/ColumnNode.h"
#include "Nodes/ColumnsAndExprNode.h"
#include "Nodes/ColumnsAndValuesNode.h"
#include "Nodes/ConstraintNode.h"
#include "Nodes/ExpressionsNodes/CompareNodes/EqualsNode.h"
#include "Nodes/ExpressionsNodes/CompareNodes/LessEqNode.h"
#include "Nodes/ExpressionsNodes/CompareNodes/MoreEqNode.h"
#include "Nodes/ExpressionsNodes/CompareNodes/MoreNode.h"
#include "Nodes/ExpressionsNodes/CompareNodes/NoEqualsNode.h"
#include "Nodes/ExpressionsNodes/ExprNode.h"
#include "Nodes/ExpressionsNodes/IndentExprNode.h"
#include "Nodes/ExpressionsNodes/IndentNode.h"
#include "Nodes/ExpressionsNodes/LogicNodes/AndLogicNode.h"
#include "Nodes/ExpressionsNodes/LogicNodes/NotLogicNode.h"
#include "Nodes/ExpressionsNodes/LogicNodes/OrLogicNode.h"
#include "Nodes/ExpressionsNodes/UpdateExprNode.h"
#include "Nodes/RootNode.h"
#include "Nodes/UpdatesAndExprNode.h"
#include "Nodes/ValuesNodes/CharValueNode.h"
#include "Nodes/ValuesNodes/FloatValueNode.h"
#include "Nodes/ValuesNodes/IntValueNode.h"
#include "Nodes/ValuesNodes/NullValueNode.h"
#include "Nodes/VariableListNode.h"
#include "Nodes/VariableNode.h"

void TreeVisitor::visit(RootNode* node) {
    for (auto& child : node->getChildren()) {
        // request->clear(); перед заходом в новую функцию, возможно, стоит отчищать
        child->accept(this);
    }
}

void TreeVisitor::visit(CreateNode* node) {
    auto action = std::make_shared<CreateNode>(*node);
    auto visitor = std::make_shared<CreateVisitor>(CreateVisitor());
    error = CreateAction(visitor).execute(action);
}

void TreeVisitor::visit(DropNode* node) {
    print(node->getTableName());
    auto action = std::make_shared<DropNode>(*node);
    auto visitor = std::make_shared<TreeVisitor>(*this);
    error = DropAction(visitor).execute(action);
}

void TreeVisitor::visit(ShowCreateNode* node) {
    auto action = std::make_shared<ShowCreateNode>(*node);
    auto visitor = std::make_shared<TreeVisitor>(*this);
    error = ShowCreateAction(visitor).execute(action);
}

void TreeVisitor::visit(InsertNode* node) {
    auto visitor = std::make_shared<InsertVisitor>(InsertVisitor());
    auto action = std::make_shared<InsertNode>(*node);
    error = InsertAction(visitor).execute(action);
}

void TreeVisitor::visit(SelectNode* node) {
    auto visitor = std::make_shared<SelectVisitor>(SelectVisitor());
    auto action = std::make_shared<SelectNode>(*node);
    error = SelectAction(visitor).execute(action);
}

void TreeVisitor::visit(UpdateNode* node) {
    auto visitor = std::make_shared<UpdateVisitor>(UpdateVisitor());
    auto action = std::make_shared<UpdateNode>(*node);
    error = UpdateAction(visitor).execute(action);
}

void TreeVisitor::visit(DeleteNode* node) {
    auto visitor = std::make_shared<DeleteVisitor>(DeleteVisitor());
    auto action = std::make_shared<DeleteNode>(*node);
    error = DeleteAction(visitor).execute(action);
}

void TreeVisitor::visit(ConstraintNode* node) {}
void TreeVisitor::visit(VariableNode* node) {}
void TreeVisitor::visit(IntValueNode* node) {}
void TreeVisitor::visit(CharValueNode* node) {}
void TreeVisitor::visit(FloatValueNode* node) {}
void TreeVisitor::visit(NullValueNode* node) {}
void TreeVisitor::visit(ColumnNode* node) {}
void TreeVisitor::visit(ColumnsAndValuesNode* node) {}
void TreeVisitor::visit(ExprNode* node) {}
void TreeVisitor::visit(AndLogicNode* node) {}
void TreeVisitor::visit(OrLogicNode* node) {}
void TreeVisitor::visit(NotLogicNode* node) {}
void TreeVisitor::visit(AddNode* node) {}
void TreeVisitor::visit(DivNode* node) {}
void TreeVisitor::visit(SubNode* node) {}
void TreeVisitor::visit(MultNode* node) {}
void TreeVisitor::visit(EqualsNode* node) {}
void TreeVisitor::visit(NoEqualsNode* node) {}
void TreeVisitor::visit(MoreEqNode* node) {}
void TreeVisitor::visit(MoreNode* node) {}
void TreeVisitor::visit(LessEqNode* node) {}
void TreeVisitor::visit(LessNode* node) {}
void TreeVisitor::visit(IdentNode* node) {}
void TreeVisitor::visit(VariableListNode* node) {}
void TreeVisitor::visit(ColumnsAndExprNode* node) {}
void TreeVisitor::visit(UpdateExprNode* node) {}
void TreeVisitor::visit(UpdatesAndExprNode* node) {}
void TreeVisitor::visit(IndentExprNode* node) {}
void TreeVisitor::visit(ValueExprNode* node) {}
void TreeVisitor::visit(AssignUpdateNode* node) {}
