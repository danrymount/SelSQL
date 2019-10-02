
#include "Headers/FileManager.h"

void FileManager::WriteMetaData(Table* table) {
    files_[table->name]->write(table->name.c_str(), sizeof(table->name.size()));
    files_[table->name]->seekp(Constants::MD_COLUMN_NAME_SIZE);
    int field_size = table->fields.size();
    files_[table->name]->write(reinterpret_cast<char*>(&field_size), Constants::MD_TABLE_COLUMN_AMOUNT_SIZE);
    for (auto& field : table->fields) {
        files_[table->name]->write(field.name.c_str(), Constants::MD_COLUMN_NAME_SIZE);
        int constr_size = field.constraints.size();
        int type = Type(field.type);
        files_[table->name]->write(reinterpret_cast<char*>(&type), Constants::MD_COLUMN_TYPE_SIZE);
        files_[table->name]->write(reinterpret_cast<char*>(&constr_size), Constants::MD_COLUMN_CONSTR_AMOUNT_SIZE);
        for (auto constr : field.constraints) {
            int contr_type = Constraint(constr);
            files_[table->name]->write(reinterpret_cast<char*>(&contr_type), Constants::MD_COLUMN_CONSTR_SIZE);
        }
    }
    files_[table->name]->close();
}

void FileManager::ReadMetaData(std::string table_name) {
    Table table;
    std::vector<Variable> fields;
    char name[Constants::MD_TABLE_NAME_SIZE];
    int column_size;
    files_[table_name]->read(name, Constants::MD_TABLE_NAME_SIZE);
    files_[table_name]->read(reinterpret_cast<char*>(&column_size), Constants::MD_TABLE_COLUMN_AMOUNT_SIZE);
    for (int i = 0; i < column_size; ++i) {
        Variable var;
        char var_name[Constants::MD_COLUMN_NAME_SIZE];
        int type = 0;
        int constr_size = 0;
        std::vector<Constraint> constraint;
        files_[table_name]->read(var_name, Constants::MD_COLUMN_NAME_SIZE);
        files_[table_name]->read(reinterpret_cast<char*>(&type), Constants::MD_COLUMN_TYPE_SIZE);
        files_[table_name]->read(reinterpret_cast<char*>(&constr_size), Constants::MD_COLUMN_CONSTR_AMOUNT_SIZE);
        var.name = std::string(var_name);
        var.type = Type(type);

        for (int j = 0; j < constr_size; ++j) {
            int constr_type = 0;
            files_[table_name]->read(reinterpret_cast<char*>(&constr_type), Constants::MD_COLUMN_CONSTR_SIZE);
            constraint.emplace_back(Constraint(constr_type));
        }
        var.constraints = constraint;
        fields.emplace_back(var);
    }
    table.name = std::string(name);
    table.fields = fields;
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
