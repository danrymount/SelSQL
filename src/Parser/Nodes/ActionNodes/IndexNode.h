//
// Created by sapiest on 24.12.2019.
//

#ifndef SELSQL_INDEXNODE_H
#define SELSQL_INDEXNODE_H
#include "BaseActionNode.h"
class IndexNode : public BaseActionNode {
   public:
    IndexNode(BaseNode* tableNode, BaseNode* columnNode) : BaseActionNode(tableNode, columnNode) {}

    void accept(TreeVisitor* v) override { v->visit(this); }
};
#endif  // SELSQL_INDEXNODE_H
