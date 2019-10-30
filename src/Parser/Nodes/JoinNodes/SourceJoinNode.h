//
// Created by toxaxab on 28.10.2019.
//

#ifndef SELSQL_SOURCEJOINNODE_H
#define SELSQL_SOURCEJOINNODE_H

#include "BaseJoinNode.h"
class SourceJoinNode : public BaseJoinNode {
   public:
    SourceJoinNode(BaseNode* _source, IdentNode* _alias) : source(_source), alias(_alias) {}

    void accept(TreeVisitor* v) override { v->visit(this); }

    BaseNode* getSource() { return source; }
    IdentNode* getAlias() { return alias; }

   private:
    BaseNode* source = nullptr;
    IdentNode* alias = nullptr;
};

#endif  // SELSQL_SOURCEJOINNODE_H
