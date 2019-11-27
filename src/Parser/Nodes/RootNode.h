//
// Created by sapiest on 18.10.2019.
//

#ifndef SELSQL_ROOTNODE_H
#define SELSQL_ROOTNODE_H

#include <map>
#include <utility>
#include "ActionNodes/BaseActionNode.h"
#include "BaseNode.h"
class RootNode : public BaseNode {
   public:
    explicit RootNode(std::vector<BaseActionNode*> _children) : children(std::move(_children)) {}

    ~RootNode() override {
        for (auto& ch : children) {
            delete ch;
        }
    }

    std::vector<BaseActionNode*> getChildren() { return children; }

    void accept(TreeVisitor* v) override { v->visit(this); }

    void isTransaction(int _transactionFlag) { transactionFlag = _transactionFlag; }

    int isTransaction() { return transactionFlag; }

   private:
    std::vector<BaseActionNode*> children;
    int transactionFlag = false;
    // std::map<NodeType, BaseNode*> children;
};

#endif  // SELSQL_ROOTNODE_H
