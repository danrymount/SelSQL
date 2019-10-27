
#ifndef SELSQL_CONSTANTS_H
#define SELSQL_CONSTANTS_H
#include "CommonUtils.h"
#include "map"
#include "math.h"
#include "string"
class Constants {
   public:
    inline static const std::string FILE_TYPE = ".selDB";
    static const int MD_TABLE_NAME_SIZE = 256;
    static const int MD_TABLE_COLUMN_AMOUNT_SIZE = sizeof(int);
    static const int MD_TABLE_COLUMN_AMOUNT_MAX = 16;
    static const int MD_COLUMN_NAME_SIZE = 256;
    static const int MD_COLUMN_TYPE_SIZE = sizeof(int);
    static const int MD_COLUMN_TYPE_SIZE_2 = sizeof(int);
    static const int MD_COLUMN_CONSTR_SIZE = sizeof(int);
    static const int MD_COLUMN_CONSTR_AMOUNT_SIZE = sizeof(int);
    static const int MD_COLUMN_CONSTR_AMOUNT_MAX = 10;

    static const int DATA_BLOCK_START_POS = MD_TABLE_NAME_SIZE + MD_TABLE_COLUMN_AMOUNT_SIZE +
                                            MD_TABLE_COLUMN_AMOUNT_MAX * (MD_COLUMN_NAME_SIZE + MD_COLUMN_TYPE_SIZE +
                                                                          MD_COLUMN_TYPE_SIZE_2 +
                                                                          MD_COLUMN_CONSTR_AMOUNT_SIZE +
                                                                          MD_COLUMN_CONSTR_AMOUNT_MAX * MD_COLUMN_CONSTR_SIZE) +
                                            4;
    static const int DATA_SIZE = 1 * 1024;
    static const int DATA_BLOCK_RECORD_AMOUNT = sizeof(int);
    static const int DATA_BLOCK_RECORD_LAST_POS = sizeof(int);
    static const int DATA_BLOCK_DELETED_AMOUNT = sizeof(int);

    inline static std::map<Type, size_t> TYPE_SIZE = {{TYPE_INT, sizeof(int)},
                                                      {TYPE_FLOAT, sizeof(double)},
                                                      {TYPE_CHAR, 256}};
};

#endif  // SELSQL_CONSTANTS_H
