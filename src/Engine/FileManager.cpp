#include "Headers/FileManager.h"
#include <memory>
#include "../Utils/Structures/Data/DataBlock.h"

void FileManager::WriteTableMetaData(const std::shared_ptr<Table>& table) {
    auto new_file = files_[table->name];
    new_file->seekp(0, std::ios::beg);
    std::string name;
    name.reserve(Constants::MD_COLUMN_NAME_SIZE);
    name = table->name;
    new_file->write(name.c_str(), Constants::MD_COLUMN_NAME_SIZE);
    WriteIntToFile(new_file, table->getFields().size());
    for (auto& field : table->getFields()) {
        name.clear();
        name = field.first;
        new_file->write(name.c_str(), Constants::MD_COLUMN_NAME_SIZE);
        WriteIntToFile(new_file, Type(field.second.type));
        WriteIntToFile(new_file, field.second.size);
        WriteIntToFile(new_file, field.second.getConstraints().size());
        for (auto constr : field.second.getConstraints()) {
            WriteIntToFile(new_file, Constraint(constr));
        }
    }

    //    std::cerr << Constants::DATA_BLOCK_START_POS - 4 << std::endl;
    new_file->seekp(Constants::DATA_BLOCK_START_POS - 4, std::ios::beg);
    WriteIntToFile(new_file, table->record_amount);
}

void FileManager::ReadTableMetaData(const std::string& table_name) {
    auto table = std::make_shared<Table>();
    char name[Constants::MD_TABLE_NAME_SIZE];
    int column_size;
    files_[table_name]->read(name, Constants::MD_TABLE_NAME_SIZE);
    column_size = ReadIntoFromFile(files_[table_name]);
    for (int i = 0; i < column_size; ++i) {
        Variable var;
        char var_name[Constants::MD_COLUMN_NAME_SIZE];
        int type = 0;
        int type_size = 0;
        int constr_size = 0;
        std::vector<Constraint> constraints;
        files_[table_name]->read(var_name, Constants::MD_COLUMN_NAME_SIZE);
        type = ReadIntoFromFile(files_[table_name]);
        type_size = ReadIntoFromFile(files_[table_name]);
        constr_size = ReadIntoFromFile(files_[table_name]);
        var.type = Type(type);
        var.size = type_size;
        for (int j = 0; j < constr_size; ++j) {
            int constr_type = 0;
            constr_type = ReadIntoFromFile(files_[table_name]);
            constraints.emplace_back(Constraint(constr_type));
        }
        var.setConstraints(constraints);
        table->addField(std::string(var_name), var);
    }
    table->name = std::string(name);
    files_[table_name]->seekg(Constants::DATA_BLOCK_START_POS - 4, std::ios::beg);
    table->record_amount = ReadIntoFromFile(files_[table_name]);
    table->calcRecordSize();
    table_data[table_name] = table;
}
int FileManager::OpenFile(const std::string& table_name) {
    files_[table_name] = std::make_shared<std::fstream>(table_name + Constants::FILE_TYPE,
                                                        std::ios::binary | std::ios::out | std::ios::in);
    if (!files_[table_name]->is_open()) {
        files_.erase(table_name);
        return 1;
    }
    ReadTableMetaData(table_name);
    return 0;
}
int FileManager::CreateFile(const std::shared_ptr<Table>& table) {
    this->CloseAllFiles();
    //    if (files_.find(table->name) != files_.end()) {
    //        files_[table->name]->close();
    //        files_.erase(table->name);
    //    }

    std::ifstream file;
    file.open(table->name + Constants::FILE_TYPE);
    if (file.is_open()) {
        file.close();
        return 1;
    }
    std::cout << table->name + Constants::FILE_TYPE << std::endl;
    files_[table->name] = std::make_shared<std::fstream>(table->name + Constants::FILE_TYPE,
                                                         std::ios::binary | std::ios::out | std::ios::in | std::ios::trunc);

    WriteTableMetaData(table);
    this->CloseAllFiles();

    return 0;
}
std::shared_ptr<Table> FileManager::GetTable(const std::string& table_name) { return table_data[table_name]; }

int FileManager::DeleteFile(const std::string& table_name) {
    this->CloseAllFiles();
    return std::remove((table_name + Constants::FILE_TYPE).c_str());
}

int FileManager::UpdateFile(const std::shared_ptr<Table>& table, const std::vector<std::shared_ptr<DataBlock>>& data) {
    this->WriteTableMetaData(table);
    if (data.empty()) {
        return 0;
    }

    this->WriteDataBlocks(std::string(table->name), data);
    return 0;
}

