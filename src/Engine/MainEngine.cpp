//
// Created by quiks on 01.10.2019.
//

#include "Headers/MainEngine.h"

Message MainEngine::CreateTable(const std::shared_ptr<Table>& table) {
    Message result;
    int error = file_manager_->CreateFile(table);
    file_manager_->CloseAllFiles();
    if (error) {
        result = Message(ErrorConstants::ERR_TABLE_EXISTS);
    }
    return result;
}

std::shared_ptr<Table> MainEngine::ShowCreateTable(const std::string& tableName) {
    std::shared_ptr<Table> table(new Table());
    if (file_manager_->OpenFile(tableName)) {
        return table;
    }
    table = file_manager_->GetTable(tableName);
    file_manager_->CloseAllFiles();
    return table;
}

Message MainEngine::DropTable(const std::string& tableName) {
    Message result;
    int error = file_manager_->DeleteFile(tableName);
    if (error) {
        result = Message(ErrorConstants::ERR_TABLE_NOT_EXISTS);
    }
    file_manager_->CloseAllFiles();
    return result;
}

MainEngine::MainEngine() {
    file_manager_ = std::make_shared<FileManager>();
    transact_manager_ = std::make_shared<TransactManager>();
}

std::pair<std::shared_ptr<Table>, std::shared_ptr<Cursor>> MainEngine::GetCursor(const std::string& tableName) {
    file_manager_->CloseAllFiles();
    std::shared_ptr<Table> table(new Table());
    std::shared_ptr<Cursor> cursor(new Cursor());

    if (file_manager_->OpenFile(tableName)) {
        return std::make_pair(table, cursor);
    }
    table = file_manager_->GetTable(tableName);
    cursor = std::make_shared<Cursor>(table, file_manager_, transact_manager_);
    return std::make_pair(table, cursor);
}
int MainEngine::GetTransactionId() { return transact_manager_->GetTransactionId(); }

void MainEngine::Commit(int transaction_id) {
    std::cerr << "COMMIT id = " << transaction_id << std::endl;
    transact_manager_->ClearUsed(transaction_id);
}
