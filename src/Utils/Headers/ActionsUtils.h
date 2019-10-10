//
// Created by sapiest on 02.10.2019.
//

#ifndef SELSQL_ACTIONSUTILS_H
#define SELSQL_ACTIONSUTILS_H
#include <string>
#include "../Structures/Data/Table.h"
#include "CommonUtils.h"
#include "ParserUtils.h"

class ActionsUtils {
    ParserUtils parserUtils;

   public:
    std::string makeRequestCreateFromTable(Table table);
};

#endif  // SELSQL_ACTIONSUTILS_H
