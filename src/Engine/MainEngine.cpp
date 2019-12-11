//
// Created by quiks on 01.10.2019.
//

#include "Headers/MainEngine.h"

Message MainEngine::CreateTable(const std::shared_ptr<Table>& table) {
    Message result;
    int error = file_manager_->CreateFile(table);
    if (error) {
        result = Message(ErrorConstants::ERR_TABLE_EXISTS);
    }
    return result;
}

std::shared_ptr<Table> MainEngine::ShowCreateTable(const std::string& tableName) {
    std::shared_ptr<Table> table(new Table());

    auto [meta, data] = file_manager_->OpenFile(tableName);
    if (meta == nullptr or data == nullptr) {
        return table;
    }
    file_manager_->ReadTableMetaData(tableName, meta);
    table = file_manager_->GetTable(tableName);
    return table;
}

Message MainEngine::DropTable(const std::string& tableName) {
    Message result;
    int error = file_manager_->DeleteFile(tableName);
    if (error) {
        result = Message(ErrorConstants::ERR_TABLE_NOT_EXISTS);
    }
    return result;
}

MainEngine::MainEngine() {
    transact_manager_ = std::make_shared<TransactManager>();
    file_manager_ = std::make_shared<FileManager>(transact_manager_);
}

std::pair<std::shared_ptr<Table>, std::shared_ptr<Cursor>> MainEngine::GetCursor(const std::string& tableName,
                                                                                 long transaction_sp) {
    std::shared_ptr<Table> table(new Table());
    std::shared_ptr<Cursor> cursor(new Cursor());
    std::cerr << "GET CURSOR  " << transaction_sp << std::endl;
    auto [meta, data] = file_manager_->OpenFile(tableName);

    if (meta == nullptr or data == nullptr) {
        table = std::make_shared<Table>();
        return std::make_pair(table, cursor);
    }
    file_manager_->ReadTableMetaData(tableName, meta);
    table = file_manager_->GetTable(tableName);
    cursor = std::make_shared<Cursor>(table, file_manager_, transact_manager_, data);
    return std::make_pair(table, cursor);
}
long MainEngine::GetTransactionSP() { return transact_manager_->GetTransactionSP(); }

void MainEngine::Commit(long transaction_sp) {
    if (transact_manager_->IsSuccessful(transaction_sp)) {
        std::cerr << "COMMIT id = " << transaction_sp << std::endl;
    }

    transact_manager_->ClearUsed(transaction_sp);
    //    file_manager_->Clear(transaction_sp);
}
