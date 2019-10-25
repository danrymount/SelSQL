//
// Created by quiks on 01.10.2019.
//

#ifndef SELSQL_MAINENGINE_H
#define SELSQL_MAINENGINE_H

#include <memory>
#include "../../Utils/Structures/Data/Table.h"

#include "../../Utils/Structures/Error.h"
#include "../Headers/FileManager.h"
#include "Cursor.h"
class MainEngine {
    Cursor* cursor;

   public:
    std::shared_ptr<FileManager> file_manager_;

    explicit MainEngine();
    std::pair<std::shared_ptr<Table>, std::shared_ptr<Cursor>> GetCursor(std::string table_name);

    Error CreateTable(const std::shared_ptr<Table>& table);
    std::shared_ptr<Table> ShowCreateTable(const std::string& table_name);
    Error DropTable(const std::string& table_name);
};

#endif  // SELSQL_MAINENGINE_H
