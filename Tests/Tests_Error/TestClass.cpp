#include <gtest/gtest.h>
#include "../../Client/Client.h"
#include "../Headers/TestUtils.h"
#include "../RQG/Generator.h"

TEST(SERVER_TEST_ERROR, ERROR_TEST_CREATE_EXISTING_TABLE) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE i(id INT);", "Success"},
                              {"CREATE TABLE i(id INT);", "Table already exists ERROR: 1"}});
    TestUtils::clear();
}

TEST(SERVER_TEST_ERROR, ERROR_TEST_CREATE_WITH_REPEATED_CONSTRAINTS) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE asa(id INT UNIQUE UNIQUE, id1 FLOAT NOT NULL);",
                               "Constraint already exists ERROR: 3"}});
}

TEST(SERVER_TEST_ERROR, ERROR_TEST_CREATE_WITH_SAME_COLUMN_NAMES) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE ac(id INT UNIQUE, id FLOAT NOT NULL);",
                               "Field name already used ERROR: 4"}});
}

TEST(SERVER_TEST_ERROR, ERROR_TEST_INSERT_NULL_IN_NOT_NULL) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE m(id INT UNIQUE, di FLOAT NOT NULL);", "Success"},
                              {"insert into m values(1, 2);", "Success"},
                              {"insert into m values(null, null);", "Null values unavailable ERROR: 8"}});
}

TEST(SERVER_TEST_ERROR, ERROR_TEST_UPDATE_SET_NULL_IN_NOT_NULL) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE mm(id INT UNIQUE, di FLOAT NOT NULL);", "Success"},
                              {"insert into mm values(1, 2);", "Success"},
                              {"insert into mm values(3, 4);", "Success"},
                              {"update mm set di = null, id = 3;", "Null values unavailable ERROR: 8"}});
}

TEST(SERVER_TEST_ERROR, ERROR_TEST_INSERT_IN_A_NO_EXISTENT_TABLE) {
    TestUtils::clear();
    TestUtils::checkRequests({{"insert into qqq values(1, 2);", "Table doesn`t exist ERROR: 2"}});
}

TEST(SERVER_TEST_ERROR, ERROR_TEST_INSERT_INVALID_COUNT_OF_COLUMNS) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE ac(id INT UNIQUE, di FLOAT NOT NULL);", "Success"},
                              {"insert into ac(id) values(1, 2);", "Invalid count of columns and values ERROR: 5"}});
}

TEST(SERVER_TEST_ERROR, ERROR_TEST_INSERT_EXISTING_VALUE_IN_UNIQUE_COLUMN) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE k(id INT UNIQUE);", "Success"},
                              {"insert into k values(1);", "Success"},
                              {"insert into k values(2);", "Success"},
                              {"insert into k values(1);", "Value already exists, use UNIQUE value ERROR: 9"}});
}

TEST(SERVER_TEST_ERROR, ERROR_TEST_INSERT_STRING_IN_INT) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE j(id INT);", "Success"},
                              {"insert into j values('sdfsdf');",
                               "Value is out of range OR is not a number ERROR: 12"}});
}

TEST(SERVER_TEST_ERROR, ERROR_TEST_INSERT_NULL_IN_NOT_NULL_IN_COLUMN) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE asas(id INT UNIQUE, id1 FLOAT NOT NULL);", "Success"},
                              {"insert into asas(id) values(1);", "Null values unavailable ERROR: 8"}});
}

TEST(SERVER_TEST_ERROR, ERROR_TEST_INSERT_IN_NO_EXISTENT_COLUMN) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE asas(id INT UNIQUE, id1 FLOAT);", "Success"},
                              {"insert into asas(isasfdasf, id) values(1, 2);", "Field doesnt exist ERROR: 6"}});
}

TEST(SERVER_TEST_ERROR, ERROR_TEST_INSERT_IN_NO_EXISTENT_TABLE) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE asas(id INT UNIQUE, id1 FLOAT);", "Success"},
                              {"DROP TABLE asas;", "Success"},
                              {"insert into asas values (1, 5.5);", "Table doesn`t exist ERROR: 2"}});
}

TEST(SERVER_TEST_ERROR, ERROR_TEST_INSERT_OVERSIZE_INT) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE j(id INT);", "Success"},
                              {"insert into j values(159753159753);",
                               "Int oversize (Str num 1, sym num 30): 159753159753"}});
}

