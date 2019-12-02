//
// Created by sapiest on 11.11.2019.
//

#ifndef SELSQL_UNIONINTERSECTNODE_H
#define SELSQL_UNIONINTERSECTNODE_H
#include <vector>
#include "../ActionNodes/BaseActionNode.h"
#include "../ActionNodes/SelectNode.h"
#include "BaseJoinNode.h"
class UnionIntersectNode : public BaseActionNode {
   public:
    ~UnionIntersectNode() override {
        for (auto child : childs) {
            delete child;
        }
    }

    void addChild(SelectNode* node) { childs.emplace_back(node); }

    std::vector<SelectNode*> getChildren() { return childs; }

   private:
    std::vector<SelectNode*> childs;
};

#endif  // SELSQL_UNIONINTERSECTNODE_H
