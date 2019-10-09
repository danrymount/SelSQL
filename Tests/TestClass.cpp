//
// Created by sapiest on 03.10.2019.
//
#include <gtest/gtest.h>
#include "../src/Logic/Headers/MainLogic.h"
#include "../src/Utils/Structures/Data/Table.h"
#include "../src/Utils/Structures/Data/Variable.h"
#include "Headers/TestUtils.h"
#include "parser.cpp"

BigResponse res;
std::string str;

TEST(CREATE_TEST, TEST2) {
    std::string str = "CREATE TABLE name1(ID INT);";
    res = parse_request(str.c_str());
    EXPECT_EQ("name1", res.ddlData.table.name);
    auto field = *(res.ddlData.table.getFields().begin());
    EXPECT_EQ("ID", field.first);
    EXPECT_EQ(0, field.second.type);
}
TEST(CREATE_TEST, TEST3) {
    std::string str = "CREATE TABLE name2(col1 FLOAT NOT NULL);";
    res = parse_request(str.c_str());
    EXPECT_EQ("name2", res.ddlData.table.name);
    auto field = *(res.ddlData.table.getFields().begin());
    EXPECT_EQ("col1", field.first);
    EXPECT_EQ(1, field.second.type);
    EXPECT_EQ(0, field.second.getConstraints()[0]);
}
// TEST(CREATE_TEST, TEST4) {
//    std::string str = "CREATE TABLE name3(col1 BOOLEAN UNIQUE, col2 INT PRIMARY KEY);";
//    res = parse_request(str.c_str());
//    std::vector<std::pair<std::string, Variable>> fields = {{"col1",
//                                                             Variable(BOOLEAN, std::vector<Constraint>{UNIQUE})},
//                                                            {"col2",
//                                                             Variable(INT, std::vector<Constraint>{PRIMARY_KEY})}};
//    Error err;
//    BigResponse obj(CREATE, "name3", DDLdata(Table(string("name"), fields), ""), DMLdata(), DQLdata(), err);
//    TestUtils::compareTables(obj, res);
//}

