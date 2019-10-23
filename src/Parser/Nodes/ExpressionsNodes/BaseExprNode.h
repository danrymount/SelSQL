//
// Created by sapiest on 22.10.2019.
//

#ifndef SELSQL_BASEEXPRNODE_H
#define SELSQL_BASEEXPRNODE_H
#include "../BaseNode.h"
class BaseExprNode : public BaseNode {
   public:
    BaseExprNode() = default;

    virtual std::string getBaseValue() { return std::string(); }

    int getResult() { return result; }

    void setResult(int _res) { result = _res; }

   private:
    int result = -1;
};
#endif  // SELSQL_BASEEXPRNODE_H
