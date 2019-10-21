//
// Created by sapiest on 21.10.2019.
//

#ifndef SELSQL_INSERTNODE_H
#define SELSQL_INSERTNODE_H
#include <utility>

#include "../ValuesNodes/BaseValueNode.h"
#include "BaseActionNode.h"
class InsertNode : public BaseActionNode {
   public:
    InsertNode(std::string _name, std::vector<BaseNode *> _children, std::vector<BaseValueNode *> _values)
                                                                                                        : BaseActionNode(Action::INSERT,
                                                                                                                         std::move(_name),
                                                                                                                         std::move(_children)),
                                                                                                          values(std::move(_values)) {
    }

    std::vector<BaseValueNode *> getValues() { return values; }

    void accept(TreeVisitor *v) override { v->visit(this); }

   private:
    std::vector<BaseValueNode *> values;
};

#endif  // SELSQL_INSERTNODE_H
