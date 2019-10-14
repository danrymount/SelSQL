//
// Created by sapiest on 02.10.2019.
//

#ifndef SELSQL_ACTIONSUTILS_H
#define SELSQL_ACTIONSUTILS_H
#include <string>
#include "../../Engine/Headers/Cursor.h"
#include "../Structures/Data/Table.h"
#include "../Structures/Error.h"
#include "CommonUtils.h"
#include "ParserUtils.h"

class ActionsUtils {
   public:
    //    int checkNotNull(std::string newVal, std::string oldVal, std::unique_ptr<Error> error) {
    //        if (newVal == "null") {
    //            *error = Error(ErrorConstants::ERR_NOT_NULL);
    //            return 1;
    //        }
    //        return 0;
    //    }

    // std::vector<std::function<int(std::string, std::string, std::unique_ptr<Error> error)>> constraintsCheckers =
    // (checkNotNull);

    std::vector<std::function<Error(const std::string&, const std::string&)>> constraintsCheckers = {checkNotNull,
                                                                                                     checkPrimaryKey,
                                                                                                     checkUnique};

    typedef std::vector<std::pair<std::string, std::string>> Record;
    std::string makeRequestCreateFromTable(Table table);

    Error checkConstraint(std::vector<std::string> columns, std::vector<std::string> values,
                          std::pair<std::shared_ptr<Table>, std::shared_ptr<Cursor>> cursor);

    Record getTableRecord(std::pair<std::shared_ptr<Table>, std::shared_ptr<Cursor>> cursor);

   private:
    ParserUtils parserUtils;

    static Error checkNotNull(const std::string& newVal, const std::string& oldVal);

    static Error checkUnique(const std::string& newVal, const std::string& oldVal);

    static Error checkPrimaryKey(const std::string& newVal, const std::string& oldVal);
};

#endif  // SELSQL_ACTIONSUTILS_H
