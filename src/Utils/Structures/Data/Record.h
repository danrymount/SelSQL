//
// Created by quiks on 07.12.2019.
//

#ifndef SELSQL_RECORD_H
#define SELSQL_RECORD_H
#include <cstring>
// TODO CHANGE LONG TO long long or int64_t
// TODO ADD COMMITED FLAG
// TODO ADD TRANSACTION CALENDAR
struct Record {
    char *values_buf;
    int64_t tr_s = 0;
    int64_t tr_e = 0;
    char commited_tr_s = '0';
    char commited_tr_e = '0';
    int record_size = 0;

    explicit Record(int rec_size) : record_size(rec_size) { values_buf = new char[record_size]; };

    int GetRecordSize() {
        return record_size + sizeof(tr_s) + sizeof(tr_e) + sizeof(commited_tr_s) + sizeof(commited_tr_e);
    };
    char *GetRecordBuf() {
        char *result = new char[GetRecordSize()];
        int offset = 0;
        std::memcpy(&result[offset], values_buf, record_size);
        offset += record_size;
        std::memcpy(&result[offset], &tr_s, sizeof(tr_s));
        offset += sizeof(tr_s);
        std::memcpy(&result[offset], &tr_e, sizeof(tr_e));
        offset += sizeof(tr_e);
        std::memcpy(&result[offset], &commited_tr_s, sizeof(commited_tr_s));
        offset += sizeof(commited_tr_s);
        std::memcpy(&result[offset], &commited_tr_e, sizeof(commited_tr_e));
        offset += sizeof(commited_tr_e);
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
        std::memcpy(&commited_tr_s, &buf[offset], sizeof(commited_tr_s));
        offset += sizeof(commited_tr_s);
        std::memcpy(&commited_tr_e, &buf[offset], sizeof(commited_tr_e));
    };
    void SetValues(char *buf) { std::memcpy(values_buf, buf, record_size); };

    ~Record() { delete[] values_buf; }
};
#endif  // SELSQL_RECORD_H