TEST(SERVER_TEST_ERROR, ERROR_TEST_INSERT_IN_SAME_COLUMNS) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE qw(id INT UNIQUE, id1 FLOAT);", "Success"},
                              {"insert into qw(id, id) values (1, 3);", "Same column in request ERROR: 7"}});
}

TEST(SERVER_TEST_ERROR, ERROR_TEST_INSERT_OVERSIZE_CHAR) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE qe(id CHAR(10));", "Success"},
                              {"insert into qe values ('qwertyuiopqwertyuio');", "CHAR IS VERY BIG ERROR: 13"}});
}

TEST(SERVER_TEST_ERROR, ERROR_TEST_INSERT_OVERSIZE_CHAR_256) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE qh(id CHAR(256));", "Success"},
                              {"insert into qh values "
                               "('qqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqq"
                               "qqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqq"
                               "qqqq"
                               "q"
                               "qqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqq"
                               "qqqq"
                               "q"
                               "qqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqq');",
                               "CHAR IS VERY BIG ERROR: 13"}});
}

TEST(SERVER_TEST_ERROR, ERROR_TEST_INSERT_OVERSIZE_CHAR_1) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE qh(id CHAR(1));", "Success"},
                              {"insert into qh values ('');", "Success"},
                              {"insert into qh values ('a');", "Success"},
                              {"insert into qh values ('aa');", "CHAR IS VERY BIG ERROR: 13"}});
}

TEST(SERVER_TEST_ERROR, ERROR_TEST_INSERT_INVALID_COUNT_OF_COLUMNS_ALL_COLUMNS) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE fn(id INT NOT NULL , age float, name char(150));", "Success"},
                              {"INSERT INTO fn values(1, 2.9);", "Invalid count of columns and values ERROR: 5"}});
}

TEST(SERVER_TEST_ERROR, ERROR_TEST_INSERT_STRING_IN_FLOAT) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE fn(age float);", "Success"},
                              {"INSERT INTO fn values('dfgdg');",
                               "Value is out of range OR is not a number ERROR: 12"}});
}

TEST(SERVER_TEST_ERROR, ERROR_TEST_UPDATE_SET_DUPLICATE_VALUES_IN_UNIQUE_COLUMN_WITH_WHERE) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE qg(id int unique);", "Success"},
                              {"insert into qg values (1);", "Success"},
                              {"insert into qg values (2);", "Success"},
                              {"update qg set id = 2 where id = 1;",
                               "Value already exists, use UNIQUE value ERROR: 9"}});
}

TEST(SERVER_TEST_ERROR, ERROR_TEST_UPDATE_SET_DUPLICATE_VALUES_IN_ALL_UNIQUE_COLUMN) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE qg(id int unique);", "Success"},
                              {"insert into qg values (1);", "Success"},
                              {"insert into qg values (2);", "Success"},
                              {"update qg set id = 3;", "Value already exists, use UNIQUE value ERROR: 9"}});
}

TEST(SERVER_TEST_ERROR, ERROR_TEST_UPDATE_SET_DUPLICATE_VALUES_IN_UNIQUE_COLUMN) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE qg(id int unique);", "Success"},
                              {"insert into qg values (1);", "Success"},
                              {"insert into qg values (2);", "Success"},
                              {"update qg set id = 2;", "Value already exists, use UNIQUE value ERROR: 9"}});
}

TEST(SERVER_TEST_ERROR, ERROR_TEST_SELECT_WHERE_TYPES_MISMATCH) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE qg(id int unique, name char(50));", "Success"},
                              {"insert into qg values (1, 'qwe');", "Success"},
                              {"insert into qg values (2, 'ewq');", "Success"},
                              {"select * from qg where id = name + 1;", "Types mismatch ERROR: 11"}});
}

TEST(SERVER_TEST_ERROR, ERROR_TEST_SELECT_JOIN_WITH_NOT_EXISTING_TABLE) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE fn(id INT NOT NULL , age float, name char(150));", "Success"},
                              {"INSERT INTO fn values(1, 2.9, 'sfsf');", "Success"},
                              {"INSERT INTO fn values(2, 3.789, 'qwerty');", "Success"},
                              {"SELECT * from fn join nf on fn.id = nf.id;", "Table doesn`t exist ERROR: 2"}});
}

TEST(SERVER_TEST_ERROR, ERROR_TEST_SELECT_JOIN_WITH_NOT_EXISTING_COLUMN) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE fn(id INT NOT NULL , age float, name char(150));", "Success"},
                              {"INSERT INTO fn values(1, 2.9, 'sfsf');", "Success"},
                              {"INSERT INTO fn values(2, 3.789, 'qwerty');", "Success"},
                              {"SELECT * from fn as f1 join fn on f1.id = fn.id11;", "Field doesnt exist ERROR: 6"}});
}