TEST(CREATE_TEST, TEST5) {
    std::string str = "CREATE TABLE name4(col1 INT UNIQUE NOT NULL, col2 CHAR, col3 FLOAT NOT NULL);";
    res = parse_request(str.c_str());
    std::vector<std::pair<std::string, Variable>> fields = {{"col1",
                                                             Variable(INT, std::vector<Constraint>{UNIQUE, NOT_NULL})},
                                                            {"col2", Variable(CHAR, std::vector<Constraint>{})},
                                                            {"col3",
                                                             Variable(FLOAT, std::vector<Constraint>{NOT_NULL})}};
    Error err;
    BigResponse obj(CREATE, "name4", DDLdata(Table("name4", fields), ""), DMLdata(), DQLdata(), err);
    TestUtils::compareTables(obj, res);
}
// TEST(CREATE_TEST, TEST6) {
//    std::string str = "CREATE TABLE name5(col1 FLOAT UNIQUE PRIMARY KEY, col2 CHAR NOT NULL PRIMARY KEY UNIQUE, col3 "
//                      "BOOLEAN);";
//    res = parse_request(str.c_str());
//    std::vector<std::pair<std::string, Variable>> fields = {{"col1",
//                                                             Variable(FLOAT,
//                                                                      std::vector<Constraint>{UNIQUE, PRIMARY_KEY})},
//                                                            {"col2", Variable(CHAR, std::vector<Constraint>{NOT_NULL,
//                                                                                                            PRIMARY_KEY,
//                                                                                                            UNIQUE})},
//                                                            {"col3", Variable(BOOLEAN, std::vector<Constraint>{})}};
//    Error err;
//    BigResponse obj(CREATE, "name5", DDLdata(Table("name5", fields), ""), DMLdata(), DQLdata(), err);
//    TestUtils::compareTables(obj, res);
//}
// TEST(CREATE_TEST, TEST7) {
//    std::string str = "CREATE TABLE name6(col1 BOOLEAN , col2 CHAR , col3 INT);";
//    res = parse_request(str.c_str());
//    std::vector<std::pair<std::string, Variable>> fields = {{"col1", Variable(BOOLEAN, std::vector<Constraint>{})},
//                                                            {"col2", Variable(CHAR, std::vector<Constraint>{})},
//                                                            {"col3", Variable(INT, std::vector<Constraint>{})}};
//    Error err;
//    BigResponse obj(CREATE, "name6", DDLdata(Table("name6", fields), ""), DMLdata(), DQLdata(), err);
//    TestUtils::compareTables(obj, res);
//}
TEST(CREATE_TEST, TEST8) {
    std::string str = "CREATE TABLE name7(col1 CHAR UNIQUE, col2 INT NOT NULL, col3 CHAR PRIMARY KEY); ";
    res = parse_request(str.c_str());
    std::vector<std::pair<std::string, Variable>> fields = {{"col1", Variable(CHAR, std::vector<Constraint>{UNIQUE})},
                                                            {"col2", Variable(INT, std::vector<Constraint>{NOT_NULL})},
                                                            {"col3",
                                                             Variable(CHAR, std::vector<Constraint>{PRIMARY_KEY})}};
    Error err;
    BigResponse obj(CREATE, "name7", DDLdata(Table("name7", fields), ""), DMLdata(), DQLdata(), err);
    TestUtils::compareTables(obj, res);
}
TEST(CREATE_TEST, TEST9) {
    std::string str = "CREATE TABLE name8(col1 CHAR UNIQUE, col2 CHAR UNIQUE, col3 CHAR UNIQUE, col4 CHAR UNIQUE); ";
    res = parse_request(str.c_str());
    std::vector<std::pair<std::string, Variable>> fields = {{"col1", Variable(CHAR, std::vector<Constraint>{UNIQUE})},
                                                            {"col2", Variable(CHAR, std::vector<Constraint>{UNIQUE})},
                                                            {"col3", Variable(CHAR, std::vector<Constraint>{UNIQUE})},
                                                            {"col4", Variable(CHAR, std::vector<Constraint>{UNIQUE})}};
    Error err;
    BigResponse obj(CREATE, "name8", DDLdata(Table("name8", fields), ""), DMLdata(), DQLdata(), err);
    TestUtils::compareTables(obj, res);
}

TEST(SYNTAX_ERROR_TEST, TEST1) {
    std::string str = "";
    res = parse_request(str.c_str());
    EXPECT_EQ(1, res.error.getErrorCode());
    EXPECT_EQ("syntax error, unexpected $end (Str num 1, sym num 0): ", res.error.getErrorMsg());
}

TEST(SYNTAX_ERROR_TEST, TEST2) {
    std::string str = "c";
    res = parse_request(str.c_str());
    EXPECT_EQ(1, res.error.getErrorCode());
    EXPECT_EQ("syntax error, unexpected STRING (Str num 1, sym num 1): c", res.error.getErrorMsg());
}

TEST(SYNTAX_ERROR_TEST, TEST3) {
    std::string str = "CREAT TABLE name1(ID INT);";
    res = parse_request(str.c_str());
    EXPECT_EQ(1, res.error.getErrorCode());
    EXPECT_EQ("syntax error, unexpected STRING (Str num 1, sym num 5): CREAT", res.error.getErrorMsg());
}

TEST(SYNTAX_ERROR_TEST, TEST4) {
    std::string str = "CREATE TABLE name1(1565 INT);";
    res = parse_request(str.c_str());
    EXPECT_EQ(1, res.error.getErrorCode());
    EXPECT_EQ("syntax error, unexpected NUMBER, expecting STRING (Str num 1, sym num 21): 1565",
              res.error.getErrorMsg());
}

TEST(SYNTAX_ERROR_TEST, TEST5) {
    std::string str = "CREATE TABLE name1(ID IN);";
    res = parse_request(str.c_str());
    EXPECT_EQ(1, res.error.getErrorCode());
    EXPECT_EQ("syntax error, unexpected STRING, expecting TYPE (Str num 1, sym num 21): IN", res.error.getErrorMsg());
}

