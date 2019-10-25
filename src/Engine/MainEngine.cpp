//
// Created by quiks on 01.10.2019.
//

#include "Headers/MainEngine.h"

Error MainEngine::CreateTable(const std::shared_ptr<Table>& table) {
    Error result;
    int error = file_manager_->CreateFile(table);
    file_manager_->CloseAllFiles();
    if (error) {
        result = Error(ErrorConstants::ERR_TABLE_EXISTS);
    }
    return result;
}

std::shared_ptr<Table> MainEngine::ShowCreateTable(const std::string& table_name) {
    std::shared_ptr<Table> table(new Table());
    if (file_manager_->OpenFile(table_name)) {
        return table;
    }
    table = file_manager_->GetTable(table_name);
    file_manager_->CloseAllFiles();
    return table;
}

Error MainEngine::DropTable(const std::string& table_name) {
    Error result;
    int error = file_manager_->DeleteTable(table_name);
    if (error) {
        result = Error(ErrorConstants::ERR_TABLE_NOT_EXISTS);
    }
    file_manager_->CloseAllFiles();
    return result;

    return result;
}  // returns 0 if table is dropped and non-zero if table doesnt exist

MainEngine::MainEngine() { file_manager_ = std::make_shared<FileManager>(); }

std::pair<std::shared_ptr<Table>, std::shared_ptr<Cursor>> MainEngine::GetCursor(std::string table_name) {
    file_manager_->CloseAllFiles();
    std::shared_ptr<Table> table(new Table());
    std::shared_ptr<Cursor> cursor1(new Cursor());

    if (file_manager_->OpenFile(table_name)) {
        return std::make_pair(table, cursor1);
    };
    table = std::make_shared<Table>(*file_manager_->GetTable(table_name));
    cursor1 = std::make_shared<Cursor>(table, file_manager_);
    return std::make_pair(table, cursor1);
}
