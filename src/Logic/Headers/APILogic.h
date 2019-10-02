//
// Created by toxaxab on 30.09.2019.
//

#ifndef SELSQL_APILOGIC_H
#define SELSQL_APILOGIC_H

class APILogic {
   public:
    virtual void addColumn(char* name, char* type) = 0;
    virtual void addConstraint(char* name) = 0;
    virtual Table* finish() = 0;
    virtual void addTableName(char* name) = 0;
    virtual void addActionName(char* name) = 0;
};

#endif  // SELSQL_APILOGIC_H
