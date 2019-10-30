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
#include "../Structures/Message.h"
#include "CommonUtils.h"
#include "ParserUtils.h"

class ActionsUtils {
   public:
    typedef std::vector<std::pair<std::string, std::string>> Record;

    std::array<std::function<Message(std::string, std::string)>, 3> constraintsCheckers = {checkNotNull,
                                                                                           checkPrimaryKey,
                                                                                           checkUnique};

    static Message checkFieldsExist(const std::shared_ptr<Table>& table, const Record& updateColumns);

    static int isNumbers(const std::string& a, const std::string& b);
    static int isChars(const std::string& a, const std::string& b);
    static int compareEquals(const std::string& a, const std::string& b);
    static int compareNoEquals(const std::string& a, const std::string& b) { return !compareEquals(a, b); }
    static int compareMore(const std::string& a, const std::string& b);
    static int compareMoreEq(const std::string& a, const std::string& b);
    static int compareLess(const std::string& a, const std::string& b);
    static int compareLessEq(const std::string& a, const std::string& b);

    static std::string makeRequestCreateFromTable(const std::shared_ptr<Table>& table);

    Message checkConstraint(const Record& updateColumns, const std::shared_ptr<Table>& table,
                            const std::vector<ActionsUtils::Record>& records, bool isUpdate = false);
    static std::string
    getSelectMessage(std::vector<std::vector<std::pair<std::pair<std::string, std::string>, std::string>>> values);

    static std::string checkSelectColumns(std::vector<Record> values, const std::vector<std::string>& selectCols);
    static std::string getSelectMessage(std::vector<Record> values);
    static std::string getTableInfo(const std::shared_ptr<Table>& table, int includeCols);
    static std::vector<Record> getAllRecords(const std::pair<std::shared_ptr<Table>, std::shared_ptr<Cursor>>& cursor);

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
    inline static std::array<std::function<int(const std::string&, const std::string&)>, 6> checkSign = {
                                                                                                        compareMoreEq,
                                                                                                        compareMore,
                                                                                                        compareNoEquals,
                                                                                                        compareEquals,
                                                                                                        compareLess,
                                                                                                        compareLessEq};

   private:
    static int checkSameForUpdate(const Record& oldRec, const Record& newRec, const std::shared_ptr<Table>& table);
    static Message checkNotNull(std::string newVal, const std::string& oldVal = "");
    static Message checkUnique(std::string newVal, const std::string& oldVal);
    static Message checkPrimaryKey(const std::string& newVal, const std::string& oldVal);
};

#endif  // SELSQL_ACTIONSUTILS_H
