//
// Created by sapiest on 07.10.2019.
//

#pragma once
#ifndef SELSQL_DMLDATA_H
#define SELSQL_DMLDATA_H

#include "DQLdata.h"

class DMLdata : public DQLdata {
   public:
    DMLdata() = default;

    DMLdata(std::vector<std::string> &_columns, std::vector<std::string> &_values,
            std::map<std::string, Condition> &_conditions)
                                                                                                        : DQLdata(_columns,
                                                                                                                  _conditions),
                                                                                                          values(_values) {
    }

    std::vector<std::string> values;
    // std::map<std::string, std::string> values;

    void clear() override {
        values.clear();
        DQLdata().clear();
    }
};

#endif  // SELSQL_DMLDATA_H
