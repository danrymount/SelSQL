//
// Created by quiks on 01.10.2019.
//

#ifndef SELSQL_MAINENGINE_H
#define SELSQL_MAINENGINE_H

#include <memory>
#include "../../Utils/Structures/Data/Table.h"

#include "../../Utils/Structures/Message.h"
#include "../Headers/FileManager.h"
#include "Cursor.h"
class MainEngine {
    std::shared_ptr<FileManager> file_manager_;

   public:

    explicit MainEngine();
    Message CreateTable(const std::shared_ptr<Table>& table);
    std::shared_ptr<Table> ShowCreateTable(const std::string& tableName);
    Message DropTable(const std::string& tableName);
    std::pair<std::shared_ptr<Table>, std::shared_ptr<Cursor>> GetCursor(std::string tableName);
};

#endif  // SELSQL_MAINENGINE_H
