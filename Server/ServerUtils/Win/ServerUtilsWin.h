//
// Created by User on 10.11.2019.
//

#ifndef SELSQL_SERVERUTILSWIN_H
#define SELSQL_SERVERUTILSWIN_H

class ServerUtils {
   public:
    static void startServer();
    static void closeServer(int);
    static void setSockOpt(int);
};

#endif  // SELSQL_SERVERUTILSWIN_H
