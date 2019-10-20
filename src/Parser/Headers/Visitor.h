//
// Created by sapiest on 20.10.2019.
//

#ifndef SELSQL_VISITOR_H
#define SELSQL_VISITOR_H

class RootNode;
class CreateNode;
class DropNode;
class ShowCreateNode;
class ConstraintNode;
class VariableNode;
class BaseNode;
class BaseActionNode;

class Visitor {
   public:
    virtual void visit(RootNode* node) = 0;

    virtual void visit(CreateNode* node) = 0;
    virtual void visit(DropNode* node) = 0;
    virtual void visit(ShowCreateNode* node) = 0;

    virtual void visit(ConstraintNode* node) = 0;
    virtual void visit(VariableNode* node) = 0;
};
#endif  // SELSQL_VISITOR_H
