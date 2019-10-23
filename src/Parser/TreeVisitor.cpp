//
// Created by sapiest on 20.10.2019.
//

#include "Headers/TreeVisitor.h"
#include <iostream>
#include "../Logic/Actions/Headers/InsertAction.h"
#include "../Logic/Actions/Headers/SelectAction.h"
#include "../Logic/Headers/MainLogic.h"
#include "Headers/InsertVisitor.h"
#include "Headers/SelectVisitor.h"
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
    std::cout << "ROOT" << std::endl;
    request = std::make_shared<BigRequest>();
    response = std::make_shared<BigResponse>();
    for (auto& child : node->getChildren()) {
        // request->clear(); перед заходом в новую функцию, возможно, стоит отчищать
        child->accept(this);
    }
}

void TreeVisitor::visit(CreateNode* node) {
    request->action = node->getAction();
    request->tableName = node->getTableName();
    std::cout << "CREATE" << std::endl;
    std::cout << node->getTableName() << std::endl;
    node->getChild()->accept(this);
}

void TreeVisitor::visit(DropNode* node) {
    request->action = node->getAction();
    request->tableName = node->getTableName();
    std::cout << "DROP" << std::endl;
    std::cout << node->getTableName() << std::endl;
    // response = std::make_shared<BigResponse>(MainLogic::executeRequest(request));
}

void TreeVisitor::visit(ShowCreateNode* node) {
    request->action = node->getAction();
    request->tableName = node->getTableName();
    std::cout << "SHOWCREATE" << std::endl;
    std::cout << node->getTableName() << std::endl;
}

void TreeVisitor::visit(InsertNode* node) {
    request->action = node->getAction();
    request->tableName = node->getTableName();
    std::cout << "INSERT" << std::endl;
    auto visitor = std::make_shared<InsertVisitor>(InsertVisitor());
    auto action = std::make_shared<InsertNode>(*node);
    InsertAction(visitor).execute(action);

    // response = std::make_shared<BigResponse>(MainLogic::executeRequest(request));
}

void TreeVisitor::visit(SelectNode* node) {
    auto visitor = std::make_shared<SelectVisitor>(SelectVisitor());
    auto action = std::make_shared<SelectNode>(*node);
    SelectAction(visitor).execute(action);
}

void TreeVisitor::visit(UpdateNode* node) {}
void TreeVisitor::visit(DeleteNode* node) {}

void TreeVisitor::visit(ConstraintNode* node) {
    std::cout << "CONSTRAINT = ";
    std::cout << node->getConstraint() << std::endl;
    request->ddlData.table.addConstraint(node->getConstraint());
}

void TreeVisitor::visit(VariableNode* node) {
    std::cout << "VAR = ";
    std::cout << node->getVarName() << " TYPE = " << node->getVarType() << std::endl;
    request->ddlData.table.addField(node->getVarName(), node->getVarType());
    for (auto& child : node->getConstraints()) {
        child->accept(this);
    }
}
void TreeVisitor::visit(IntValueNode* node) {
    std::cout << node->getValue() << std::endl;
    // request->dmlData.values.emplace_back(node->getValue());
}

void TreeVisitor::visit(CharValueNode* node) { std::cout << node->getValue() << std::endl; }

void TreeVisitor::visit(FloatValueNode* node) { std::cout << node->getValue() << std::endl; }

void TreeVisitor::visit(NullValueNode* node) { std::cout << node->getValue() << std::endl; }

void TreeVisitor::visit(ColumnNode* node) { std::cout << node->getName() << std::endl; }

void TreeVisitor::visit(ColumnsAndValuesNode* node) {
    for (auto& column : node->getColumns()) {
        column->accept(this);
    }
    for (auto& value : node->getValues()) {
        value->accept(this);
    }
}

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

void TreeVisitor::visit(VariableListNode* node) {
    for (auto& var : node->getVariables()) {
        var->accept(this);
    }
}

void TreeVisitor::visit(ColumnsAndExprNode* node) {}
void TreeVisitor::visit(UpdateExprNode* node) {}
void TreeVisitor::visit(UpdatesAndExprNode* node) {}
void TreeVisitor::visit(IndentExprNode* node) {}
void TreeVisitor::visit(ValueExprNode* node) {}

void TreeVisitor::visit(LogicNode* node) {}
