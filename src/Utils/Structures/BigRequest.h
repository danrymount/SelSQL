//
// Created by sapiest on 07.10.2019.
//

#ifndef SELSQL_BIGREQUEST_H
#define SELSQL_BIGREQUEST_H

#include "BigResponse.h"
#include "Data/DDLdata.h"
#include "Data/DMLdata.h"
#include "Data/DQLdata.h"
#include "Error.h"
class BigRequest {
   public:
    BigRequest() = default;
    BigRequest(Action _action, std::string &_tableName, DDLdata *_ddlData, DMLdata *_dmlData, DQLdata *_dqlData)
                                                                                                        : action(_action),
                                                                                                          tableName(_tableName),
                                                                                                          ddlData(_ddlData),
    {}

    Action action;
    std::string tableName;
    DDLdata *ddlData;
    DMLdata *dmlData;
    DQLdata *dqlData;

    void clear() {
        tableName.erase();
        ddlData.clear() dmlData.clear() dqlData.clear()
    }
};

#endif  // SELSQL_BIGREQUEST_H
