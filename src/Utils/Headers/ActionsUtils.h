//
// Created by sapiest on 02.10.2019.
//

#ifndef SELSQL_ACTIONSUTILS_H
#define SELSQL_ACTIONSUTILS_H
#include <functional>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include "../../Engine/Headers/Cursor.h"
#include "../Structures/Data/Table.h"
#include "../Structures/Error.h"
#include "CommonUtils.h"
#include "ParserUtils.h"

class ActionsUtils {
   public:
    std::array<std::function<Error(const std::string&, const std::string&)>, 3> constraintsCheckers = {checkNotNull,
                                                                                                       checkPrimaryKey,
                                                                                                       checkUnique};

    static Error checkFieldsExist(std::shared_ptr<Table> table, std::vector<std::string> colNames);

    static int isNumbers(const std::string& a, const std::string& b) {
        if (std::isdigit(*a.c_str()) && std::isdigit(*b.c_str()))
            return 1;
        return 0;
    }

    static int isChars(const std::string& a, const std::string& b) {
        if (std::isalpha(*a.c_str()) && std::isalpha(*b.c_str()))
            return 1;
        return 0;
    }

    static int compareEquals(const std::string& a, const std::string& b) {
        if (!isNumbers(a, b)) {
            auto c = a.substr(1, a.length() - 2);
            if (isChars(c, b)) {
                return c == b;
            }
        }
        return std::stod(a) == std::stod(b);
    }

    static int compareNoEquals(const std::string& a, const std::string& b) { return !compareEquals(a, b); }

    static int compareMore(const std::string& a, const std::string& b) {
        if (!isNumbers(a, b)) {
            auto c = a.substr(1, a.length() - 2);
            if (isChars(c, b)) {
                return c > b;
            }
        }
        return std::stod(a) > std::stod(b);
    }

    static int compareMoreEq(const std::string& a, const std::string& b) {
        if (!isNumbers(a, b)) {
            auto c = a.substr(1, a.length() - 2);
            if (isChars(c, b)) {
                return c >= b;
            }
        }
        return std::stod(a) >= std::stod(b);
    }

    static int compareLess(const std::string& a, const std::string& b) {
        if (!isNumbers(a, b)) {
            auto c = a.substr(1, a.length() - 2);
            if (isChars(c, b)) {
                return c < b;
            }
        }
        return std::stod(a) < std::stod(b);
    }

    static int compareLessEq(const std::string& a, const std::string& b) {
        if (!isNumbers(a, b)) {
            auto c = a.substr(1, a.length() - 2);
            if (isChars(c, b)) {
                return c <= b;
            }
        }
        return std::stod(a) <= std::stod(b);
    }

    inline static std::array<std::function<int(const std::string&, const std::string&)>, 6> checkSign = {
                                                                                                        compareMoreEq,
                                                                                                        compareMore,
                                                                                                        compareNoEquals,
                                                                                                        compareEquals,
                                                                                                        compareLess,
                                                                                                        compareLessEq};

    typedef std::vector<std::pair<std::string, std::string>> Record;
    std::string makeRequestCreateFromTable(Table table);

    Error checkConstraint(std::vector<std::string> columns, std::vector<std::string> values,
                          std::pair<std::shared_ptr<Table>, std::shared_ptr<Cursor>> cursor);

    RecordsData checkExpression(std::pair<Expr, vecString> expr, RecordsData records);

    static int checkLogic(std::vector<int> binRes, std::vector<std::string> logicElems);

    static void PrintSelect(std::vector<std::pair<std::string, std::vector<std::string>>> values);

    inline static std::array<std::function<double(double a, double b)>, 4> calculate = {[](double a, double b) {
                                                                                            return a + b;
                                                                                        },
                                                                                        {[](double a, double b) {
                                                                                            return a - b;
                                                                                        }},
                                                                                        {[](double a, double b) {
                                                                                            return a * b;
                                                                                        }},
                                                                                        {[](double a, double b) {
                                                                                            if (b != 0)
                                                                                                return a / b;
                                                                                            else
                                                                                                return 0.0;  // zero
                                                                                                             // division
                                                                                        }}};

   private:
    ParserUtils parserUtils;

    static std::vector<std::pair<Cmp, std::string>> countExpr(std::pair<std::string, std::string> record,
                                                              std::vector<std::pair<std::string, std::string>> row,
                                                              Expr exprs);

    inline static std::map<std::string, std::function<int(int a, int b)>> logicCalculate = {{"and",
                                                                                             [](int a, int b) {
                                                                                                 return a and b;
                                                                                             }},
                                                                                            {"or",
                                                                                             [](int a, int b) {
                                                                                                 return a or b;
                                                                                             }}

    };

    static Error checkNotNull(const std::string& newVal, const std::string& oldVal);

    static Error checkUnique(const std::string& newVal, const std::string& oldVal);

    static Error checkPrimaryKey(const std::string& newVal, const std::string& oldVal);
};

#endif  // SELSQL_ACTIONSUTILS_H
