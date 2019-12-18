//
// Created by sapiest on 18.12.2019.
//

#ifndef SELSQL_SYSTEMTIMENODE_H
#define SELSQL_SYSTEMTIMENODE_H

#include <utility>

#include "BaseNode.h"
class SystemTimeNode : public BaseNode {
   public:
    SystemTimeNode(std::string a, std::string b) : periodA(a), periodB(b) {}

    void accept(TreeVisitor* v) override { v->visit(this); }

    std::string getPeriodA() { return periodA; }
    std::string getPeriodB() { return periodB; }

   private:
    std::string periodA;
    std::string periodB;
};

#endif  // SELSQL_SYSTEMTIMENODE_H
