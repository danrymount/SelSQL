//
// Created by sapiest on 07.10.2019.
//

#ifndef SELSQL_MESSAGE_H
#define SELSQL_MESSAGE_H
#include <string>
#include <utility>
#include "../Headers/ErrorConstants.h"
class Message {
   public:
    Message() = default;

    explicit Message(int _errorCode) {
        errorCode_ = _errorCode;
        msg_ = ErrorConstants::mapOfErrors[_errorCode];
    }
    explicit Message(std::string message) { msg_ = std::move(message); }

    int getErrorCode() { return errorCode_; }

    std::string getMsg() { return msg_; }

   private:
    int errorCode_ = 0;
    std::string msg_;
};

#endif  // SELSQL_MESSAGE_H
