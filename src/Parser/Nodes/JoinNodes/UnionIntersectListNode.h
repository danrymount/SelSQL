//
// Created by sapiest on 12.11.2019.
//

#ifndef SELSQL_UNIONINTERSECTLISTNODE_H
#define SELSQL_UNIONINTERSECTLISTNODE_H

#include "../ActionNodes/BaseActionNode.h"
#include "UnionIntersectNode.h"
class UnionIntersectListNode : public BaseActionNode {
   public:
    ~UnionIntersectListNode() override {
        for (auto& child : childs) {
            delete child;
        }
    }

    void addUnionIntersectNode(UnionIntersectNode* node) { childs.emplace_back(node); }

    std::vector<UnionIntersectNode*> getChilds() { return childs; }

    void accept(TreeVisitor* v) override { v->visit(this); }

   private:
    std::vector<UnionIntersectNode*> childs;
};

#endif  // SELSQL_UNIONINTERSECTLISTNODE_H