TEST(SERVER_TEST_ERROR, ERROR_TEST_SELECT_JOIN_WITH_NOT_EXISTING_ALIAS) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE fn(id INT NOT NULL , age float, name char(150));", "Success"},
                              {"INSERT INTO fn values(1, 2.9, 'sfsf');", "Success"},
                              {"INSERT INTO fn values(2, 3.789, 'qwerty');", "Success"},
                              {"SELECT * from fn join fn on f1.id = fn.id;", "Field doesnt exist ERROR: 6"}});
}

TEST(SERVER_TEST_ERROR, ERROR_TEST_SELECT_NOT_EXISTING_COLUMN) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE fn(id INT NOT NULL , age float, name char(150));", "Success"},
                              {"INSERT INTO fn values(1, 2.9, 'sfsf');", "Success"},
                              {"SELECT dgdfg, id from fn;", "Field doesnt exist ERROR: 6"}});
}

TEST(SERVER_TEST_ERROR, ERROR_TEST_SELECT_UNION_WITH_DIFFERENT_COLUMNS) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE table1(id INT NOT NULL, age float, name char(150));", "Success"},
                              {"CREATE TABLE table2(id INT NOT NULL, age float);", "Success"},
                              {"INSERT INTO table1 values(1, 2.9, 'sfsf');", "Success"},
                              {"INSERT INTO table2 values(1, 3.5);", "Success"},
                              {"SELECT age from table1 UNION SELECT id from table2;", "Field doesnt exist ERROR: 6"}});
}

TEST(SERVER_TEST_ERROR, ERROR_TEST_SELECT_INTERSECT_WITH_DIFFERENT_COLUMNS) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE table1(id INT NOT NULL, age float, name char(150));", "Success"},
                              {"CREATE TABLE table2(id INT NOT NULL, age float);", "Success"},
                              {"INSERT INTO table1 values(1, 2.9, 'sfsf');", "Success"},
                              {"INSERT INTO table2 values(1, 3.5);", "Success"},
                              {"SELECT age from table1 INTERSECT SELECT id from table2;",
                               "Field doesnt exist ERROR: 6"}});
}

TEST(SERVER_TEST_ERROR, ERROR_TEST_SELECT_ALL_UNION_WITH_DIFFERENT_COLUMNS) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE table1(id INT NOT NULL, age float, name char(150));", "Success"},
                              {"CREATE TABLE table2(id INT NOT NULL, age float);", "Success"},
                              {"INSERT INTO table1 values(1, 2.9, 'sfsf');", "Success"},
                              {"INSERT INTO table2 values(1, 3.5);", "Success"},
                              {"SELECT * from table1 UNION SELECT * from table2;", "Field doesnt exist ERROR: 6"}});
}

TEST(SERVER_TEST_SYN_ERROR, SYN_ERROR_TEST_EMPTY_STRING) {
    TestUtils::clear();
    TestUtils::checkRequests({{"", "syntax error, unexpected $end (Str num 1, sym num 0): "}});
}

TEST(SERVER_TEST_SYN_ERROR, SYN_ERROR_TEST_ONE_SYMBOL) {
    TestUtils::clear();
    TestUtils::checkRequests({{"с", "syntax error, unexpected $end (Str num 1, sym num 0): "}});
}

TEST(SERVER_TEST_SYN_ERROR, SYN_ERROR_TEST_CREATE_WITHOUT_COLUMNS) {
    TestUtils::clear();
    TestUtils::checkRequests({{"create table t ();",
                               "syntax error, unexpected RBRACKET, expecting IDENT (Str num 1, sym num 14): )"}});
}

TEST(SERVER_TEST_SYN_ERROR, SYN_ERROR_TEST_ERROR_IN_CREATE) {
    TestUtils::clear();
    TestUtils::checkRequests({{"creat table t (id int);",
                               "syntax error, unexpected IDENT (Str num 1, sym num 5): creat"}});
}

TEST(SERVER_TEST_SYN_ERROR, SYN_ERROR_TEST_ERROR_IN_TABLE) {
    TestUtils::clear();
    TestUtils::checkRequests({{"create tale t (id int);",
                               "syntax error, unexpected IDENT, expecting TABLE (Str num 1, sym num 10): tale"}});
}

