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
class IntValueNode;
class CharValueNode;
class FloatValueNode;
class NullValueNode;
class ColumnNode;

class Visitor {
   public:
    virtual void visit(RootNode* node) = 0;

    virtual void visit(CreateNode* node) = 0;
    virtual void visit(DropNode* node) = 0;
    virtual void visit(ShowCreateNode* node) = 0;

    virtual void visit(ConstraintNode* node) = 0;
    virtual void visit(VariableNode* node) = 0;

    virtual void visit(IntValueNode* node) = 0;
    virtual void visit(CharValueNode* node) = 0;
    virtual void visit(FloatValueNode* node) = 0;
    virtual void visit(NullValueNode* node) = 0;

    virtual void visit(ColumnNode* node) = 0;
};
#endif  // SELSQL_VISITOR_H
