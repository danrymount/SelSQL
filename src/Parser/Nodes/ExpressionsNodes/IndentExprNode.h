//
// Created by sapiest on 23.10.2019.
//

#ifndef SELSQL_INDENTEXPRNODE_H
#define SELSQL_INDENTEXPRNODE_H
#include <utility>

#include "BaseExprNode.h"
class IndentExprNode : public BaseExprNode {
   public:
    explicit IndentExprNode(std::string _name) : name(std::move(_name)) {}
    IndentExprNode(std::string _aliasName, std::string _name)
                                                                                                        : aliasName(std::move(_aliasName)),
                                                                                                          name(std::move(_name)) {
    }

    std::string getBaseValue() override { return name; }

    std::string getAliasname() { return aliasName; }

    void accept(TreeVisitor* v) override { v->visit(this); }

   private:
    std::string aliasName;
    std::string name;
};
#endif  // SELSQL_INDENTEXPRNODE_H