TEST(SERVER_TEST_SYN_ERROR, SYN_ERROR_TEST_ERROR_IN_TABLE_NAME) {
    TestUtils::clear();
    TestUtils::checkRequests({{"create table 555 (id int);",
                               "syntax error, unexpected NUMBER, expecting IDENT (Str num 1, sym num 14): 555"}});
}

TEST(SERVER_TEST_SYN_ERROR, SYN_ERROR_TEST_ERROR_IN_INT) {
    TestUtils::clear();
    TestUtils::checkRequests({{"create table t (id innt);",
                               "syntax error, unexpected IDENT, expecting INT_TYPE or FLOAT_TYPE or CHAR_TYPE (Str "
                               "num "
                               "1, sym num 19): innt"}});
}

TEST(SERVER_TEST_SYN_ERROR, SYN_ERROR_TEST_ERROR_IN_CHAR) {
    TestUtils::clear();
    TestUtils::checkRequests({{"create table t (id charrrr(50));",
                               "syntax error, unexpected IDENT, expecting INT_TYPE or FLOAT_TYPE or CHAR_TYPE (Str "
                               "num "
                               "1, sym num 22): charrrr"}});
}

TEST(SERVER_TEST_SYN_ERROR, SYN_ERROR_TEST_ERROR_IN_CHAR_LENGTH) {
    TestUtils::clear();
    TestUtils::checkRequests({{"create table t (name char(45454554525452452452452));",
                               "syntax error, unexpected $undefined, expecting INT_TYPE or FLOAT_TYPE or CHAR_TYPE "
                               "(Str num 1, sym num 46): char(45454554525452452452452)"}});
}

TEST(SERVER_TEST_SYN_ERROR, SYN_ERROR_TEST_ERROR_IN_CHAR_NUMBERS) {
    TestUtils::clear();
    TestUtils::checkRequests({{"create table t (name char54545(20));",
                               "syntax error, unexpected IDENT, expecting INT_TYPE or FLOAT_TYPE or CHAR_TYPE (Str "
                               "num "
                               "1, sym num 26): char54545"}});
}

TEST(SERVER_TEST_SYN_ERROR, SYN_ERROR_TEST_ERROR_IN_TYPE_OF_COLUMN) {
    TestUtils::clear();
    TestUtils::checkRequests({{"create table t (name 456456);",
                               "syntax error, unexpected NUMBER, expecting INT_TYPE or FLOAT_TYPE or CHAR_TYPE "
                               "(Str "
                               "num 1, sym num 23): 456456"}});
}

TEST(SERVER_TEST_SYN_ERROR, SYN_ERROR_TEST_ERROR_IN_CHAR_WITHOUT_LENGTH) {
    TestUtils::clear();
    TestUtils::checkRequests({{"create table t (name int, nn char);",
                               "syntax error, unexpected IDENT, expecting INT_TYPE or FLOAT_TYPE or CHAR_TYPE (Str "
                               "num "
                               "1, sym num 27): char"}});
}

TEST(SERVER_TEST_SYN_ERROR, SYN_ERROR_TEST_ERROR_MISSING_BRACKET) {
    TestUtils::clear();
    TestUtils::checkRequests({{"create table t (name int;",
                               "syntax error, unexpected SEMICOLON, expecting RBRACKET or COMMA (Str num 1, sym "
                               "num "
                               "21): ;"}});
}

TEST(SERVER_TEST_SYN_ERROR, SYN_ERROR_TEST_ERROR_IN_DROP) {
    TestUtils::clear();
    TestUtils::checkRequests({{"dro table t;", "syntax error, unexpected IDENT (Str num 1, sym num 3): dro"}});
}

TEST(SERVER_TEST_SYN_ERROR, SYN_ERROR_TEST_ERROR_IN_DROP_SEVERAL_TABLES) {
    TestUtils::clear();
    TestUtils::checkRequests({{"drop table t, y;",
                               "syntax error, unexpected COMMA, expecting SEMICOLON (Str num 1, sym num 11): ,"}});
}

TEST(SERVER_TEST_SYN_ERROR, SYN_ERROR_TEST_ERROR_IN_DROP_MISSING_SEMICOLON) {
    TestUtils::clear();
    TestUtils::checkRequests({{"drop table t",
                               "syntax error, unexpected $end, expecting SEMICOLON (Str num 1, sym num 10): "}});
}

