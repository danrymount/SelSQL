//
// Created by sapiest on 07.10.2019.
//

#pragma once
#ifndef SELSQL_BIGREQUEST_H
#define SELSQL_BIGREQUEST_H

#include "BigResponse.h"
#include "Data/DDLdata.h"
#include "Data/DMLdata.h"
#include "Data/DQLdata.h"
#include "Data/Expresion.h"

class BigRequest {
   public:
    BigRequest() = default;
    BigRequest(Action _action, std::string _tableName, DDLdata _ddlData, DMLdata _dmlData, DQLdata _dqlData,
               std::pair<Expr, vecString> _expression)
                                                                                                        : action(_action),
                                                                                                          tableName(_tableName),
                                                                                                          ddlData(_ddlData),
                                                                                                          dmlData(_dmlData),
                                                                                                          dqlData(_dqlData),
                                                                                                          expression(_expression) {
    }

    Action action;
    std::string tableName;
    DDLdata ddlData;
    DMLdata dmlData;
    DQLdata dqlData;
    std::pair<Expr, vecString> expression;

    void clear() {
        tableName.erase();
        ddlData.clear();
        dmlData.clear();
        dqlData.clear();
    }
};

#endif  // SELSQL_BIGREQUEST_H
