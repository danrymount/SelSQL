
#ifndef SELSQL_APIENGINE_H
#define SELSQL_APIENGINE_H

#include "../../Utils/Headers/CommonUtils.h"
class APIEngine {
   public:
    virtual int CreateTable(Table* new_table) = 0;
    virtual Table* ShowCreateTable(std::string table_name) = 0;
    virtual int DropTable(std::string table_name) = 0;
};
#endif  // SELSQL_APIENGINE_H
