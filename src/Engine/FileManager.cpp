#include "Headers/FileManager.h"
#include <cstring>
#include <memory>
#include "../Utils/Structures/Data/DataBlock.h"
#include "../Utils/Structures/Data/Table.h"

void FileManager::WriteTableMetaData(const std::shared_ptr<Table>& table) {
    std::fstream* new_file = files_[table->name];
    new_file->seekp(0);
    new_file->write(table->name.c_str(), table->name.size());
    new_file->seekp(Constants::MD_COLUMN_NAME_SIZE);
    write_int(new_file, table->getFields().size());
    for (auto& field : table->getFields()) {
        new_file->write(field.first.c_str(), Constants::MD_COLUMN_NAME_SIZE);
        write_int(new_file, Type(field.second.type));
        write_int(new_file, field.second.getConstraints().size());
        for (auto constr : field.second.getConstraints()) {
            write_int(new_file, Constraint(constr));
        }
    }

    files_[table->name]->seekp(Constants::DATA_BLOCK_START_POS - 4, std::ios::beg);
    write_int(files_[table->name], table->record_amount);
}

void FileManager::ReadTableMetaData(std::string table_name) {
    auto table = new Table();
    char name[Constants::MD_TABLE_NAME_SIZE];
    int column_size;
    files_[table_name]->read(name, Constants::MD_TABLE_NAME_SIZE);
    column_size = read_int(files_[table_name]);
    for (int i = 0; i < column_size; ++i) {
        Variable var;
        char var_name[Constants::MD_COLUMN_NAME_SIZE];
        int type = 0;
        int constr_size = 0;
        std::vector<Constraint> constraints;
        files_[table_name]->read(var_name, Constants::MD_COLUMN_NAME_SIZE);
        type = read_int(files_[table_name]);
        constr_size = read_int(files_[table_name]);
        var.type = Type(type);

        for (int j = 0; j < constr_size; ++j) {
            int constr_type = 0;
            constr_type = read_int(files_[table_name]);
            constraints.emplace_back(Constraint(constr_type));
        }
        var.setConstraints(constraints);
        table->addField(std::string(var_name), var);
    }
    table->name = std::string(name);
    files_[table_name]->seekg(Constants::DATA_BLOCK_START_POS - 4, std::ios::beg);
    table->record_amount = read_int(files_[table_name]);
    table->calcRecordSize();
    table_data[table_name] = table;
}
int FileManager::OpenFile(std::string table_name) {
    files_[table_name] = new std::fstream(table_name + Constants::FILE_TYPE,
                                          std::ios::binary | std::ios::out | std::ios::in);
    if (!files_[table_name]->is_open()) {
        files_.erase(table_name);
        return 1;
    }
    ReadTableMetaData(table_name);

    return 0;
}
int FileManager::CreateFile(const std::shared_ptr<Table>& table) {
    if (files_.find(table->name) != files_.end()) {
        return 1;
    }

    std::ifstream file;
    file.open(table->name + Constants::FILE_TYPE);
    if (file.is_open()) {
        file.close();
        return 1;
    }
    std::cout << table->name + Constants::FILE_TYPE << std::endl;
    files_[table->name] = new std::fstream(table->name + Constants::FILE_TYPE,
                                           std::ios::binary | std::ios::out | std::ios::in | std::ios::trunc);

    WriteTableMetaData(table);
    files_[table->name]->close();
    return 0;
}
Table* FileManager::GetTable(std::string table_name) { return table_data[table_name]; }

int FileManager::DeleteTable(std::string table_name) {
    if (files_.find(table_name) != files_.end()) {
        files_[table_name]->close();
        files_.erase(table_name);
    }
    return std::remove((table_name + Constants::FILE_TYPE).c_str());
}

int FileManager::UpdateFile(const std::shared_ptr<Table>& table, const std::vector<DataBlock*>& data) {
    this->WriteTableMetaData(table);
    if (data.size() == 0) {
        return 0;
    }
    this->WriteDataBlock(table->name, data);
    this->files_[table->name]->close();
    return 0;
}

std::vector<DataBlock*> FileManager::ReadDataBlocks(const std::string& table_name) {
    std::vector<DataBlock*> data;
    int readed_data = 0;
    int offset = Constants::DATA_BLOCK_START_POS;
    auto infile = files_[table_name];
    auto table = table_data[table_name];
    if (table->record_amount == 0) {
        auto dataBlock = new DataBlock;
        dataBlock->record_size = table->record_size;
        dataBlock->max_deleted_amount = Constants::DATA_SIZE / table->record_size;
        dataBlock->setDeletedPos(new char[dataBlock->max_deleted_amount * sizeof(short int)]);
        data.emplace_back(dataBlock);
        return data;
    }
    while (readed_data != table->record_amount) {
        auto new_data = new char[Constants::DATA_SIZE];
        auto dataBlock = new DataBlock();
        infile->seekg(offset, std::ios::beg);
        dataBlock->record_size = table->record_size;
        dataBlock->record_amount = read_int(infile);
        readed_data += dataBlock->record_amount;
        dataBlock->last_record_pos = read_int(infile);
        dataBlock->deleted = read_int(infile);
        dataBlock->max_deleted_amount = Constants::DATA_SIZE / table->record_size;
        char* deleted = new char[dataBlock->max_deleted_amount * sizeof(short int)];
        infile->read(deleted, dataBlock->max_deleted_amount * sizeof(short int));
        dataBlock->setDeletedPos(deleted);
        offset += Constants::DATA_BLOCK_RECORD_AMOUNT + Constants::DATA_BLOCK_DELETED_AMOUNT +
                  Constants::DATA_BLOCK_RECORD_AMOUNT + Constants::DATA_BLOCK_RECORD_LAST_POS +
                  dataBlock->max_deleted_amount * sizeof(short int);
        infile->seekg(offset, std::ios::beg);
        infile->read(new_data, Constants::DATA_SIZE);
        offset += Constants::DATA_SIZE;
        dataBlock->setData(new_data);
        data.emplace_back(dataBlock);
    }

    return data;
}
void FileManager::WriteDataBlock(std::string table_name, const std::vector<DataBlock*>& data) {
    auto outfile = files_[table_name];
    int offset = Constants::DATA_BLOCK_START_POS;
    for (auto block : data) {
        outfile->seekp(offset, std::ios::beg);
        write_int(outfile, block->record_amount);
        write_int(outfile, block->last_record_pos);
        write_int(outfile, block->deleted);
        outfile->write(block->getDeletedPos(), block->max_deleted_amount * sizeof(short int));
        offset += Constants::DATA_BLOCK_RECORD_AMOUNT + Constants::DATA_BLOCK_DELETED_AMOUNT +
                  Constants::DATA_BLOCK_RECORD_AMOUNT + Constants::DATA_BLOCK_RECORD_LAST_POS +
                  block->max_deleted_amount * sizeof(short int);
        outfile->seekp(offset, std::ios::beg);
        outfile->write(block->data_, Constants::DATA_SIZE);
        offset += Constants::DATA_SIZE;
        delete block;
    }
    outfile->close();
}
void write_int(std::fstream* file, int value) { file->write(reinterpret_cast<char*>(&value), sizeof(int)); }
int read_int(std::fstream* file) {
    int res;
    file->read(reinterpret_cast<char*>(&res), sizeof(int));
    return res;
}