//
// Created by sapiest on 18.10.2019.
//

#ifndef SELSQL_CONSTRAINTSLISTNODE_H
#define SELSQL_CONSTRAINTSLISTNODE_H

#include "../../Utils/Headers/CommonUtils.h"
#include "ConstraintNode.h"
class ConstraintsListNode : public BaseNode {
   public:
    explicit ConstraintsListNode(std::vector<ConstraintNode*> _constraintsList)
                                                                                                        : BaseNode(NodeType::CONSTRAINTS_LIST),
                                                                                                          constraintsList(_constraintsList) {
    }

    ~ConstraintsListNode() override {
        for (auto& v : constraintsList) {
            delete v;
        }
    }

    void push(ConstraintNode* var) { constraintsList.emplace_back(var); }

    NodeType getNodeType() override { return BaseNode::getNodeType(); }

    std::vector<ConstraintNode*> getConstraintsList() { return constraintsList; }

   private:
    std::vector<ConstraintNode*> constraintsList;
};

#endif  // SELSQL_CONSTRAINTSLISTNODE_H
