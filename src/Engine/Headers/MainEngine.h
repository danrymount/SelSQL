//
// Created by quiks on 01.10.2019.
//

#ifndef SELSQL_MAINENGINE_H
#define SELSQL_MAINENGINE_H

#include "../../Utils/Structures/BigResponse.h"
#include "../../Utils/Structures/Data/Table.h"

#include "../Headers/FileManager.h"
#include "Cursor.h"
class MainEngine {
    Cursor* cursor;
    int check_condition(std::string rec_val, Condition cond_val);

   public:
    FileManager* file_manager_;

    explicit MainEngine();

    BigResponse Select(BigRequest* bigRequest);
    BigResponse Insert(BigRequest* bigRequest);
    BigResponse Update(BigRequest* bigRequest);
    BigResponse Delete(BigRequest* bigRequest);
    BigResponse CreateTable(BigRequest* request);
    BigResponse ShowCreateTable(BigRequest request);
    BigResponse DropTable(BigRequest* request);
};

#endif  // SELSQL_MAINENGINE_H