TEST(SYNTAX_ERROR_TEST, TEST6) {
    std::string str = "CREATE TABLE name1(ID INT NOT NU);";
    res = parse_request(str.c_str());
    EXPECT_EQ(1, res.error.getErrorCode());
    EXPECT_EQ("syntax error, unexpected STRING, expecting COMMA or BRACKET or CONSTRAINT (Str num 1, sym num 25): NOT",
              res.error.getErrorMsg());
}

TEST(SYNTAX_ERROR_TEST, TEST7) {
    std::string str = "CREATE TABLE name1( );";
    res = parse_request(str.c_str());
    EXPECT_EQ(1, res.error.getErrorCode());
    EXPECT_EQ("syntax error, unexpected BRACKET, expecting STRING (Str num 1, sym num 18): )", res.error.getErrorMsg());
}

TEST(SYNTAX_ERROR_TEST, TEST8) {
    std::string str = "inser into y values ('s');";
    res = parse_request(str.c_str());
    EXPECT_EQ(1, res.error.getErrorCode());
    EXPECT_EQ("syntax error, unexpected STRING (Str num 1, sym num 5): inser", res.error.getErrorMsg());
}

TEST(SYNTAX_ERROR_TEST, TEST9) {
    std::string str = "insert ino y values('s');";
    res = parse_request(str.c_str());
    EXPECT_EQ(1, res.error.getErrorCode());
    EXPECT_EQ("syntax error, unexpected STRING (Str num 1, sym num 6): insert", res.error.getErrorMsg());
}

TEST(SYNTAX_ERROR_TEST, TEST10) {
    std::string str = "insert into t vles('asas');";
    res = parse_request(str.c_str());
    EXPECT_EQ(1, res.error.getErrorCode());
    EXPECT_EQ("syntax error, unexpected STRING, expecting BRACKET or VALUES (Str num 1, sym num 16): vles", res.error.getErrorMsg());
}

TEST(SYNTAX_ERROR_TEST, TEST11) {
    std::string str = "insert into t ('asas');";
    res = parse_request(str.c_str());
    EXPECT_EQ(1, res.error.getErrorCode());
    EXPECT_EQ("syntax error, unexpected STROKE, expecting STRING (Str num 1, sym num 14): '", res.error.getErrorMsg());
}

TEST(SYNTAX_ERROR_TEST, TEST12) {
    std::string str = "insert into t values(asas);";
    res = parse_request(str.c_str());
    EXPECT_EQ(1, res.error.getErrorCode());
    EXPECT_EQ("syntax error, unexpected STRING, expecting SEMICOLON (Str num 1, sym num 23): asas", res.error.getErrorMsg());
}

TEST(SYNTAX_ERROR_TEST, TEST13) {
    std::string str = "insert into values(asas);";
    res = parse_request(str.c_str());
    EXPECT_EQ(1, res.error.getErrorCode());
    EXPECT_EQ("syntax error, unexpected VALUES, expecting STRING (Str num 1, sym num 17): values", res.error.getErrorMsg());
}

TEST(SYNTAX_ERROR_TEST, TEST14) {
    std::string str = "insert into kkkk values ('asas', 3.14)";
    res = parse_request(str.c_str());
    EXPECT_EQ(1, res.error.getErrorCode());
    EXPECT_EQ("syntax error, unexpected $end, expecting SEMICOLON (Str num 1, sym num 34): ", res.error.getErrorMsg());
}

TEST(SYNTAX_ERROR_TEST, TEST15) {
    std::string str = "insert into kkkk values ('asas', 3.14) whre a = 'd';";
    res = parse_request(str.c_str());
    EXPECT_EQ(1, res.error.getErrorCode());
    EXPECT_EQ("syntax error, unexpected STRING, expecting SEMICOLON (Str num 1, sym num 38): whre", res.error.getErrorMsg());
}

