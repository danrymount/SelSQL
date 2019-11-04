//
// Created by quiks on 17.10.2019.
//

#ifndef SELSQL_DATABLOCK_H
#define SELSQL_DATABLOCK_H
#include <cstring>
#include "../../Headers/Constants.h"
class DataBlock {
   public:
    char* data_;
    short int* deleted_pos_;
    size_t record_amount = 0;
    size_t record_size = 0;
    size_t last_record_pos = 0;
    size_t deleted = 0;
    size_t max_deleted_amount = 0;
    void setDeletedPos(char* pos) { deleted_pos_ = reinterpret_cast<short*>(pos); }
    char* getDeletedPos() { return reinterpret_cast<char*>(deleted_pos_); }
    void setData(char* pos) {
      delete[] data_;
    data_ = pos;
    };
    DataBlock() { data_ = new char[Constants::DATA_SIZE]; };

    ~DataBlock() {
        delete[] data_;
        delete[] deleted_pos_;
    }
};

#endif  // SELSQL_DATABLOCK_H
