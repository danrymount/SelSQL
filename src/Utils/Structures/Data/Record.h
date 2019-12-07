//
// Created by quiks on 07.12.2019.
//

#ifndef SELSQL_RECORD_H
#define SELSQL_RECORD_H
#include <cstring>
struct Record {
    int index = 0;
    char *record_buf;
    long tr_s = 0;
    long tr_e = 0;
    int record_size = 0;

    explicit Record(int rec_size) : record_size(rec_size) { record_buf = new char[record_size]; };

    int GetRecordFullSize() { return sizeof(index) + record_size + sizeof(tr_s) + sizeof(tr_e); };
    char *GetRecordBuf() {
        char *result = new char[GetRecordFullSize()];
        int offset = 0;
        std::memcpy(&result[offset], &index, sizeof(index));
        offset += sizeof(index);
        std::memcpy(&result[offset], record_buf, record_size);
        offset += record_size;
        std::memcpy(&result[offset], &tr_s, sizeof(tr_s));
        offset += sizeof(tr_s);
        std::memcpy(&result[offset], &tr_e, sizeof(tr_e));
        offset += sizeof(tr_e);
        return result;
    };

    void SetAllRecordBuf(char *buf) {
        int offset = 0;
        std::memcpy(&index, &buf[offset], sizeof(index));
        offset += sizeof(index);
        std::memcpy(record_buf, &buf[offset], record_size);
        offset += record_size;
        std::memcpy(&tr_s, &buf[offset], sizeof(tr_s));
        offset += sizeof(tr_s);
        std::memcpy(&tr_e, &buf[offset], sizeof(tr_e));
        offset += sizeof(tr_e);
    };
    void SetOnlyRecordBuf(char *buf) { std::memcpy(record_buf, buf, record_size); };
    ~Record() { delete[] record_buf; }
};
#endif  // SELSQL_RECORD_H
