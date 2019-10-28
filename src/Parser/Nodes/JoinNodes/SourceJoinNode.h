//
// Created by toxaxab on 28.10.2019.
//

#ifndef SELSQL_SOURCEJOINNODE_H
#define SELSQL_SOURCEJOINNODE_H
#include "../BaseNode.h"
class SourceJoinNode : public BaseNode {
   public:
    SourceJoinNode(BaseNode* _source, IdentNode* _alias) : source(_source), alias(_alias) {}

    void accept(TreeVisitor* v) override {
        // TODO;
    }

   private:
    BaseNode* source = nullptr;
    IdentNode* alias = nullptr;
};

#endif  // SELSQL_SOURCEJOINNODE_H
