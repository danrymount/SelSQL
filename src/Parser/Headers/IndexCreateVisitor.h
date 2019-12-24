//
// Created by sapiest on 25.12.2019.
//

#ifndef SELSQL_INDEXCREATEVISITOR_H
#define SELSQL_INDEXCREATEVISITOR_H
#include "../Nodes/ActionNodes/IndexNode.h"
#include "../Nodes/ColumnNode.h"
#include "../Nodes/ExpressionsNodes/IndentNode.h"
#include "../Nodes/TableNode.h"
#include "TreeVisitor.h"

class IndexCreateVisitor : public TreeVisitor {
   public:
    explicit IndexCreateVisitor(std::shared_ptr<MainEngine> _engine) : TreeVisitor(std::move(_engine)){};

    void visit(IndexNode* node) override {
        node->getSource()->accept(this);
        node->getChild()->accept(this);
    }

    void visit(TableNode* node) override { tableName = node->getChild()->getBaseValue(); }

    void visit(ColumnNode* node) override { colName = node->getColumn()->getBaseValue(); }

    std::string getTableName() { return tableName; }

    std::string getColName() { return colName; }

   private:
    std::string tableName;
    std::string colName;
};
#endif  // SELSQL_INDEXCREATEVISITOR_H
