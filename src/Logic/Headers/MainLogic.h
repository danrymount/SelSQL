//
// Created by toxaxab on 30.09.2019.
//

#ifndef SELSQL_MAINLOGIC_H
#define SELSQL_MAINLOGIC_H

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>
#include "../../Engine/Headers/MainEngine.h"
#include "../../Utils/Structures/BigResponse.h"
using std::string;

class MainLogic {
    inline static MainEngine* mainEngine = new MainEngine();

   public:
    static BigResponse executeRequest(std::shared_ptr<BigRequest> _request);
};

#endif  // SELSQL_MAINLOGIC_H
