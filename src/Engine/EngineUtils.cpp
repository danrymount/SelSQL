//
// Created by quiks on 31.10.2019.
//

#include "Headers/EngineUtils.h"

#include <memory>
char NAME_SEPARATOR = '_';
void WriteIntToFile(std::fstream *file, int value) { file->write(reinterpret_cast<char *>(&value), sizeof(int)); }

int ReadIntFromFile(std::fstream *file) {
    int res;
    file->read(reinterpret_cast<char *>(&res), sizeof(int));
    return res;
}
buffer_data GetTableBuffer(Table *table) {
    char *data = new char[C::MD_SIZE];
    int offset = 0;
    std::string name;
    name.reserve(C::MD_COLUMN_NAME_SIZE);
    name = table->name;
    std::memcpy(&data[offset], name.c_str(), C::MD_COLUMN_NAME_SIZE);
    offset += C::MD_COLUMN_NAME_SIZE;
    int value = table->getFields().size();
    std::memcpy(&data[offset], &value, sizeof(value));
    offset += sizeof(value);
    for (auto &field : table->getFields()) {
        name.clear();
        name = field.first;
        std::memcpy(&data[offset], name.c_str(), C::MD_COLUMN_NAME_SIZE);
        offset += C::MD_COLUMN_NAME_SIZE;
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
    //    value = table->record_amount;
    //    std::memcpy(&data[offset], &value, sizeof(value));
    //    offset += sizeof(value);
    return buffer_data(data, offset);
}
std::shared_ptr<Table> ReadTableFromBuffer(char *data) {
    auto table = std::make_shared<Table>();
    int offset = 0;
    char name[C::MD_TABLE_NAME_SIZE] = {};
    std::memcpy(&name, &data[offset], C::MD_TABLE_NAME_SIZE);
    table->name = name;
    offset += C::MD_TABLE_NAME_SIZE;
    int column_size;
    std::memcpy(&column_size, &data[offset], sizeof(int));
    offset += sizeof(int);
    for (int i = 0; i < column_size; ++i) {
        Variable var;
        char var_name[C::MD_COLUMN_NAME_SIZE];
        int type = 0;
        int type_size = 0;
        int constr_size = 0;
        std::vector<Constraint> constraints;
        std::memcpy(var_name, &data[offset], C::MD_COLUMN_NAME_SIZE);
        offset += C::MD_COLUMN_NAME_SIZE;
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
    file->flush();
    if (file == nullptr or !file->is_open()) {
        return -1;
    }
    file->seekg(0, std::ios::end);
    int size = file->tellg();
    file->clear();
    file->seekg(std::ios::beg);
    return size;
}
buffer_data GetDataBlockBuffer(DataBlock *data_block) {
    char *data = new char[C::DATA_BLOCK_SIZE];
    int offset = 0;
    std::memcpy(&data[offset], data_block->data_, C::DATA_BLOCK_SIZE);

    offset += C::DATA_BLOCK_SIZE;
    return buffer_data(data, offset);
}
std::shared_ptr<DataBlock> ReadDataBlockFromBuffer(char *data) {
    int offset = 0;
    auto dataBlock = std::make_shared<DataBlock>();
    char *block_data = new char[C::DATA_BLOCK_SIZE];
    std::memcpy(block_data, &data[offset], C::DATA_BLOCK_SIZE);
    offset += C::DATA_BLOCK_SIZE;
    dataBlock->setData(block_data);

    return dataBlock;
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
    //    dist->write(buf, C::DATA_BLOCK_RECORD_AMOUNT);
    //    memcpy(&rec_amount,buf,sizeof(int));
    //
    //    offset += 4;
    dist->write(buf, size);

    //        while (offset < size) {
    //            int block_id = 0;
    //            memcpy(&block_id, &buf[offset], sizeof(int));
    //            //        std::cerr << ""block_id << std::endl;
    //            offset += 4;
    //            dist->seekp(block_id * GetDataBlockSize(record_size));
    //            dist->write(&buf[offset], GetDataBlockSize(record_size));
    //            offset += GetDataBlockSize(record_size);
    //        }
    //        //    if (rec_amount == 0) {
    //        //        char *empty = new char[GetFileSize(dist)];
    //        //        memset(empty,0,GetFileSize(dist));
    //        //        dist->write(empty,GetFileSize(dist));
    //        //        delete[] empty;
    //        //    }
    dist->flush();
    delete[] buf;
}
std::string ConstructFileName(const std::string &table_name, size_t transaction_id) {
    return C::TEMP_DIR + DIR_SEPARATOR + std::to_string(transaction_id) + NAME_SEPARATOR + table_name +
           C::TEMP_FILE_TYPE;
}
std::string FindTempFile(const std::string &table_name, size_t transaction_id) {
    std::string res;
    fs::create_directories(C::TEMP_DIR);
    for (const auto &file : fs::directory_iterator(C::TEMP_DIR)) {
        size_t file_id = 0;
        std::string file_name = file.path().string();
        file_name.erase(file_name.begin(), file_name.begin() + C::TEMP_DIR.size() + 1);
        std::string file_id_str = file_name.substr(0, file_name.find(NAME_SEPARATOR));
        file_id = std::stoll(file_id_str);
        if (file_id == transaction_id and file_name.find(table_name) != std::string::npos) {
            return file.path().string();
        }
    }
    return ConstructFileName(table_name, transaction_id);
}