TEST(SYNTAX_ERROR_TEST, TEST16) {
    std::string str = "insert into kkkk values ('asas', 3.14) where 'a' = 'd';";
    res = parse_request(str.c_str());
    EXPECT_EQ(1, res.error.getErrorCode());
    EXPECT_EQ("syntax error, unexpected STROKE, expecting STRING (Str num 1, sym num 40): '", res.error.getErrorMsg());
}

TEST(SYNTAX_ERROR_TEST, TEST17) {
    std::string str = "insert into kkkk values ('asas', 3.14) where a ? 'd';";
    res = parse_request(str.c_str());
    EXPECT_EQ(1, res.error.getErrorCode());
    EXPECT_EQ("syntax error, unexpected OTHER, expecting EQUALLY or SIGN (Str num 1, sym num 41): ?", res.error.getErrorMsg());
}

TEST(SYNTAX_ERROR_TEST, TEST18) {
    std::string str = "delte from a;";
    res = parse_request(str.c_str());
    EXPECT_EQ(1, res.error.getErrorCode());
    EXPECT_EQ("syntax error, unexpected STRING (Str num 1, sym num 5): delte", res.error.getErrorMsg());
}

TEST(SYNTAX_ERROR_TEST, TEST19) {
    std::string str = "delete fro a;";
    res = parse_request(str.c_str());
    EXPECT_EQ(1, res.error.getErrorCode());
    EXPECT_EQ("syntax error, unexpected STRING (Str num 1, sym num 6): delete", res.error.getErrorMsg());
}

TEST(SYNTAX_ERROR_TEST, TEST20) {
    std::string str = "delete from 'asdas';";
    res = parse_request(str.c_str());
    EXPECT_EQ(1, res.error.getErrorCode());
    EXPECT_EQ("syntax error, unexpected STROKE, expecting STRING (Str num 1, sym num 12): '", res.error.getErrorMsg());
}

TEST(SYNTAX_ERROR_TEST, TEST21) {
    std::string str = "delete from 1;";
    res = parse_request(str.c_str());
    EXPECT_EQ(1, res.error.getErrorCode());
    EXPECT_EQ("syntax error, unexpected NUMBER, expecting STRING (Str num 1, sym num 12): 1", res.error.getErrorMsg());
}

TEST(SYNTAX_ERROR_TEST, TEST22) {
    std::string str = "delete from t (where a = b);";
    res = parse_request(str.c_str());
    EXPECT_EQ(1, res.error.getErrorCode());
    EXPECT_EQ("syntax error, unexpected BRACKET, expecting SEMICOLON (Str num 1, sym num 13): (", res.error.getErrorMsg());
}

TEST(SYNTAX_ERROR_TEST, TEST23) {
    std::string str = "upate t set a = '5';";
    res = parse_request(str.c_str());
    EXPECT_EQ(1, res.error.getErrorCode());
    EXPECT_EQ("syntax error, unexpected STRING (Str num 1, sym num 5): upate", res.error.getErrorMsg());
}

TEST(SYNTAX_ERROR_TEST, TEST24) {
    std::string str = "update 't' set a = '5';";
    res = parse_request(str.c_str());
    EXPECT_EQ(1, res.error.getErrorCode());
    EXPECT_EQ("syntax error, unexpected STROKE, expecting STRING (Str num 1, sym num 7): '", res.error.getErrorMsg());
}

TEST(SYNTAX_ERROR_TEST, TEST25) {
    std::string str = "update t se a = '5';";
    res = parse_request(str.c_str());
    EXPECT_EQ(1, res.error.getErrorCode());
    EXPECT_EQ("syntax error, unexpected STRING, expecting SET (Str num 1, sym num 9): se", res.error.getErrorMsg());
}

TEST(SYNTAX_ERROR_TEST, TEST26) {
    std::string str = "update t set 'a' = 's';";
    res = parse_request(str.c_str());
    EXPECT_EQ(1, res.error.getErrorCode());
    EXPECT_EQ("syntax error, unexpected STROKE, expecting STRING (Str num 1, sym num 11): '", res.error.getErrorMsg());
}

