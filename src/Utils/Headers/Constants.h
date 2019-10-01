
#ifndef SELSQL_CONSTANTS_H
#define SELSQL_CONSTANTS_H
#include "math.h"

class Constants {
   public:
    inline static const std::string FILE_TYPE = ".selDB";
    static const int META_BLOCK_TABLE_NAME_SIZE = 256;
    static const int META_BLOCK_TABLE_COLUMN_AMOUNT_SIZE = sizeof(int);
    static const int META_BLOCK_COLUMN_NAME_SIZE = 256;
    static const int META_BLOCK_COLUMN_TYPE_SIZE = sizeof(int);
    static const int META_BLOCK_COLUMN_CONSTR_SIZE = sizeof(int);
    static const int META_BLOCK_COLUMN_CONSTR_AMOUNT_SIZE = 10;
    static const int DATA_PAGE_SIZE = 4 * 1024;
};

#endif  // SELSQL_CONSTANTS_H
