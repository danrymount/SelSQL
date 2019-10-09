//
// Created by sapiest on 07.10.2019.
//

#pragma once
#ifndef SELSQL_DDLDATA_H
#define SELSQL_DDLDATA_H
#include "../../Headers/CommonUtils.h"
#include "Table.h"
class DDLdata {
   public:
    DDLdata() = default;
    DDLdata(Table _table, std::string _returnMsg) : table(_table), returnMsg(_returnMsg) {}

    Table table;
    std::string returnMsg;

    void clear() {
        table.clear();
        returnMsg.erase();
    }
};

#endif  // SELSQL_DDLDATA_H
