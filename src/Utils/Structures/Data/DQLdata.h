//
// Created by sapiest on 07.10.2019.
//

#pragma once
#ifndef SELSQL_DQLDATA_H
#define SELSQL_DQLDATA_

class DQLdata {
   public:
    DQLdata() = default;
    std::vector<std::string> columns;

    virtual void clear() { columns.clear(); }
};

#endif  // SELSQL_DQLDATA_H
