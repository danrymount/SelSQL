//
// Created by sapiest on 21.10.2019.
//

#ifndef SELSQL_DELETENODE_H
#define SELSQL_DELETENODE_H
class DeleteNode : public BaseActionNode {
   public:
    DeleteNode(std::string _name, std::vector<BaseNode*> _children)
                                                                                                        : BaseActionNode(Action::DELETE_ACT,
                                                                                                                         std::move(_name),
                                                                                                                         std::move(_children)) {
    }

    explicit DeleteNode(std::string _name) : BaseActionNode(Action::DELETE_ACT, std::move(_name)) {}

    void accept(TreeVisitor* v) override { v->visit(this); }
};
#endif  // SELSQL_DELETENODE_H
