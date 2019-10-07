//
// Created by sapiest on 07.10.2019.
//

#ifndef SELSQL_BIGRESPONSE_H
#define SELSQL_BIGRESPONSE_H

#include <string>
#include "BigRequest.h"
#include "Data/DDLdata.h"
#include "Data/DMLdata.h"
#include "Data/DQLdata.h"
#include "Error.h"
class BigResponse : public BigRequest {
    BigResponse() = default;

    // BigResponse(Action _action, Error _error) : BigRequest(_action, ), error(_error) {}

   public:
    Error error;
};

#endif  // SELSQL_BIGRESPONSE_H
