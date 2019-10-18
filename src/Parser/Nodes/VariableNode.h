//
// Created by sapiest on 18.10.2019.
//

#ifndef SELSQL_VARIABLENODE_H
#define SELSQL_VARIABLENODE_H

#include <string>
#include "../../Utils/Headers/CommonUtils.h"
#include "BaseNode.h"

class Variable : public BaseNode {
   public:
    Variable(std::string &_name, Type _type) : BaseNode(NodeType::VARIABLE), name(_name), type(_type) {}

    NodeType getNodeType() override { return BaseNode::getNodeType(); }

   private:
    std::string name;
    Type type;
};

#endif  // SELSQL_VARIABLENODE_H
