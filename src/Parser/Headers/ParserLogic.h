//
// Created by sapiest on 05.10.2019.
//

#ifndef SELSQL_PARSERLOGIC_H
#define SELSQL_PARSERLOGIC_H

#include <string>
using std::string;

#include "../../Engine/Headers/MainEngine.h"
#include "../../Utils/Headers/ParserUtils.h"
#include "../../Utils/Structures/BigResponse.h"

class ParserLogic {
    MainEngine engineApi;
    ParserUtils parserUtils;
    BigResponse response;
    std::map<std::string, int> checkName;

   public:
    void addColumn(char* name, char* type);
    void addConstraint(char* name);
    BigResponse finish();
    void addTableName(char* name);
    void addActionName(char* name);
    void addSelectColumn(char* name);
    void addCondition(char* name, char* sign, char* value);
    void addValue(char* value);
    void addColumn(char* anme);
};

#endif  // SELSQL_PARSERLOGIC_H
