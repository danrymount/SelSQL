//
// Created by sapiest on 21.10.2019.
//

#ifndef SELSQL_NULLVALUENODE_H
#define SELSQL_NULLVALUENODE_H
#include "BaseValueNode.h"
class NullValueNode : public BaseValueNode {
   public:
    explicit NullValueNode(std::string& _value) : value(_value) {
        std::transform(value.begin(), value.end(), value.begin(), [](unsigned char c) { return std::tolower(c); });
    }

    std::string getValue() { return value; };

    void accept(TreeVisitor* v) override { v->visit(this); }

   private:
    std::string value;
};

#endif  // SELSQL_NULLVALUENODE_H
