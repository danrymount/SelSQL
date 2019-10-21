//
// Created by sapiest on 20.10.2019.
//

#include <iostream>
#include "Nodes/ActionNodes/CreateNode.h"
#include "Nodes/ActionNodes/DeleteNode.h"
#include "Nodes/ActionNodes/DropNode.h"
#include "Nodes/ActionNodes/InsertNode.h"
#include "Nodes/ActionNodes/SelectNode.h"
#include "Nodes/ActionNodes/ShowCreateNode.h"
#include "Nodes/ActionNodes/UpdateNode.h"
#include "Nodes/ColumnNode.h"
#include "Nodes/ColumnsAndValuesNode.h"
#include "Nodes/ConstraintNode.h"
#include "Nodes/RootNode.h"
#include "Nodes/ValuesNodes/CharValueNode.h"
#include "Nodes/ValuesNodes/FloatValueNode.h"
#include "Nodes/ValuesNodes/IntValueNode.h"
#include "Nodes/ValuesNodes/NullValueNode.h"
#include "Nodes/VariableNode.h"

#include "../Logic/Headers/MainLogic.h"
#include "Headers/TreeVisitor.h"
void TreeVisitor::visit(RootNode* node) {
    if (node == nullptr)
        return;
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
    for (auto& child : node->getChildren()) {
        child->accept(this);
    }
    response = std::make_shared<BigResponse>(MainLogic::executeRequest(request));
}

void TreeVisitor::visit(DropNode* node) {
    request->action = node->getAction();
    request->tableName = node->getTableName();
    std::cout << "DROP" << std::endl;
    std::cout << node->getTableName() << std::endl;
    for (auto& child : node->getChildren()) {
        child->accept(this);
    }
    response = std::make_shared<BigResponse>(MainLogic::executeRequest(request));
}

void TreeVisitor::visit(ShowCreateNode* node) {
    request->action = node->getAction();
    request->tableName = node->getTableName();
    std::cout << "SHOWCREATE" << std::endl;
    std::cout << node->getTableName() << std::endl;
    for (auto& child : node->getChildren()) {
        child->accept(this);
    }
    response = std::make_shared<BigResponse>(MainLogic::executeRequest(request));
}

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

void TreeVisitor::visit(InsertNode* node) {}

void TreeVisitor::visit(SelectNode* node) {}
void TreeVisitor::visit(UpdateNode* node) {}
void TreeVisitor::visit(DeleteNode* node) {}

void TreeVisitor::visit(ColumnsAndValuesNode* node) {}