TEST(SYNTAX_ERROR_TEST, TEST27) {
    std::string str = "update t set 5 = 's';";
    res = parse_request(str.c_str());
    EXPECT_EQ(1, res.error.getErrorCode());
    EXPECT_EQ("syntax error, unexpected NUMBER, expecting STRING (Str num 1, sym num 11): 5", res.error.getErrorMsg());
}

TEST(SYNTAX_ERROR_TEST, TEST28) {
    std::string str = "update t set a 's';";
    res = parse_request(str.c_str());
    EXPECT_EQ(1, res.error.getErrorCode());
    EXPECT_EQ("syntax error, unexpected STROKE, expecting EQUALLY (Str num 1, sym num 12): '", res.error.getErrorMsg());
}

TEST(SYNTAX_ERROR_TEST, TEST29) {
    std::string str = "update t set a = ;";
    res = parse_request(str.c_str());
    EXPECT_EQ(1, res.error.getErrorCode());
    EXPECT_EQ("syntax error, unexpected SEMICOLON, expecting NUMBER or STROKE or VALNULL or FLOATNUM (Str num 1, sym num 13): ;", res.error.getErrorMsg());
}

TEST(SYNTAX_ERROR_TEST, TEST30) {
    std::string str = "update t set a = 5 whee a = b;";
    res = parse_request(str.c_str());
    EXPECT_EQ(1, res.error.getErrorCode());
    EXPECT_EQ("syntax error, unexpected STRING, expecting SEMICOLON (Str num 1, sym num 17): whee", res.error.getErrorMsg());
}

TEST(SYNTAX_ERROR_TEST, TEST31) {
    std::string str = "update t set a = 5 where a  b;";
    res = parse_request(str.c_str());
    EXPECT_EQ(1, res.error.getErrorCode());
    EXPECT_EQ("syntax error, unexpected STRING, expecting EQUALLY or SIGN (Str num 1, sym num 20): b", res.error.getErrorMsg());
}

TEST(SYNTAX_ERROR_TEST, TEST32) {
    std::string str = "selec * from t;";
    res = parse_request(str.c_str());
    EXPECT_EQ(1, res.error.getErrorCode());
    EXPECT_EQ("syntax error, unexpected STRING (Str num 1, sym num 5): selec", res.error.getErrorMsg());
}

TEST(SYNTAX_ERROR_TEST, TEST33) {
    std::string str = "select ** from t;";
    res = parse_request(str.c_str());
    EXPECT_EQ(1, res.error.getErrorCode());
    EXPECT_EQ("syntax error, unexpected OTHER, expecting STRING or ALL (Str num 1, sym num 8): **", res.error.getErrorMsg());
}

TEST(SYNTAX_ERROR_TEST, TEST34) {
    std::string str = "select 'sdf' from t;";
    res = parse_request(str.c_str());
    EXPECT_EQ(1, res.error.getErrorCode());
    EXPECT_EQ("syntax error, unexpected STROKE, expecting STRING or ALL (Str num 1, sym num 7): '", res.error.getErrorMsg());
}

TEST(SYNTAX_ERROR_TEST, TEST35) {
    std::string str = "select sdf fsdd from t;";
    res = parse_request(str.c_str());
    EXPECT_EQ(1, res.error.getErrorCode());
    EXPECT_EQ("syntax error, unexpected STRING, expecting COMMA or FROM (Str num 1, sym num 13): fsdd", res.error.getErrorMsg());
}

TEST(SYNTAX_ERROR_TEST, TEST36) {
    std::string str = "select sdf frm t;";
    res = parse_request(str.c_str());
    EXPECT_EQ(1, res.error.getErrorCode());
    EXPECT_EQ("syntax error, unexpected STRING, expecting COMMA or FROM (Str num 1, sym num 12): frm", res.error.getErrorMsg());
}

TEST(SYNTAX_ERROR_TEST, TEST37) {
    std::string str = "select * from 't';";
    res = parse_request(str.c_str());
    EXPECT_EQ(1, res.error.getErrorCode());
    EXPECT_EQ("syntax error, unexpected STROKE, expecting STRING (Str num 1, sym num 12): '", res.error.getErrorMsg());
}

