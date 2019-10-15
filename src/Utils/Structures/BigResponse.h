//
// Created by sapiest on 07.10.2019.
//

#pragma once
#ifndef SELSQL_BIGRESPONSE_H
#define SELSQL_BIGRESPONSE_H

#include <string>
#include "BigRequest.h"
#include "Data/DDLdata.h"
#include "Data/DMLdata.h"
#include "Data/DQLdata.h"
#include "Data/Expresion.h"
#include "Error.h"

class BigResponse : public BigRequest {
   public:
    BigResponse() = default;

    BigResponse(Action _action, std::string _tableName, DDLdata _ddlData, DMLdata _dmlData, DQLdata _dqlData,
                Error _error,  std::pair<Expr, vecString> _expression)
                                                                                                        : BigRequest(_action,
                                                                                                                     _tableName,
                                                                                                                     _ddlData,
                                                                                                                     _dmlData,
                                                                                                                     _dqlData,
                                                                                                                     _expression),
                                                                                                          error(_error) {
    }

    Error error;
};

#endif  // SELSQL_BIGRESPONSE_H
