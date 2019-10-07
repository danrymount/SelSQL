//
// Created by toxaxab on 30.09.2019.
//

#ifndef SELSQL_COMMONUTILS_H
#define SELSQL_COMMONUTILS_H

#include <map>
#include <string>
#include <vector>
enum Type { INT, FLOAT, BOOLEAN, CHAR };

enum Constraint { NOT_NULL, PRIMARY_KEY, UNIQUE };

enum Action { SHOW_CREATE, DROP, CREATE, SELECT, DELETE, INSERT, UPDATE };

// struct Response {
//    Response() = default;
//    Response(Action _action, Table _table, int _code, std::string _errorMsg, std::string _returnMsg)
//                                                                                                        :
//                                                                                                        action(_action),
//                                                                                                          table(_table),
//                                                                                                          code(_code),
//                                                                                                          errorMsg(_errorMsg),
//                                                                                                          returnMsg(_returnMsg)
//                                                                                                          {
//    }
//
//    Action action;
//    Table table;
//    int code = 0;
//    std::string errorMsg;
//    std::string returnMsg;
//
//    void clear() {
//        this->code = 0;
//        this->errorMsg.erase();
//        this->returnMsg.erase();
//        this->table.clear();
//    }
//};

#endif  // SELSQL_COMMONUTILS_H