TEST(SERVER_TEST_SYN_ERROR, SYN_ERROR_TEST_ERROR_IN_SHOW_CREATE_SHOW) {
    TestUtils::clear();
    TestUtils::checkRequests({{"shw create table t;", "syntax error, unexpected IDENT (Str num 1, sym num 3): shw"}});
}

TEST(SERVER_TEST_SYN_ERROR, SYN_ERROR_TEST_ERROR_IN_SHOW_CREATE_CREATE) {
    TestUtils::clear();
    TestUtils::checkRequests({{"show crate table t;",
                               "syntax error, unexpected IDENT, expecting CREATE_ACTION (Str num 1, sym num 9): "
                               "crate"}});
}

TEST(SERVER_TEST_SYN_ERROR, SYN_ERROR_TEST_ERROR_IN_SHOW_CREATE_UNNECESSARY_BRACKETS) {
    TestUtils::clear();
    TestUtils::checkRequests({{"show create table (t);",
                               "syntax error, unexpected LBRACKET, expecting IDENT (Str num 1, sym num 16): ("}});
}

TEST(SERVER_TEST_SYN_ERROR, SYN_ERROR_TEST_ERROR_IN_INSERT_COMMA_IN_TABLE_NAME) {
    TestUtils::clear();
    TestUtils::checkRequests({{"insert into 'tt' values(1);",
                               "syntax error, unexpected STRVAL, expecting IDENT (Str num 1, sym num 14): 'tt'"}});
}

TEST(SERVER_TEST_SYN_ERROR, SYN_ERROR_TEST_ERROR_IN_INSERT_INSERT) {
    TestUtils::clear();
    TestUtils::checkRequests({{"inser into t values(1);",
                               "syntax error, unexpected IDENT (Str num 1, sym num 5): inser"}});
}

TEST(SERVER_TEST_SYN_ERROR, SYN_ERROR_TEST_ERROR_IN_INSERT_INTO) {
    TestUtils::clear();
    TestUtils::checkRequests({{"insert ito t values(1);",
                               "syntax error, unexpected IDENT, expecting INTO (Str num 1, sym num 9): ito"}});
}

TEST(SERVER_TEST_SYN_ERROR, SYN_ERROR_TEST_ERROR_IN_INSERT_NUMBER_IN_COLUMN_NAME) {
    TestUtils::clear();
    TestUtils::checkRequests({{"insert into t(1);",
                               "syntax error, unexpected NUMBER, expecting IDENT (Str num 1, sym num 13): 1"}});
}

TEST(SERVER_TEST_SYN_ERROR, SYN_ERROR_TEST_ERROR_IN_INSERT_VALUES) {
    TestUtils::clear();
    TestUtils::checkRequests({{"insert into t(col) valus;",
                               "syntax error, unexpected IDENT, expecting VALUES (Str num 1, sym num 21): valus"}});
}

TEST(SERVER_TEST_SYN_ERROR, SYN_ERROR_TEST_ERROR_IN_INSERT_MISSING_VALUES) {
    TestUtils::clear();
    TestUtils::checkRequests({{"insert into t(col) (5);",
                               "syntax error, unexpected LBRACKET, expecting VALUES (Str num 1, sym num 17): ("}});
}

TEST(SERVER_TEST_SYN_ERROR, SYN_ERROR_TEST_ERROR_IN_INSERT_MISSING_BRACKET_WITH_VALUES) {
    TestUtils::clear();
    TestUtils::checkRequests({{"insert into t values;",
                               "syntax error, unexpected SEMICOLON, expecting LBRACKET (Str num 1, sym num 18): "
                               ";"}});
}

TEST(SERVER_TEST_SYN_ERROR, SYN_ERROR_TEST_ERROR_IN_INSERT_EMPTY_VALUES) {
    TestUtils::clear();
    TestUtils::checkRequests({{"insert into t values();",
                               "syntax error, unexpected RBRACKET (Str num 1, sym num 19): )"}});
}

TEST(SERVER_TEST_SYN_ERROR, SYN_ERROR_TEST_ERROR_IN_INSERT_MISSING_INTO) {
    TestUtils::clear();
    TestUtils::checkRequests({{"insert t values(7);",
                               "syntax error, unexpected IDENT, expecting INTO (Str num 1, sym num 7): t"}});
}

TEST(SERVER_TEST_SYN_ERROR, SYN_ERROR_TEST_ERROR_IN_SELECT_JOIN_WITHOUT_ON) {
    TestUtils::clear();
    TestUtils::checkRequests({{"select * from t join tt;",
                               "syntax error, unexpected SEMICOLON, expecting ON (Str num 1, sym num 19): ;"}});
}

