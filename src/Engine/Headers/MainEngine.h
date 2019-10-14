//
// Created by quiks on 01.10.2019.
//

#ifndef SELSQL_MAINENGINE_H
#define SELSQL_MAINENGINE_H

#include <memory>
#include "../../Utils/Structures/BigResponse.h"
#include "../../Utils/Structures/Data/Table.h"

#include "../Headers/FileManager.h"
#include "Cursor.h"
class MainEngine {
    Cursor* cursor;

   public:
    std::shared_ptr<FileManager> file_manager_;

    explicit MainEngine();
    std::pair<std::shared_ptr<Table>, std::shared_ptr<Cursor>> GetCursor(std::string table_name);

    BigResponse CreateTable(BigRequest* request);
    BigResponse ShowCreateTable(BigRequest request);
    BigResponse DropTable(BigRequest* request);
};

#endif  // SELSQL_MAINENGINE_H
