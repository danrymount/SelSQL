//
// Created by quiks on 17.10.2019.
//

#ifndef SELSQL_DATABLOCK_H
#define SELSQL_DATABLOCK_H
#include <cstring>
#include "../../Headers/Constants.h"
class DataBlock {
   public:
    int was_changed = 0;
    char* data_;
    void setData(char* pos) { data_ = pos; };
    DataBlock(){};

    ~DataBlock() { delete[] data_; }
};

#endif  // SELSQL_DATABLOCK_H