std::vector<std::shared_ptr<DataBlock>> FileManager::ReadDataBlocks(const std::string& table_name) {
    std::vector<std::shared_ptr<DataBlock>> data;
    int readed_data = 0;
    int offset = Constants::DATA_BLOCK_START_POS;
    auto infile = files_[table_name];
    auto table = table_data[table_name];
    if (table->record_amount == 0) {
        auto dataBlock = std::make_shared<DataBlock>();
        dataBlock->record_size = table->record_size;
        dataBlock->max_deleted_amount = Constants::DATA_SIZE / table->record_size;
        dataBlock->setDeletedPos(new char[dataBlock->max_deleted_amount * sizeof(short int)]);
        data.emplace_back(dataBlock);
        return data;
    }
    while (readed_data != table->record_amount) {
        //        std::cerr<<offset<<std::endl;
        auto new_data = new char[Constants::DATA_SIZE];
        auto dataBlock = std::make_shared<DataBlock>();
        infile->seekg(offset, std::ios::beg);
        dataBlock->record_size = table->record_size;
        dataBlock->record_amount = ReadIntoFromFile(infile);
        readed_data += dataBlock->record_amount;
        dataBlock->last_record_pos = ReadIntoFromFile(infile);
        dataBlock->deleted = ReadIntoFromFile(infile);
        dataBlock->max_deleted_amount = Constants::DATA_SIZE / table->record_size;
        char* deleted = new char[dataBlock->max_deleted_amount * sizeof(short int)];
        infile->read(deleted, dataBlock->max_deleted_amount * sizeof(short int));
        dataBlock->setDeletedPos(deleted);
        offset += Constants::DATA_BLOCK_RECORD_AMOUNT + Constants::DATA_BLOCK_DELETED_AMOUNT +
                  Constants::DATA_BLOCK_RECORD_AMOUNT + Constants::DATA_BLOCK_RECORD_LAST_POS +
                  dataBlock->max_deleted_amount * sizeof(short int);
        //        std::cerr<<offset<<std::endl;
        infile->seekg(offset, std::ios::beg);
        infile->read(new_data, Constants::DATA_SIZE);
        offset += Constants::DATA_SIZE;
        dataBlock->setData(new_data);
        data.emplace_back(dataBlock);
    }

    return data;
}
void FileManager::WriteDataBlocks(const std::string& table_name, const std::vector<std::shared_ptr<DataBlock>>& data) {
    auto outfile = files_[table_name];
    int offset = Constants::DATA_BLOCK_START_POS;
    for (const auto& block : data) {
        if (block->record_amount == 0) {
            continue;
        }
        //        std::cerr << offset << std::endl;
        outfile->seekp(offset, std::ios::beg);
        WriteIntToFile(outfile, block->record_amount);
        WriteIntToFile(outfile, block->last_record_pos);
        WriteIntToFile(outfile, block->deleted);
        outfile->write(block->getDeletedPos(), block->max_deleted_amount * sizeof(short int));
        offset += Constants::DATA_BLOCK_RECORD_AMOUNT + Constants::DATA_BLOCK_DELETED_AMOUNT +
                  Constants::DATA_BLOCK_RECORD_AMOUNT + Constants::DATA_BLOCK_RECORD_LAST_POS +
                  block->max_deleted_amount * sizeof(short int);
        //        std::cerr<<offset<<std::endl;
        outfile->seekp(offset, std::ios::beg);
        outfile->write(block->data_, Constants::DATA_SIZE);
        //        char new_d[Constants::DATA_SIZE];
        //        outfile->seekg(offset,std::ios::beg);
        //        outfile->read(new_d,Constants::DATA_SIZE);
        offset += Constants::DATA_SIZE;
    }
    outfile->seekp(offset, std::ios::beg);
    WriteIntToFile(outfile, 0);
}

void FileManager::CloseAllFiles() {
    for (const auto& file : files_) {
        file.second->close();
    }
    files_.clear();
}

void WriteIntToFile(const std::shared_ptr<std::fstream>& file, int value) {
    file->write(reinterpret_cast<char*>(&value), sizeof(int));
}
int ReadIntoFromFile(const std::shared_ptr<std::fstream>& file) {
    int res;
    file->read(reinterpret_cast<char*>(&res), sizeof(int));
    return res;
}