//
// Created by sapiest on 07.10.2019.
//

#pragma once
#ifndef SELSQL_DQLDATA_H
#define SELSQL_DQLDATA_

class DQLdata {
   public:
    DQLdata() = default;
    DQLdata(std::vector<std::string> _columns, std::map<std::string, Condition> _conditions)
                                                                                                        : columns(_columns),
                                                                                                          conditions(_conditions) {
    }

    std::vector<std::string> columns;
    std::map<std::string, Condition> conditions;

    virtual void clear() {
        columns.clear();
        conditions.clear();
    }
};

#endif  // SELSQL_DQLDATA_H