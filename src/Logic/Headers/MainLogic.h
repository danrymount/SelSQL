//
// Created by toxaxab on 30.09.2019.
//

#ifndef SELSQL_MAINLOGIC_H
#define SELSQL_MAINLOGIC_H

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>
#include "../../Utils/Headers/ActionsUtils.h"
#include "../../Utils/Headers/ParserUtils.h"
#include "../Actions/Headers/ActionCreate.h"
#include "../Actions/Headers/BaseAction.h"

using std::string;

class MainLogic {
   public:
    static Response executeRequest(Response &_response);
};

#endif  // SELSQL_MAINLOGIC_H
