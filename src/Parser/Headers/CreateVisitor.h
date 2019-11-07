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
    Message visitTemplate(CreateNode* node) override {
        node->getSource()->accept(this);
        // node->getChild()->accept(this);
        if (!message.getErrorCode()) {
            auto varList = static_cast<VariableListNode*>(node->getChild())->getVariables();
            auto tableName = static_cast<IdentNode*>(node->getSource())->getBaseValue();
            table = Table(tableName, varList);
        }
        return message;
    }

    //    void visit(CreateNode* node) override {
    //        node->getSource()->accept(this);
    //        node->getChild()->accept(this);
    //    }

    void visit(VariableListNode* node) override {
        for (auto& var : node->getVariablesNodes()) {
            var->accept(this);
            if (message.getErrorCode()) {
                return;
            }
            node->addVariable(var->getVariable());
        }
    }

    std::pair<std::string, Variable> visitTemplate(VariableNode* node) override {
        if (values.find(node->getVarName()) == values.end()) {
            std::pair<std::string, Variable> var;
            std::vector<Constraint> constraints;
            values.insert(std::make_pair(node->getVarName(), 1));
            for (auto& child : node->getConstraints()) {
                child->accept(this);
                if (message.getErrorCode()) {
                    return var;
                }
                constraints.emplace_back(child->getConstraint());
            }
            if (node->getVarType() == Type::TYPE_CHAR) {
                var = std::make_pair(node->getVarName(), Variable(node->getVarType(), node->getSize(), constraints));
                // table.addFiledWithSize(node->getVarName(), node->getVarType(), node->getSize());
            } else {
                var = std::make_pair(node->getVarName(), Variable(node->getVarType(), constraints));
                // table.addField(node->getVarName(), node->getVarType());
            }
            contraints.clear();
            return var;
        } else {
            message = Message(ErrorConstants::ERR_SAME_FIELD_NAME);
        }
    }

    // void visit(IdentNode* node) override { }//table.name = node->getBaseValue(); }

    void visit(ConstraintNode* node) override {
        if (contraints.find(node->getConstraint()) == contraints.end()) {
            contraints.insert(std::make_pair(node->getConstraint(), 1));
            // table.addConstraint(node->getConstraint());
        } else {
            message = Message(ErrorConstants::ERR_SAME_CONSTRAINT);
        }
    }

    //    void visit(VariableNode* node) override {
    //        if (values.find(node->getVarName()) == values.end()) {
    //            values.insert(std::make_pair(node->getVarName(), 1));
    //            if (node->getVarType() == Type::TYPE_CHAR) {
    //                //table.addFiledWithSize(node->getVarName(), node->getVarType(), node->getSize());
    //            } else {
    //                //table.addField(node->getVarName(), node->getVarType());
    //            }
    //            for (auto& child : node->getConstraints()) {
    //                child->accept(this);
    //            }
    //            contraints.clear();
    //        } else {
    //            error = Message(ErrorConstants::ERR_SAME_FIELD_NAME);
    //        }
    //    }

    Table getTable() { return table; }

    // Message getError() { return error; }

   private:
    std::map<std::string, int> values;
    std::map<Constraint, int> contraints;
    Table table;
};

#endif  // SELSQL_CREATEVISITOR_H
