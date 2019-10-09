//
// Created by sapiest on 05.10.2019.
//

#ifndef SELSQL_TESTUTILS_H
#define SELSQL_TESTUTILS_H
#include "../../src/Utils/Headers/CommonUtils.h"
#include "gtest/gtest.h"

class TestUtils {
   public:
    static void compareTables(BigResponse &a, BigResponse &b) {
        EXPECT_EQ(a.error.getErrorCode(), b.error.getErrorCode());
        EXPECT_EQ(a.tableName, b.tableName);
        for (auto &field : a.ddlData.table.getFields()) {
            compareField(field.second, b.ddlData.table.getFields()[field.first]);
        }
        EXPECT_EQ(a.action, b.action);
        EXPECT_EQ(a.error.getErrorCode(), b.error.getErrorCode());
        EXPECT_EQ(a.error.getErrorMsg(), b.error.getErrorMsg());
    }

    static void compareField(const Variable &a, const Variable &b){
        EXPECT_EQ(a.type, b.type);
        for(int i = 0; i < a.getConstraints().size(); i++){
            EXPECT_EQ(a.getConstraints()[i], b.getConstraints()[i]);
        }
    }
};

#endif  // SELSQL_TESTUTILS_H
