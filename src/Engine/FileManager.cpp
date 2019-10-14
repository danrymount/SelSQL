#include "Headers/FileManager.h"
#include <memory>
#include "../Utils/Structures/Data/Table.h"

void FileManager::WriteMetaData(const std::shared_ptr<Table>& table) {
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
    new_file->seekp(Constants::DATA_PAGE_START_POS - 4 - 4);
    write_int(new_file, table->record_amount);
    write_int(new_file, table->last_record_pos);
    new_file->seekp(Constants::DATA_PAGE_START_POS + Constants::DATA_PAGE_SIZE);
    write_int(new_file, 0);
}

void FileManager::ReadMetaData(std::string table_name) {
    Table table;
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
        table.addField(std::string(var_name), var);
    }
    table.name = std::string(name);
    files_[table_name]->seekg(Constants::DATA_PAGE_START_POS - 4 - 4);
    table.record_amount = read_int(files_[table_name]);
    table.last_record_pos = read_int(files_[table_name]);
    table.calcRecordSize();
    table_data[table_name] = table;
}
int FileManager::OpenFile(std::string table_name) {
    files_[table_name] = new std::fstream(table_name + Constants::FILE_TYPE,
                                          std::ios::binary | std::ios::out | std::ios::in);
    if (!files_[table_name]->is_open()) {
        files_.erase(table_name);
        return 1;
    }
    ReadMetaData(table_name);

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

    WriteMetaData(table);
    files_[table->name]->close();
    return 0;
}
Table* FileManager::GetTableData(std::string table_name) { return &table_data[table_name]; }

int FileManager::DeleteTable(std::string table_name) {
    if (files_.find(table_name) != files_.end()) {
        files_.erase(table_name);
    }
    return std::remove((table_name + Constants::FILE_TYPE).c_str());
}
unsigned char* FileManager::GetData(std::string table_name) {
    auto new_data = new char[Constants::DATA_PAGE_SIZE];

    files_[table_name]->seekg(Constants::DATA_PAGE_START_POS, std::ios::beg);
    files_[table_name]->read(new_data, Constants::DATA_PAGE_SIZE);
    //    std::cerr << files_[table_name]->tellg();
    files_[table_name]->seekg(0, std::ios::beg);
    //    std::cerr << files_[table_name]->tellg();
    auto res = reinterpret_cast<unsigned char*>(new_data);
    return res;
}
int FileManager::UpdateFile(const std::shared_ptr<Table>& table, unsigned char* src) {
    this->WriteMetaData(table);
    this->WriteData(table, src);
    return 0;
}
void FileManager::WriteData(const std::shared_ptr<Table>& table, unsigned char* src) {
    std::fstream* new_file = files_[table->name];

    auto res = reinterpret_cast<char*>(src);
    new_file->seekp(Constants::DATA_PAGE_START_POS, std::ios::beg);
    //    std::cerr << new_file->tellp();
    new_file->write(res, Constants::DATA_PAGE_SIZE);

    new_file->close();
}
void write_int(std::fstream* file, int value) { file->write(reinterpret_cast<char*>(&value), sizeof(int)); }
int read_int(std::fstream* file) {
    int res;
    file->read(reinterpret_cast<char*>(&res), sizeof(int));
    return res;
}