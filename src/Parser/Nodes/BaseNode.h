//
// Created by sapiest on 18.10.2019.
//

#ifndef SELSQL_BASENODE_H
#define SELSQL_BASENODE_H

#include "../Headers/TreeVisitor.h"

enum class NodeType {
    CREATE,
    DROP,
    SHOWCREATE,
    INSERT,
    SELECT,
    UPDATE,
    DELETE,
    ROOT,
    IDENTIFIER,
    VARIABLE,
    VARIABLES_LIST,
    CONSTRAINT
};

class BaseNode {
   public:
    explicit BaseNode(NodeType _nodeType) : nodeType(_nodeType) {}

    virtual ~BaseNode() = default;

    virtual NodeType getNodeType() { return nodeType; }

    virtual void accept(TreeVisitor v) = 0;

   private:
    NodeType nodeType;
};

#endif  // SELSQL_BASENODE_H
