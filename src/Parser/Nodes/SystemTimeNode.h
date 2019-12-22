//
// Created by sapiest on 18.12.2019.
//

#ifndef SELSQL_SYSTEMTIMENODE_H
#define SELSQL_SYSTEMTIMENODE_H

#include <utility>

#include "BaseNode.h"
class SystemTimeNode : public BaseNode {
   public:
    SystemTimeNode(const std::string& a, const std::string& b)
                                                                                                        : periodA(stringToChrono(a)),
                                                                                                          periodB(stringToChrono(b)) {
    }

    void accept(TreeVisitor* v) override { v->visit(this); }

    int64_t getPeriodA() { return periodA; }
    int64_t getPeriodB() { return periodB; }

   private:
    static int64_t stringToChrono(const std::string& date) {
        std::tm tm{};
        strptime(date.c_str(), "%d-%m-%Y %H:%M:%S", &tm);
        int64_t start_time = 0;
        auto s_time = std::chrono::system_clock::from_time_t(std::mktime(&tm));
        std::memcpy(&start_time, &s_time, sizeof(s_time));
        return start_time;
    }

    int64_t periodA;
    int64_t periodB;
};

#endif  // SELSQL_SYSTEMTIMENODE_H
