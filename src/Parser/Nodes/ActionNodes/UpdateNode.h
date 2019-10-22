//
// Created by sapiest on 21.10.2019.
//

#ifndef SELSQL_UPDATENODE_H
#define SELSQL_UPDATENODE_H
class UpdateNode : public BaseActionNode {
   public:
    UpdateNode(std::string _name, std::vector<BaseNode*> _children)
                                                                                                        : BaseActionNode(Action::UPDATE,
                                                                                                                         std::move(_name),
                                                                                                                         std::move(_children)) {
    }

    explicit UpdateNode(std::string _name) : BaseActionNode(Action::UPDATE, std::move(_name)) {}

    void accept(TreeVisitor* v) override { v->visit(this); }
};

#endif  // SELSQL_UPDATENODE_H
