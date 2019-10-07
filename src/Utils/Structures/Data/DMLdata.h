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

    std::vector<std::string> values;
    // std::map<std::string, std::string> values;

    void clear() override {
        values.clear();
        DQLdata().clear();
    }
};

#endif  // SELSQL_DMLDATA_H
