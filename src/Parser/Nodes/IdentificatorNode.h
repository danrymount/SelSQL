//
// Created by sapiest on 18.10.2019.
//

#ifndef SELSQL_IDENTIFICATORNODE_H
#define SELSQL_IDENTIFICATORNODE_H
#include <string>
#include <utility>
#include "BaseNode.h"
class IdentificatorNode : public BaseNode {
   public:
    explicit IdentificatorNode(std::string _name) : BaseNode(NodeType::IDENTIFIER), name(std::move(_name)) {}

    NodeType getNodeType() override { return BaseNode::getNodeType(); }

    std::string getName() { return name; }

   private:
    std::string name;
};

#endif  // SELSQL_IDENTIFICATORNODE_H
