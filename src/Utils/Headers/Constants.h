
#ifndef SELSQL_CONSTANTS_H
#define SELSQL_CONSTANTS_H
#include "CommonUtils.h"
#include "map"
#include "math.h"
#include "string"
class Constants {
   public:
    inline static const std::string TEMP_DIR = "UNCOMMITEDFILES";
    inline static const std::string FLAG_FILE = "FLAG.flag";
    //    inline static const std::string TEMP_FILE = "TEMPORARY.file";
    inline static const std::string TEMP_FILE_TYPE = ".temp";
    inline static const std::string META_FILE_TYPE = ".METAselDB";
    inline static const std::string DATA_FILE_TYPE = ".DATAselDB";
    inline static const std::string LOG_FILE_TYPE = ".log";
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
    static const int MAX_VERSIONS = 100;

    static const int MD_SIZE = MD_TABLE_NAME_SIZE + MD_TABLE_COLUMN_AMOUNT_SIZE +
                                            MD_TABLE_COLUMN_AMOUNT_MAX * (MD_COLUMN_NAME_SIZE + MD_COLUMN_TYPE_SIZE +
                                                                          MD_COLUMN_TYPE_SIZE_2 +
                                                                          MD_COLUMN_CONSTR_AMOUNT_SIZE +
                                                                          MD_COLUMN_CONSTR_AMOUNT_MAX *
                                                                          MD_COLUMN_CONSTR_SIZE);
    static const int MD_HASH_SUM = sizeof(double);
    static const int DATA_SIZE = 1 * 1024;
    static const int DATA_BLOCK_RECORD_AMOUNT = sizeof(int);
    static const int DATA_BLOCK_RECORD_LAST_POS = sizeof(int);
    static const int DATA_BLOCK_DELETED_AMOUNT = sizeof(int);

    inline static std::map<Type, size_t> TYPE_SIZE = {{TYPE_INT, sizeof(int)},
                                                      {TYPE_FLOAT, sizeof(double)},
                                                      {TYPE_CHAR, 256}};
};

#endif  // SELSQL_CONSTANTS_H
