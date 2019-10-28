//
// Created by toxaxab on 28.10.2019.
//

#ifndef SELSQL_JOINNODE_H
#define SELSQL_JOINNODE_H
#include "../BaseNode.h"
#include "SourceJoinNode.h"
class JoinNode : public BaseNode {
   public:
    JoinNode(SourceJoinNode* _source) : sourceJoinNode(_source) {}

    void accept(TreeVisitor* v) override {
        // TODO
    }

   private:
    SourceJoinNode* sourceJoinNode;
};

#endif  // SELSQL_JOINNODE_H
