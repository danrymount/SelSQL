//
// Created by User on 10.11.2019.
//

#ifndef SELSQL_SERVERUTILSLIN_H
#define SELSQL_SERVERUTILSLIN_H

class ServerUtils {
   public:
    static void startServer();
    static void closeServer(int);
    static void setSockOpt(int);
};

#endif  // SELSQL_SERVERUTILSLIN_H
