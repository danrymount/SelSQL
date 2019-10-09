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
        for (int i = 0; i < a.ddlData.table.getFields().size(); i++) {
            EXPECT_EQ(a.ddlData.table.getFields()[i].first, b.ddlData.table.getFields()[i].first);
            compareField(a.ddlData.table.getFields()[i].second, b.ddlData.table.getFields()[i].second);
        }
        EXPECT_EQ(a.action, b.action);
        EXPECT_EQ(a.error.getErrorCode(), b.error.getErrorCode());
        EXPECT_EQ(a.error.getErrorMsg(), b.error.getErrorMsg());
    }

    static void compareField(const Variable &a, const Variable &b) {
        EXPECT_EQ(a.type, b.type);
        for (int i = 0; i < a.getConstraints().size(); i++) {
            EXPECT_EQ(a.getConstraints()[i], b.getConstraints()[i]);
        }
    }

    static void compareDql(BigResponse &a, BigResponse &b) {
        auto at = a.dqlData;
        auto bt = b.dqlData;
        for (int i = 0; i < at.columns.size(); ++i) {
            EXPECT_EQ(at.columns[i], bt.columns[i]);
        }
        compareConditions(at, bt);
    }

    static void compareDml(BigResponse &a, BigResponse &b) {
        auto at = a.dmlData;
        auto bt = b.dmlData;
        if (!at.columns.empty()) {
            EXPECT_EQ(at.columns.size(), bt.columns.size());
            EXPECT_EQ(at.values.size(), bt.values.size());
            for (int i = 0; i < at.columns.size(); ++i) {
                EXPECT_EQ(at.columns[i], bt.columns[i]);
                EXPECT_EQ(at.values[i], bt.values[i]);
            }
            compareConditions(at, bt);
        } else {
            EXPECT_EQ(at.columns.empty(), bt.columns.empty());
        }
    }

    static void compareConditions(DQLdata &at, DQLdata &bt) {
        auto bIt = bt.conditions.begin();
        for (auto aIt = at.conditions.begin(); aIt != at.conditions.end(); aIt++) {
            EXPECT_EQ((*aIt).second.value, (*bIt).second.value);
            EXPECT_EQ((*aIt).second.sign, (*bIt).second.sign);
            EXPECT_EQ((*aIt).first, (*bIt).first);
            if (bIt != bt.conditions.end())
                bIt++;
        }
    }
};

#endif  // SELSQL_TESTUTILS_H