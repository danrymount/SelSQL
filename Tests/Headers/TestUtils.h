//
// Created by sapiest on 05.10.2019.
//

#ifndef SELSQL_TESTUTILS_H
#define SELSQL_TESTUTILS_H
#include "../../src/Utils/Headers/CommonUtils.h"
#include "gtest/gtest.h"

class TestUtils {
   public:
    static void comparetTables(const Response &a, const Response &b){
        EXPECT_EQ(a.code, b.code);
        EXPECT_EQ(a.table.name, b.table.name);
        for (auto &field : a.table.getFields()){
            compareField(field.second, b.table.getFields()[field.first]);
        }
        EXPECT_EQ(a.action, b.action);
        EXPECT_EQ(a.errorMsg, b.errorMsg);
        EXPECT_EQ(a.returnMsg, b.errorMsg);
    }

    static void compareField(const Variable &a, const Variable &b){
        EXPECT_EQ(a.type, b.type);
        for(int i = 0; i < a.getConstraints().size(); i++){
            EXPECT_EQ(a.getConstraints()[i], b.getConstraints()[i]);
        }
    }
};

#endif  // SELSQL_TESTUTILS_H
