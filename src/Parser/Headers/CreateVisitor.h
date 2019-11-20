//
// Created by sapiest on 24.10.2019.
//

#ifndef SELSQL_CREATEVISITOR_H
#define SELSQL_CREATEVISITOR_H
#include <utility>

#include "../Nodes/ActionNodes/CreateNode.h"
#include "../Nodes/ConstraintNode.h"
#include "../Nodes/ExpressionsNodes/IndentNode.h"
#include "../Nodes/VariableListNode.h"
#include "../Nodes/VariableNode.h"
#include "TreeVisitor.h"
class CreateVisitor : public TreeVisitor {
   public:
    explicit CreateVisitor(std::shared_ptr<MainEngine> _engine) : TreeVisitor(std::move(_engine)) {}

    void visit(CreateNode* node) override {
        node->getSource()->accept(this);
        node->getChild()->accept(this);
    }

    void visit(VariableListNode* node) override {
        for (auto& var : node->getVariables()) {
            var->accept(this);
        }
    }

    void visit(IdentNode* node) override { table.name = node->getBaseValue(); }

    void visit(ConstraintNode* node) override {
        if (contraints.find(node->getConstraint()) == contraints.end()) {
            contraints.insert(std::make_pair(node->getConstraint(), 1));
            table.addConstraint(node->getConstraint());
        } else {
            error = Message(ErrorConstants::ERR_SAME_CONSTRAINT);
        }
    }

    void visit(VariableNode* node) override {
        if (values.find(node->getVarName()) == values.end()) {
            values.insert(std::make_pair(node->getVarName(), 1));
            if (node->getVarType() == Type::TYPE_CHAR) {
                table.addFiledWithSize(node->getVarName(), node->getVarType(), node->getSize());
            } else {
                table.addField(node->getVarName(), node->getVarType());
            }
            for (auto& child : node->getConstraints()) {
                child->accept(this);
            }
            contraints.clear();
        } else {
            error = Message(ErrorConstants::ERR_SAME_FIELD_NAME);
        }
    }

    Table getTable() { return table; }

    Message getError() { return error; }

   private:
    Message error;
    std::map<std::string, int> values;
    std::map<Constraint, int> contraints;
    Table table;
};

#endif  // SELSQL_CREATEVISITOR_H
