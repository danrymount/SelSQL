
#include "Headers/FileManager.h"

void FileManager::WriteMetaData(Table* table) {
    std::fstream* new_file = files_[table->name];
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
    new_file->close();
}

void FileManager::ReadMetaData(std::string table_name) {
    Table table;
    char name[Constants::MD_TABLE_NAME_SIZE];
    int column_size;
    files_[table_name]->read(name, Constants::MD_TABLE_NAME_SIZE);
    files_[table_name]->read(reinterpret_cast<char*>(&column_size), Constants::MD_TABLE_COLUMN_AMOUNT_SIZE);
    for (int i = 0; i < column_size; ++i) {
        Variable var;
        char var_name[Constants::MD_COLUMN_NAME_SIZE];
        int type = 0;
        int constr_size = 0;
        std::vector<Constraint> constraints;
        files_[table_name]->read(var_name, Constants::MD_COLUMN_NAME_SIZE);
        files_[table_name]->read(reinterpret_cast<char*>(&type), Constants::MD_COLUMN_TYPE_SIZE);
        files_[table_name]->read(reinterpret_cast<char*>(&constr_size), Constants::MD_COLUMN_CONSTR_AMOUNT_SIZE);
        var.type = Type(type);

        for (int j = 0; j < constr_size; ++j) {
            int constr_type = 0;
            files_[table_name]->read(reinterpret_cast<char*>(&constr_type), Constants::MD_COLUMN_CONSTR_SIZE);
            constraints.emplace_back(Constraint(constr_type));
        }
        var.setConstraints(constraints);
        table.addField(std::string(var_name),var);
    }
    table.name = std::string(name);

    table_data[table_name] = table;
}
int FileManager::OpenFile(std::string table_name) {
    files_[table_name] = new std::fstream(table_name + Constants::FILE_TYPE,
                                          std::ios::binary | std::ios::out | std::ios::in);
    if (!files_[table_name]->is_open()) {
        return 1;
    }
    ReadMetaData(table_name);

    return 0;
}
int FileManager::CreateFile(Table* table) {
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
    return 0;
}
Table* FileManager::GetTableData(std::string table_name) { return &table_data[table_name]; }

int FileManager::DeleteTable(std::string table_name) {
    if (files_.find(table_name) != files_.end()) {
        files_.erase(table_name);
    }
    return std::remove((table_name + Constants::FILE_TYPE).c_str());
}
void write_int(std::fstream* file, int value) { file->write(reinterpret_cast<char*>(&value), sizeof(int)); }
int read_int(std::fstream* file) {
    int res;
    file->write(reinterpret_cast<char*>(&res), sizeof(int));
    return res;
}
