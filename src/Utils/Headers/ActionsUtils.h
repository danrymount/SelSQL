//
// Created by sapiest on 02.10.2019.
//

#ifndef SELSQL_ACTIONSUTILS_H
#define SELSQL_ACTIONSUTILS_H
#include <functional>
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

    typedef std::vector<std::pair<std::string, std::string>> Record;
    std::string makeRequestCreateFromTable(Table table);

    Error checkConstraint(std::vector<std::string> columns, std::vector<std::string> values,
                          std::pair<std::shared_ptr<Table>, std::shared_ptr<Cursor>> cursor);

    static RecordsData checkExpression(std::pair<Expr, vecString> expr, RecordsData records);

    Record getTableRecord(std::pair<std::shared_ptr<Table>, std::shared_ptr<Cursor>> cursor);
    static int checkSign(std::string rec_val, Condition cond_val);

   private:
    ParserUtils parserUtils;

    static std::vector<std::pair<Cmp, std::string>> countExpr(std::string columnName, std::string val, Expr exprs);

    inline static std::map<std::string, std::function<double(double a, double b)>> calculate = {{"+",
                                                                                                 [](double a,
                                                                                                    double b) {
                                                                                                     return a + b;
                                                                                           }},
                                                                                          {"-",
                                                                                                 [](double a,
                                                                                                    double b) {
                                                                                                     return a - b;
                                                                                           }},
                                                                                          {"*",
                                                                                                 [](double a,
                                                                                                    double b) {
                                                                                                     return a * b;
                                                                                           }},
                                                                                                {"/", [](double a,
                                                                                                         double b) {
                                                                                                     if (b != 0)
                                                                                                   return a / b;
                                                                                               else
                                                                                                         return 0.0;  // zero
                                                                                                                      // division
                                                                                           }}};

    static Error checkNotNull(const std::string& newVal, const std::string& oldVal);

    static Error checkUnique(const std::string& newVal, const std::string& oldVal);

    static Error checkPrimaryKey(const std::string& newVal, const std::string& oldVal);
};

#endif  // SELSQL_ACTIONSUTILS_H
