//
// Created by sapiest on 07.10.2019.
//

#ifndef SELSQL_ERROR_H
#define SELSQL_ERROR_H
#include <string>
class Error {
   public:
    int errorCode = 0;
    std::string errorMsg;
};

#endif  // SELSQL_ERROR_H
