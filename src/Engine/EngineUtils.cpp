//
// Created by quiks on 31.10.2019.
//

#include "Headers/EngineUtils.h"

#include <memory>

void WriteIntToFile(std::fstream *file, int value) { file->write(reinterpret_cast<char *>(&value), sizeof(int)); }

int ReadIntFromFile(std::fstream *file) {
    int res;
    file->read(reinterpret_cast<char *>(&res), sizeof(int));
    return res;
}
buffer_data GetTableBuffer(Table *table) {
    char *data = new char[Constants::MD_SIZE];
    int offset = 0;
    std::string name;
    name.reserve(Constants::MD_COLUMN_NAME_SIZE);
    name = table->name;
    std::memcpy(&data[offset], name.c_str(), Constants::MD_COLUMN_NAME_SIZE);
    offset += Constants::MD_COLUMN_NAME_SIZE;
    int value = table->getFields().size();
    std::memcpy(&data[offset], &value, sizeof(value));
    offset += sizeof(value);
    for (auto &field : table->getFields()) {
        name.clear();
        name = field.first;
        std::memcpy(&data[offset], name.c_str(), Constants::MD_COLUMN_NAME_SIZE);
        offset += Constants::MD_COLUMN_NAME_SIZE;
        value = Type(field.second.type);
        std::memcpy(&data[offset], &value, sizeof(value));
        offset += sizeof(value);
        value = Type(field.second.size);
        std::memcpy(&data[offset], &value, sizeof(value));
        offset += sizeof(value);
        value = Type(field.second.getConstraints().size());
        std::memcpy(&data[offset], &value, sizeof(value));
        offset += sizeof(value);
        for (auto constr : field.second.getConstraints()) {
            value = Constraint(constr);
            std::memcpy(&data[offset], &value, sizeof(value));
            offset += sizeof(value);
        }
    }
    value = table->record_amount;
    std::memcpy(&data[offset], &value, sizeof(value));
    offset += sizeof(value);
    return buffer_data(data, offset);
}
std::shared_ptr<Table> ReadTableFromBuffer(char *data) {
    auto table = std::make_shared<Table>();
    int offset = 0;
    char name[Constants::MD_TABLE_NAME_SIZE] = {};
    std::memcpy(&name, &data[offset], Constants::MD_TABLE_NAME_SIZE);
    table->name = name;
    offset += Constants::MD_TABLE_NAME_SIZE;
    int column_size;
    std::memcpy(&column_size, &data[offset], sizeof(int));
    offset += sizeof(int);
    for (int i = 0; i < column_size; ++i) {
        Variable var;
        char var_name[Constants::MD_COLUMN_NAME_SIZE];
        int type = 0;
        int type_size = 0;
        int constr_size = 0;
        std::vector<Constraint> constraints;
        std::memcpy(var_name, &data[offset], Constants::MD_COLUMN_NAME_SIZE);
        offset += Constants::MD_COLUMN_NAME_SIZE;
        std::memcpy(&type, &data[offset], sizeof(int));
        offset += sizeof(type);
        std::memcpy(&type_size, &data[offset], sizeof(int));
        offset += sizeof(type);
        std::memcpy(&constr_size, &data[offset], sizeof(int));
        offset += sizeof(type);
        var.type = Type(type);
        var.size = type_size;
        for (int j = 0; j < constr_size; ++j) {
            int constr_type = 0;
            std::memcpy(&constr_type, &data[offset], sizeof(int));
            offset += sizeof(type);
            constraints.emplace_back(Constraint(constr_type));
        }
        var.setConstraints(constraints);
        table->addField(std::string(var_name), var);
    }
    table->name = name;
    table->calcRecordSize();
    return table;
}
int GetFileSize(std::fstream *file) {
    if (!file->is_open()) {
        return -1;
    }
    file->seekg(0, std::ios::end);
    int size = file->tellg();
    file->clear();
    file->seekg(std::ios::beg);
    return size;
}
buffer_data GetDataBlockBuffer(DataBlock *data_block) {
    char *data = new char[GetDataBlockSize(data_block)];
    int offset = 0;
    std::memcpy(&data[offset], &data_block->record_amount, sizeof(int));
    offset += sizeof(int);
    std::memcpy(&data[offset], &data_block->last_record_pos, sizeof(int));
    offset += sizeof(int);
    std::memcpy(&data[offset], &data_block->deleted, sizeof(int));
    offset += sizeof(int);
    std::memcpy(&data[offset], data_block->getDeletedPos(), data_block->max_deleted_amount * sizeof(short int));
    offset += data_block->max_deleted_amount * sizeof(short int);
    std::memcpy(&data[offset], data_block->data_, Constants::DATA_SIZE);

    offset += Constants::DATA_SIZE;
    return buffer_data(data, offset);
}
std::shared_ptr<DataBlock> ReadDataBlockFromBuffer(char *data, int record_size) {
    int offset = 0;
    auto dataBlock = std::make_shared<DataBlock>();
    dataBlock->record_size = record_size;
    std::memcpy(&dataBlock->record_amount, &data[offset], sizeof(int));
    offset += sizeof(int);
    std::memcpy(&dataBlock->last_record_pos, &data[offset], sizeof(int));
    offset += sizeof(int);
    std::memcpy(&dataBlock->deleted, &data[offset], sizeof(int));
    offset += sizeof(int);
    dataBlock->max_deleted_amount = Constants::DATA_SIZE / record_size;
    char *deleted = new char[dataBlock->max_deleted_amount * sizeof(short int)];
    std::memcpy(deleted, &data[offset], dataBlock->max_deleted_amount * sizeof(short int));
    offset += dataBlock->max_deleted_amount * sizeof(short int);
    dataBlock->setDeletedPos(deleted);
    char *block_data = new char[Constants::DATA_SIZE];
    std::memcpy(block_data, &data[offset], Constants::DATA_SIZE);
    offset += Constants::DATA_SIZE;
    dataBlock->setData(block_data);

    return dataBlock;
}
int GetDataBlockSize(DataBlock *data_block) {
    return Constants::DATA_BLOCK_DELETED_AMOUNT + Constants::DATA_BLOCK_RECORD_AMOUNT +
           Constants::DATA_BLOCK_RECORD_LAST_POS + data_block->max_deleted_amount * sizeof(short int) +
           Constants::DATA_SIZE;
}
int GetDataBlockSize(int record_size) {
    return Constants::DATA_BLOCK_DELETED_AMOUNT + Constants::DATA_BLOCK_RECORD_AMOUNT +
           Constants::DATA_BLOCK_RECORD_LAST_POS + Constants::DATA_SIZE / record_size * sizeof(short int) +
           Constants::DATA_SIZE;
}
void RestoreFromTemp(std::fstream *src, std::fstream *dist, int record_size) {
    src->flush();

    int rec_amount = 0;
    int offset = 0;
    int size = GetFileSize(src);
    char *buf = new char[size];
    src->seekg(std::ios::beg);
    src->read(buf, size);
    dist->clear();
    dist->seekp(std::ios::beg);
    dist->write(buf, Constants::DATA_BLOCK_RECORD_AMOUNT);
    memcpy(&rec_amount,buf,sizeof(int));

    offset += 4;

    while (offset < size) {
        int block_id = 0;
        memcpy(&block_id, &buf[offset], sizeof(int));
        offset += 4;
        dist->seekp(4 + block_id * GetDataBlockSize(record_size));
        dist->write(&buf[offset], GetDataBlockSize(record_size));
        offset += GetDataBlockSize(record_size);
    }
    if (rec_amount == 0) {
        char *empty = new char[GetFileSize(dist)];
        memset(empty,0,GetFileSize(dist));
        dist->write(empty,GetFileSize(dist));
        delete[] empty;
    }
    dist->flush();
    delete[] buf;
}

void DB_FILE::close() {
    meta_file->close();
    data_file->close();
    delete meta_file;
    delete data_file;
}

int DB_FILE::isOpen() { return meta_file->is_open() and data_file->is_open(); }
DB_FILE::DB_FILE(std::fstream *m_file, std::fstream *d_file) : meta_file(m_file), data_file(d_file) {}
