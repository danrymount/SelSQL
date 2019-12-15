//
// Created by quiks on 01.10.2019.
//

#ifndef SELSQL_MAINENGINE_H
#define SELSQL_MAINENGINE_H

#include <memory>
#include "../../Utils/Structures/Data/Table.h"
#include "../../Utils/Structures/Message.h"
#include "Cursor.h"
#include "FileManager.h"
#include "TransactManager.h"

class MainEngine {
    std::shared_ptr<FileManager> file_manager_;
    std::shared_ptr<TransactManager> transact_manager_;
    std::shared_ptr<DataManager> data_manager_;

   public:
    explicit MainEngine();
    Message CreateTable(const std::shared_ptr<Table>& table);
    std::shared_ptr<Table> ShowCreateTable(const std::string& tableName);
    Message DropTable(const std::string& tableName);
    std::pair<std::shared_ptr<Table>, std::shared_ptr<Cursor>> GetCursor(const std::string& tableName,
                                                                         int64_t transaction_id);
    int64_t GetTransactionSP();
    void Commit(int64_t transaction_id);
};

#endif  // SELSQL_MAINENGINE_H