TEST(SYNTAX_ERROR_TEST, TEST38) {
    std::string str = "select * from 5;";
    res = parse_request(str.c_str());
    EXPECT_EQ(1, res.error.getErrorCode());
    EXPECT_EQ("syntax error, unexpected NUMBER, expecting STRING (Str num 1, sym num 12): 5", res.error.getErrorMsg());
}

TEST(SYNTAX_ERROR_TEST, TEST39) {
    std::string str = "select * from ttt whre a = 5;";
    res = parse_request(str.c_str());
    EXPECT_EQ(1, res.error.getErrorCode());
    EXPECT_EQ("syntax error, unexpected STRING, expecting SEMICOLON (Str num 1, sym num 18): whre", res.error.getErrorMsg());
}

TEST(SYNTAX_ERROR_TEST, TEST40) {
    std::string str = "select * from ttt where 'a' = 5;";
    res = parse_request(str.c_str());
    EXPECT_EQ(1, res.error.getErrorCode());
    EXPECT_EQ("syntax error, unexpected STROKE, expecting STRING (Str num 1, sym num 20): '", res.error.getErrorMsg());
}

TEST(SYNTAX_ERROR_TEST, TEST41) {
    std::string str = "select * from ttt where a  5;";
    res = parse_request(str.c_str());
    EXPECT_EQ(1, res.error.getErrorCode());
    EXPECT_EQ("syntax error, unexpected NUMBER, expecting EQUALLY or SIGN (Str num 1, sym num 21): 5", res.error.getErrorMsg());
}

TEST(SYNTAX_ERROR_TEST, TEST42) {
    std::string str = "dro table ddd;";
    res = parse_request(str.c_str());
    EXPECT_EQ(1, res.error.getErrorCode());
    EXPECT_EQ("syntax error, unexpected STRING (Str num 1, sym num 3): dro", res.error.getErrorMsg());
}

TEST(SYNTAX_ERROR_TEST, TEST43) {
    std::string str = "drop tale ddd;";
    res = parse_request(str.c_str());
    EXPECT_EQ(1, res.error.getErrorCode());
    EXPECT_EQ("syntax error, unexpected STRING, expecting TABLE (Str num 1, sym num 8): tale", res.error.getErrorMsg());
}

TEST(SYNTAX_ERROR_TEST, TEST44) {
    std::string str = "drop table 15;";
    res = parse_request(str.c_str());
    EXPECT_EQ(1, res.error.getErrorCode());
    EXPECT_EQ("syntax error, unexpected NUMBER, expecting STRING (Str num 1, sym num 11): 15", res.error.getErrorMsg());
}

TEST(SYNTAX_ERROR_TEST, TEST45) {
    std::string str = "drop table asd, asd;";
    res = parse_request(str.c_str());
    EXPECT_EQ(1, res.error.getErrorCode());
    EXPECT_EQ("syntax error, unexpected COMMA, expecting SEMICOLON (Str num 1, sym num 13): ,", res.error.getErrorMsg());
}

TEST(SYNTAX_ERROR_TEST, TEST46) {
    std::string str = "sho create table h;";
    res = parse_request(str.c_str());
    EXPECT_EQ(1, res.error.getErrorCode());
    EXPECT_EQ("syntax error, unexpected STRING (Str num 1, sym num 3): sho", res.error.getErrorMsg());
}

TEST(SYNTAX_ERROR_TEST, TEST47) {
    std::string str = "show crate table h;";
    res = parse_request(str.c_str());
    EXPECT_EQ(1, res.error.getErrorCode());
    EXPECT_EQ("syntax error, unexpected STRING (Str num 1, sym num 4): show", res.error.getErrorMsg());
}

TEST(SYNTAX_ERROR_TEST, TEST48) {
    std::string str = "show create tale h;";
    res = parse_request(str.c_str());
    EXPECT_EQ(1, res.error.getErrorCode());
    EXPECT_EQ("syntax error, unexpected STRING, expecting TABLE (Str num 1, sym num 15): tale", res.error.getErrorMsg());
}

