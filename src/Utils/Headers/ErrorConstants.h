//
// Created by sapiest on 07.10.2019.
//

#ifndef SELSQL_ERRORCONSTANTS_H
#define SELSQL_ERRORCONSTANTS_H
#include <map>
#include <string>
class ErrorConstants {
   public:
    inline static const std::string ERR_TABLE_EXISTS_str = "Table already exists";
    inline static const std::string ERR_TABLE_NOT_EXISTS_str = "Table doesn`t exist";
    inline static const std::string ERR_SAME_CONSTRAINT_str = "Constraint already exists";
    inline static const std::string ERR_SAME_FIELD_NAME_str = "Field name already used";
    inline static const std::string ERR_INSERT_VALUES_SIZE_str = "Invalid count of columns and values";
    inline static const std::string ERR_NO_SUCH_FIELD_str = "Field doesnt exist";
    inline static const std::string ERR_SAME_COLUMN_str = "Same column in request";
    inline static const std::string ERR_NOT_NULL_str = "Null values unavailable";
    inline static const std::string ERR_UNIQUE_str = "Value already exists, use UNIQUE value";
    inline static const std::string ERR_TABLE_FULL_str = "TABLE IS FULL";
    inline static const std::string ERR_BIG_CHAR_str = "CHAR IS VERY BIG";

    inline static const int ERR_TABLE_EXISTS = 1;
    inline static const int ERR_TABLE_NOT_EXISTS = 2;
    inline static const int ERR_SAME_CONSTRAINT = 3;
    inline static const int ERR_SAME_FIELD_NAME = 4;
    inline static const int ERR_INSERT_VALUES_SIZE = 5;
    inline static const int ERR_NO_SUCH_FIELD = 6;
    inline static const int ERR_SAME_COLUMN = 7;
    inline static const int ERR_NOT_NULL = 8;
    inline static const int ERR_UNIQUE = 9;
    inline static const int ERR_TABLE_FULL = 10;
    inline static const int ERR_TYPE_MISMATCH = 11;
    inline static const int ERR_STO = 12;
    inline static const int ERR_BIG_CHAR = 13;
    inline static const int ERR_TRANSACT_CONFLICT = 14;

    inline static std::map<int, std::string> mapOfErrors = {{ERR_TABLE_NOT_EXISTS, ERR_TABLE_NOT_EXISTS_str},
                                                            {ERR_TABLE_EXISTS, ERR_TABLE_EXISTS_str},
                                                            {ERR_SAME_CONSTRAINT, ERR_SAME_CONSTRAINT_str},
                                                            {ERR_SAME_FIELD_NAME, ERR_SAME_FIELD_NAME_str},
                                                            {ERR_INSERT_VALUES_SIZE, ERR_INSERT_VALUES_SIZE_str},
                                                            {ERR_NO_SUCH_FIELD, ERR_NO_SUCH_FIELD_str},
                                                            {ERR_SAME_COLUMN, ERR_SAME_COLUMN_str},
                                                            {ERR_NOT_NULL, ERR_NOT_NULL_str},
                                                            {ERR_UNIQUE, ERR_UNIQUE_str},
                                                            {ERR_TABLE_FULL, ERR_TABLE_FULL_str},
                                                            {ERR_TYPE_MISMATCH, "Types mismatch"},
                                                            {ERR_STO, "Value is out of range OR is not a number"},
                                                            {ERR_BIG_CHAR, ERR_BIG_CHAR_str}};
};

#endif  // SELSQL_ERRORCONSTANTS_H
