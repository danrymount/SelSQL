//
// Created by quiks on 07.12.2019.
//

#ifndef SELSQL_RECORD_H
#define SELSQL_RECORD_H
#include <cstring>
struct Record {
    char *values_buf;
    long tr_s = 0;
    long tr_e = 0;
    char commited = 0;
    int record_size = 0;

    explicit Record(int rec_size) : record_size(rec_size) { values_buf = new char[record_size]; };

    int GetRecordSize() { return record_size + sizeof(tr_s) + sizeof(tr_e); };
    char *GetRecordBuf() {
        char *result = new char[GetRecordSize()];
        int offset = 0;
        std::memcpy(&result[offset], values_buf, record_size);
        offset += record_size;
        std::memcpy(&result[offset], &tr_s, sizeof(tr_s));
        offset += sizeof(tr_s);
        std::memcpy(&result[offset], &tr_e, sizeof(tr_e));
        offset += sizeof(tr_e);
        return result;
    };

    void SetRecord(char *buf) {
        int offset = 0;
        std::memcpy(values_buf, &buf[offset], record_size);
        offset += record_size;
        std::memcpy(&tr_s, &buf[offset], sizeof(tr_s));
        offset += sizeof(tr_s);
        std::memcpy(&tr_e, &buf[offset], sizeof(tr_e));
        offset += sizeof(tr_e);
    };
    void SetValues(char *buf) { std::memcpy(values_buf, buf, record_size); };

    ~Record() { delete[] values_buf; }
};
#endif  // SELSQL_RECORD_H