TEST(SYNTAX_ERROR_TEST, TEST49) {
    std::string str = "show create table 5;";
    res = parse_request(str.c_str());
    EXPECT_EQ(1, res.error.getErrorCode());
    EXPECT_EQ("syntax error, unexpected NUMBER, expecting STRING (Str num 1, sym num 17): 5", res.error.getErrorMsg());
}

TEST(SYNTAX_ERROR_TEST, TEST50) {
    std::string str = "show create table aa, df;";
    res = parse_request(str.c_str());
    EXPECT_EQ(1, res.error.getErrorCode());
    EXPECT_EQ("syntax error, unexpected COMMA, expecting SEMICOLON (Str num 1, sym num 19): ,", res.error.getErrorMsg());
}

TEST(ERROR_TEST, TEST1) {
    std::string str = "CREATE TABLE name1(ID INT);";
    res = parse_request(str.c_str());
    res = MainLogic::executeRequest(res);
    str = "CREATE TABLE name1(ID INT);";
    res = parse_request(str.c_str());
    res = MainLogic::executeRequest(res);
    EXPECT_EQ(1, res.error.getErrorCode());
    EXPECT_EQ(ErrorConstants::ERR_TABLE_EXISTS_str, res.error.getErrorMsg());
    str = "DROP TABLE name1;";
    res = parse_request(str.c_str());
    MainLogic::executeRequest(res);
}

TEST(SHOW_CREATE, TEST1) {
    std::string str = "CREATE TABLE name2(ID INT);";
    res = parse_request(str.c_str());
    MainLogic::executeRequest(res);
    str = "SHOW CREATE TABLE name2;";
    res = parse_request(str.c_str());
    res = MainLogic::executeRequest(res);
    EXPECT_EQ(0, res.error.getErrorCode());
    EXPECT_EQ("CREATE TABLE name2(ID INT );", res.ddlData.returnMsg);
    str = "DROP TABLE name2;";
    res = parse_request(str.c_str());
    MainLogic::executeRequest(res);
}

TEST(SHOW_CREATE, TEST2) {
    str = "CREATE TABLE name3(ID INT PRIMARY KEY);";
    res = parse_request(str.c_str());
    MainLogic::executeRequest(res);
    str = "SHOW CREATE TABLE name3;";
    res = parse_request(str.c_str());
    res = MainLogic::executeRequest(res);
    EXPECT_EQ(0, res.error.getErrorCode());
    str = "DROP TABLE name3;";
    BigResponse result = parse_request(str.c_str());
    result = MainLogic::executeRequest(result);
    EXPECT_EQ(0, result.error.getErrorCode());
    result = parse_request(res.ddlData.returnMsg.c_str());
    result = MainLogic::executeRequest(result);
    EXPECT_EQ(0, result.error.getErrorCode());
    // EXPECT_EQ(res.ddlData.table.name, result.ddlData.table.name);
}

TEST(INSERT_TEST, TEST1) {
    std::vector<std::pair<std::string, Variable>> fields = {{"id", Variable(CHAR, std::vector<Constraint>{UNIQUE})},
                                                            {"count", Variable(CHAR, std::vector<Constraint>{UNIQUE})}};
    std::vector<std::string> _values = {"4", "5"};
    std::vector<std::string> _columns;
    std::map<std::string, Condition> _conditions;

    Error err;
    BigResponse obj(INSERT, "tname", DDLdata(Table("tname", fields), ""), DMLdata(_columns, _values, _conditions),
                    DQLdata(), err);
    str = "CREATE TABLE tname(id INT, count INT);";
    res = parse_request(str.c_str());
    MainLogic::executeRequest(res);
    str = "INSERT INTO tname VALUES(4,5)";
    res = parse_request(str.c_str());
    res = MainLogic::executeRequest(res);
    EXPECT_EQ(0, res.error.getErrorCode());
    TestUtils::compareDml(obj, res);
    str = "DROP TABLE tname;";
    res = parse_request(str.c_str());
    MainLogic::executeRequest(res);
}