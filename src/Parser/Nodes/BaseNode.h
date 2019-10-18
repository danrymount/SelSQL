//
// Created by sapiest on 18.10.2019.
//

#ifndef SELSQL_BASENODE_H
#define SELSQL_BASENODE_H
enum class NodeType { ROOT, CREATE, IDENTIFIER, VARIABLE, VARIABLES_LIST };

class BaseNode {
   public:
    explicit BaseNode(NodeType _nodeType) : nodeType(_nodeType) {}

    virtual ~BaseNode() = default;

    virtual NodeType getNodeType() { return nodeType; }

   private:
    NodeType nodeType;
};

#endif  // SELSQL_BASENODE_H
