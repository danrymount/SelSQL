//
// Created by sapiest on 07.10.2019.
//

#ifndef SELSQL_ERRORCONSTANTS_H
#define SELSQL_ERRORCONSTANTS_H
#include <map>
#include <string>
class ErrorConstants {
    inline static const std::string ERR_TABLE_EXISTS_str = "Table already exists";
    inline static const std::string ERR_TABLE_NOT_EXISTS_str = "Table doesn`t exist";
    inline static const std::string ERR_SAME_CONSTRAINT_str = "Constraint already exists";
    inline static const std::string ERR_SAME_FIELD_NAME_str = "Field name already used";
    inline static const std::string ERR_INSERT_VALUES_SIZE_str = "Invalid count of columns and values";

   public:
    inline static const int ERR_TABLE_EXISTS = 1;
    inline static const int ERR_TABLE_NOT_EXISTS = 2;
    inline static const int ERR_SAME_CONSTRAINT = 3;
    inline static const int ERR_SAME_FIELD_NAME = 4;
    inline static const int ERR_INSERT_VALUES_SIZE = 5;

    inline static std::map<int, std::string> mapOfErrors = {{ERR_SAME_CONSTRAINT, ERR_INSERT_VALUES_SIZE_str},
                                                            {ERR_TABLE_EXISTS, ERR_TABLE_EXISTS_str},
                                                            {ERR_SAME_CONSTRAINT, ERR_SAME_CONSTRAINT_str},
                                                            {ERR_SAME_FIELD_NAME, ERR_SAME_FIELD_NAME_str},
                                                            {ERR_INSERT_VALUES_SIZE, ERR_INSERT_VALUES_SIZE_str}};
};

#endif  // SELSQL_ERRORCONSTANTS_H
