//
// Created by toxaxab on 30.09.2019.
//

#include "Headers/MainLogic.h"
#include <string>
void MainLogic::addColumn(char* name, char* type) {
    printf("VALNAME = %s, TYPE = %s\n", name, type);
}

void MainLogic::addConstraint(char* name) {
    printf("CONSTRAINT = %s\n", name);
}
void MainLogic::finish() {
    count += 1;
    printf("SUCCESS %d\n", count);
}
void MainLogic::addTableName(char* name) {
    printf("TABLENAME = %s\n", name);
}
void MainLogic::addActionName(char* name) {
    printf("ACTION = %s\n", name);
}