TEST(SERVER_TEST_SYN_ERROR, SYN_ERROR_TEST_ERROR_IN_SELECT_IN_JOIN) {
    TestUtils::clear();
    TestUtils::checkRequests({{"select * from t jon tt on t.id = tt.id;",
                               "syntax error, unexpected IDENT, expecting UNION or INTERSECT or SEMICOLON (Str num 1, "
                               "sym num 15): "
                               "jon"}});
}

TEST(SERVER_TEST_SYN_ERROR, SYN_ERROR_TEST_ERROR_IN_SELECT_JOIN_MISSING_ALIAS) {
    TestUtils::clear();
    TestUtils::checkRequests({{"select * from t as join tt on t.id = tt.id;",
                               "syntax error, unexpected JOIN, expecting IDENT (Str num 1, sym num 18): join"}});
}

TEST(SERVER_TEST_SYN_ERROR, SYN_ERROR_TEST_ERROR_IN_SELECT_JOIN_IN_AS) {
    TestUtils::clear();
    TestUtils::checkRequests({{"select * from t a t1 join tt on t.id = tt.id;",
                               "syntax error, unexpected IDENT, expecting UNION or INTERSECT or SEMICOLON (Str num 1, "
                               "sym num 13): a"}});
}

TEST(SERVER_TEST_SYN_ERROR, SYN_ERROR_TEST_ERROR_IN_SELECT_JOIN_MISSING_ON) {
    TestUtils::clear();
    TestUtils::checkRequests({{"select * from t as t1 join tt t.id = tt.id;",
                               "syntax error, unexpected IDENT, expecting ON (Str num 1, sym num 23): t"}});
}

TEST(SERVER_TEST_SYN_ERROR, SYN_ERROR_TEST_ERROR_IN_UNION) {
    TestUtils::clear();
    TestUtils::checkRequests({{"select * from t unon select * from t1;",
                               "syntax error, unexpected IDENT, expecting UNION or INTERSECT or SEMICOLON (Str num 1, "
                               "sym num 16): unon"}});
}

TEST(SERVER_TEST_SYN_ERROR, SYN_ERROR_TEST_ERROR_IN_INTERSECT) {
    TestUtils::clear();
    TestUtils::checkRequests({{"select * from t interect select * from t1;",
                               "syntax error, unexpected IDENT, expecting UNION or INTERSECT or SEMICOLON (Str num 1, "
                               "sym num 20): interect"}});
}

TEST(SERVER_TEST_SYN_ERROR, SYN_ERROR_TEST_ERROR_IN_UNION_MISSING_SELECT_AFTER_UNION) {
    TestUtils::clear();
    TestUtils::checkRequests({{"select * from t union;",
                               "syntax error, unexpected SEMICOLON, expecting SELECT_ACTION (Str num 1, sym num 18): "
                               ";"}});
}

TEST(SERVER_TEST_SYN_ERROR, SYN_ERROR_TEST_ERROR_IN_INTERSECT_UNEXPECTED_INSERT) {
    TestUtils::clear();
    TestUtils::checkRequests({{"select * from t intersect insert into t values(1);",
                               "syntax error, unexpected INSERT_ACTION, expecting SELECT_ACTION (Str num 1, sym num "
                               "27): insert"}});
}

TEST(RQG, RQG) {
    TestUtils::clear();
    std::string cols = "(id int, name char(50), city char(50), age int, phone int, address char(50), status char(50), "
                       "preferences char(50), car char(50), education char(50), job char(50), children int, money int, "
                       "house int, pets int);";
    TestUtils::checkRequests({{"create table peoples" + cols, "Success"},
                              {"create table employees" + cols, "Success"},
                              {"create table children" + cols, "Success"},
                              {"create table men" + cols, "Success"},
                              {"create table women" + cols, "Success"},
                              {"create table teachers" + cols, "Success"},
                              {"create table students" + cols, "Success"},
                              {"create table doctors" + cols, "Success"},
                              {"create table directors" + cols, "Success"},
                              {"create table friends" + cols, "Success"}});
    Generator query;
    Client client;
    for (int i = 0; i < 100; i++) {
        std::string a = query.run();
        client.execRequest(a);
        if (client.response.substr(0, 0) == "#") {
            EXPECT_EQ("Bad request","");
        }
        std::cout << a << std::endl;
    }
    std::cout << query.run();
}