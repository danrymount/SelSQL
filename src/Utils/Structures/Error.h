//
// Created by sapiest on 07.10.2019.
//

#ifndef SELSQL_ERROR_H
#define SELSQL_ERROR_H
#include <string>
#include "../Headers/ErrorConstants.h"
class Error {
   public:
    Error() = default;

    Error(int _code) {
        errorCode = _code;
        errorMsg = ErrorConstants::mapOfErrors[_code];
    }

    int getErrorCode() { return errorCode; }

    std::string getErrorMsg() { return errorMsg; }

   private:
    int errorCode = 0;
    std::string errorMsg;
};

#endif  // SELSQL_ERROR_H
