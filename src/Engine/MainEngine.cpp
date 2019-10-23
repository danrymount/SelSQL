//
// Created by quiks on 01.10.2019.
//

#include "Headers/MainEngine.h"

BigResponse MainEngine::CreateTable(BigRequest* request) {
    BigResponse bigResponse;
    int error = file_manager_->CreateFile(std::make_shared<Table>(request->ddlData.table));
    file_manager_->CloseAllFiles();
    if (error) {
        bigResponse.error = Error(ErrorConstants::ERR_TABLE_EXISTS);
    }
    return bigResponse;
}

BigResponse MainEngine::ShowCreateTable(BigRequest request) {
    BigResponse bigResponse;
    if (file_manager_->OpenFile(request.ddlData.table.name)) {
        bigResponse.error = Error(ErrorConstants::ERR_TABLE_NOT_EXISTS);
    } else {
        bigResponse.ddlData.table = *file_manager_->GetTable(request.ddlData.table.name);
    }
    file_manager_->CloseAllFiles();
    return bigResponse;
}

BigResponse MainEngine::DropTable(BigRequest* request) {
    BigResponse bigResponse;
    int error = file_manager_->DeleteTable(request->ddlData.table.name);
    if (error) {
        bigResponse.error = Error(ErrorConstants::ERR_TABLE_NOT_EXISTS);
    }
    file_manager_->CloseAllFiles();
    return bigResponse;
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
