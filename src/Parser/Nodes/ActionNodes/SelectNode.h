//
// Created by sapiest on 21.10.2019.
//

#ifndef SELSQL_SELECTNODE_H
#define SELSQL_SELECTNODE_H
class SelectNode : public BaseActionNode {
   public:
    SelectNode(std::string _name, std::vector<BaseNode*> _children)
                                                                                                        : BaseActionNode(Action::SELECT,
                                                                                                                         std::move(_name),
                                                                                                                         std::move(_children)) {
    }

    void accept(TreeVisitor* v) override { v->visit(this); }
};

#endif  // SELSQL_SELECTNODE_H
