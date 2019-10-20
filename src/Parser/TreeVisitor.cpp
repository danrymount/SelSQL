//
// Created by sapiest on 20.10.2019.
//

#include <iostream>
#include "Nodes/ActionNodes/CreateNode.h"
#include "Nodes/ActionNodes/DropNode.h"
#include "Nodes/ActionNodes/ShowCreateNode.h"
#include "Nodes/BaseNode.h"
#include "Nodes/ConstraintNode.h"
#include "Nodes/RootNode.h"
#include "Nodes/VariableNode.h"

#include "Headers/TreeVisitor.h"
void TreeVisitor::visit(RootNode* node) {
    std::cout << "ROOT" << std::endl;
    for (auto& child : node->getChildren()) {
        this->visit(child);
    }
}

void TreeVisitor::visit(CreateNode* node) {
    std::cout << "CREATE" << std::endl;
    std::cout << node->getTableName() << std::endl;
    for (auto& child : node->getChildren()) {
        this->visit(child);
    }
}

void TreeVisitor::visit(DropNode* node) {
    std::cout << "DROP" << std::endl;
    std::cout << node->getTableName() << std::endl;
    for (auto& child : node->getChildren()) {
        this->visit(child);
    }
}

void TreeVisitor::visit(ShowCreateNode* node) {
    std::cout << "SHOWCREATE" << std::endl;
    std::cout << node->getTableName() << std::endl;
    for (auto& child : node->getChildren()) {
        this->visit(child);
    }
}
void TreeVisitor::visit(ConstraintNode* node) {
    std::cout << "CONSTRAINT = ";
    std::cout << node->getConstraint() << std::endl;
}

void TreeVisitor::visit(VariableNode* node) {
    std::cout << "VAR = ";
    std::cout << node->getVarName() << " TYPE = " << node->getVarType() << std::endl;
    for (auto& child : node->getConstraints()) {
        this->visit(child);
    }
}

void TreeVisitor::visit(BaseNode* node) { std::cout << "BASENODE" << std::endl; }

void TreeVisitor::visit(BaseActionNode* node) {
    std::cout << "ACTION" << std::endl;
    std::cout << node->getTableName() << std::endl;
    for (auto& child : node->getChildren()) {
        this->visit(child);
    }
}
