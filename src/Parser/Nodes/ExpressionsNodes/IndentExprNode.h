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

    void accept(TreeVisitor* v) override { v->visit(this); }

    std::string getName() { return name; }

    std::string getAliasName() { return aliasName; }

   private:
    std::string name;
    std::string aliasName;
};
#endif  // SELSQL_INDENTEXPRNODE_H
