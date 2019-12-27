#include <gtest/gtest.h>
#include "../../Client/Client.h"
#include "../Headers/TestUtils.h"

TEST(SERVER_TEST_CREATE, CREATE_TEST) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE bb(id INT PRIMARY KEY UNIQUE NOT NULL, age float NOT NULL UNIQUE, name "
                               "char(50));",
                               "Success"},
                              {"INSERT INTO bb values(1, 10.5, 'Vasya');", "Success"},
                              {"SHOW CREATE TABLE bb;",
                               "CREATE TABLE bb(id INT PRIMARY KEY UNIQUE NOT NULL, age FLOAT NOT NULL UNIQUE, name "
                               "CHAR(50));"},
                              {"SELECT * from bb;",
                               "\nid|age      |name   |\n"
                               "1 |10.500000|'Vasya'|\n"}});
}

TEST(SERVER_TEST_CREATE, CREATE_TEST_MORE_CONSTRAINT) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE bbb(id INT PRIMARY KEY, age float NOT NULL, name char(50) UNIQUE);",
                               "Success"},
                              {"INSERT INTO bbb values(0, 10.5, 'Vasya');", "Success"},
                              {"SHOW CREATE TABLE bbb;",
                               "CREATE TABLE bbb(id INT PRIMARY KEY, age FLOAT NOT NULL, name CHAR(50) UNIQUE);"},
                              {"SELECT * from bbb;",
                               "\nid|age      |name   |\n"
                               "0 |10.500000|'Vasya'|\n"}});
}

TEST(SERVER_TEST_CREATE, CREATE_TEST_WITHOUT_CONSTRAINT) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE bbbb(id INT, age float, name char(30));", "Success"},
                              {"INSERT INTO bbbb values(0, 10.5, 'Vasya');", "Success"},
                              {"SHOW CREATE TABLE bbbb;", "CREATE TABLE bbbb(id INT, age FLOAT, name CHAR(30));"},
                              {"SELECT * from bbbb;",
                               "\nid|age      |name   |\n"
                               "0 |10.500000|'Vasya'|\n"}});
}

TEST(SERVER_TEST_SELECT, SELECT_TEST_TABLE_WITH_ONE_COLUMN) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE t(id INT PRIMARY KEY);", "Success"},
                              {"INSERT INTO t values(0);", "Success"},
                              {"INSERT INTO t values(1);", "Success"},
                              {"SELECT * from t;",
                               "\nid|\n"
                               "0 |\n"
                               "1 |\n"}});
}

TEST(SERVER_TEST_SELECT, SELECT_TEST_TABLE_WITH_TWO_COLUMN) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE b(id INT PRIMARY KEY, age int NOT NULL);", "Success"},
                              {"INSERT INTO b values(0, 10);", "Success"},
                              {"INSERT INTO b values(1, 20);", "Success"},
                              {"SELECT * from b;",
                               "\nid|age|\n"
                               "0 |10 |\n"
                               "1 |20 |\n"}});
}

TEST(SERVER_TEST_SELECT, SELECT_TEST_SELECTED_COLUMN) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE bz(id INT PRIMARY KEY, age int NOT NULL);", "Success"},
                              {"INSERT INTO bz values(0, 10);", "Success"},
                              {"INSERT INTO bz values(1, 20);", "Success"},
                              {"SELECT id from bz;",
                               "\nid|\n"
                               "0 |\n"
                               "1 |\n"}});
}

TEST(SERVER_TEST_SELECT, SELECT_TEST_ALL_COLUMNS_SELECTED) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE bx(id INT PRIMARY KEY, age int NOT NULL, name CHAR(50));", "Success"},
                              {"INSERT INTO bx values(0, 10, 'sdf');", "Success"},
                              {"INSERT INTO bx values(1, 20, 'tyty');", "Success"},
                              {"SELECT id, age, name from bx;",
                               "\nid|age|name  |\n"
                               "0 |10 |'sdf' |\n"
                               "1 |20 |'tyty'|\n"}});
}

TEST(SERVER_TEST_SELECT, SELECT_TEST_DUPLICATE_COLUMNS) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE bc(id INT PRIMARY KEY, age int NOT NULL, name CHAR(50));", "Success"},
                              {"INSERT INTO bc values(0, 10, 'sdf');", "Success"},
                              {"INSERT INTO bc values(1, 20, 'tyty');", "Success"},
                              {"SELECT *, id, age, name, * from bc;",
                               "\nid|age|name  |id|age|name  |id|age|name  |\n"
                               "0 |10 |'sdf' |0 |10 |'sdf' |0 |10 |'sdf' |\n"
                               "1 |20 |'tyty'|1 |20 |'tyty'|1 |20 |'tyty'|\n"}});
}

TEST(SERVER_TEST_INSERT, INSERT_TEST_NUMBER_IN_CHAR) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE qwe(id CHAR(50));", "Success"},
                              {"insert into qwe values (1);", "Success"},
                              {"select * from qwe;",
                               "\nid |\n"
                               "'1'|\n"}});
}

TEST(SERVER_TEST_INSERT, INSERT_TEST_FLOAT_NUMBER_IN_INT) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE qd(id int);", "Success"},
                              {"insert into qd values (22.22);", "Success"},
                              {"select * from qd;",
                               "\nid|\n"
                               "22|\n"}});
}

TEST(SERVER_TEST_INSERT, INSERT_TEST_FLOAT_NUMBER_IN_CHAR) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE qh(id CHAR(20));", "Success"},
                              {"insert into qh values (2.2);", "Success"},
                              {"select * from qh;",
                               "\nid        |\n"
                               "'2.200000'|\n"}});
}

TEST(SERVER_TEST_INSERT, INSERT_TEST_INT_NUMBER_IN_INT) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE qh(id int);", "Success"},
                              {"insert into qh values (1);", "Success"},
                              {"select * from qh;",
                               "\nid|\n"
                               "1 |\n"}});
}

TEST(SERVER_TEST_INSERT, INSERT_TEST_STRING_IN_CHAR) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE qh(id CHAR(20));", "Success"},
                              {"insert into qh values ('dgdf dfg %');", "Success"},
                              {"select * from qh;",
                               "\nid          |\n"
                               "'dgdf dfg %'|\n"}});
}

TEST(SERVER_TEST_INSERT, INSERT_TEST_FLOAT_NUMBER_IN_FLOAT) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE qh(id float);", "Success"},
                              {"insert into qh values (55.55);", "Success"},
                              {"select * from qh;",
                               "\nid       |\n"
                               "55.550000|\n"}});
}

TEST(SERVER_TEST_INSERT, INSERT_TEST_LONGER_STRING_IN_CHAR) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE qh(id CHAR(50));", "Success"},
                              {"insert into qh values ('ssdf 77 & % sdfsdf*/sdf''s');", "Success"},
                              {"select * from qh;",
                               "\nid                         |\n"
                               "'ssdf 77 & % sdfsdf*/sdf's'|\n"}});
}

TEST(SERVER_TEST_INSERT, INSERT_TEST_ONE_VALUE_PER_TABLE_WITH_TWO_COLUMNS) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE qh(id int NOT NULL UNIQUE, name char(10));", "Success"},
                              {"insert into qh(id) values (6663);", "Success"},
                              {"select * from qh;",
                               "\nid  |name|\n"
                               "6663|    |\n"}});
}

TEST(SERVER_TEST_INSERT, INSERT_TEST_REVERSE_ORDER) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE qh(id int NOT NULL UNIQUE, name char(10));", "Success"},
                              {"insert into qh(name, id) values ('sdf', 6663);", "Success"},
                              {"select * from qh;",
                               "\nid  |name |\n"
                               "6663|'sdf'|\n"}});
}

TEST(SERVER_TEST_INSERT, INSERT_TEST_TWO_VALUES) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE qh(id int NOT NULL UNIQUE, name char(10));", "Success"},
                              {"insert into qh values (666, 'sfd');", "Success"},
                              {"select * from qh;",
                               "\nid |name |\n"
                               "666|'sfd'|\n"}});
}

TEST(SERVER_TEST_INSERT, INSERT_TEST_NEGATIVE_NUMBER_IN_INT) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE qh(id int NOT NULL UNIQUE);", "Success"},
                              {"insert into qh values (-5);", "Success"},
                              {"select * from qh;",
                               "\nid|\n"
                               "-5|\n"}});
}

// TEST(SERVER_TEST_INSERT, TEST12) {
//    TestUtils::clear();
//    TestUtils::checkRequests({{"CREATE TABLE qh(id int UNIQUE, val int, age float, name char(10));", "Success"},
//                              {"insert into qh(val, name) values (3, 'sdf');", "Success"},
//                              {"select * from qh;",
//                               "\nid|val|age|name |\n"
//                               "  |3  |   |'sdf'|\n"}});
//}

TEST(SERVER_TEST_SHOW_CREATE, SHOW_CREATE_TEST_TWO_COLUMNS) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE g(id INT PRIMARY KEY, age int NOT NULL);", "Success"},
                              {"SHOW CREATE TABLE g;", "CREATE TABLE g(id INT PRIMARY KEY, age INT NOT NULL);"}});
}

TEST(SERVER_TEST_SHOW_CREATE, SHOW_CREATE_TEST_ONE_COLUMNS) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE h(id INT);", "Success"},
                              {"SHOW CREATE TABLE h;", "CREATE TABLE h(id INT);"}});
}

TEST(SERVER_TEST_UPDATE, UPDATE_TEST_UPDATE_INT) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE c(id INT PRIMARY KEY, age int NOT NULL);", "Success"},
                              {"INSERT INTO c values(1, 20);", "Success"},
                              {"UPDATE c SET id = 8;", "Success"},
                              {"SELECT * from c;",
                               "\nid|age|\n"
                               "8 |20 |\n"}});
}

TEST(SERVER_TEST_UPDATE, UPDATE_TEST_UPDATE_INT_AND_FLOAT) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE d(id INT PRIMARY KEY, age int NOT NULL);", "Success"},
                              {"INSERT INTO d values(10, 20);", "Success"},
                              {"UPDATE d SET id = 15, age = 9;", "Success"},
                              {"SELECT * from d;",
                               "\nid|age|\n"
                               "15|9  |\n"}});
}

TEST(SERVER_TEST_UPDATE, UPDATE_TEST_UPDATE_FLOAT_AND_CHAR) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE dd(age float PRIMARY KEY, name char(100));", "Success"},
                              {"INSERT INTO dd values(1.5, 'Petya');", "Success"},
                              {"UPDATE dd SET age = 5.5, name = 'Vasya';", "Success"},
                              {"SELECT * from dd;",
                               "\nage     |name   |\n"
                               "5.500000|'Vasya'|\n"}});
}

TEST(SERVER_TEST_UPDATE, UPDATE_TEST_SET_VALUE_EQALLY_WHERE) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE qg(id int unique);", "Success"},
                              {"insert into qg values (1);", "Success"},
                              {"insert into qg values (2);", "Success"},
                              {"update qg set id = 2 where id = 2;", "Success"},
                              {"select * from qg;",
                               "\nid|\n"
                               "1 |\n2 |\n"}});
}

TEST(SERVER_TEST_UPDATE, UPDATE_TEST_UPDATE_VALUE_WITH_WHERE) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE qg1(id int unique);", "Success"},
                              {"insert into qg1 values (1);", "Success"},
                              {"insert into qg1 values (2);", "Success"},
                              {"update qg1 set id = 3 where id = 2;", "Success"},
                              {"select * from qg1;",
                               "\nid|\n"
                               "1 |\n"
                               "3 |\n"}});
}

TEST(SERVER_TEST_UPDATE, UPDATE_TEST_UPDATE_EXISTING_UNIQUE_VALUE) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE qg2(id int unique);", "Success"},
                              {"insert into qg2 values (1);", "Success"},
                              {"update qg2 set id = 1;", "Success"},
                              {"select * from qg2;",
                               "\nid|\n"
                               "1 |\n"}});
}

TEST(SERVER_TEST_UPDATE, UPDATE_TEST_EASY_MATH_EXPRESSION_IN_SET) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE qg3(id int unique);", "Success"},
                              {"insert into qg3 values (1);", "Success"},
                              {"update qg3 set id = 2+2*2;", "Success"},
                              {"select * from qg3;",
                               "\nid|\n"
                               "6 |\n"}});
}

TEST(SERVER_TEST_UPDATE, UPDATE_TEST_COMPLEX_MATH_EXPRESSION_IN_SET) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE qg4(id int unique);", "Success"},
                              {"insert into qg4 values (1);", "Success"},
                              {"update qg4 set id = -3*(2+2*2)+6*2.5;", "Success"},
                              {"select * from qg4;",
                               "\nid|\n"
                               "-3|\n"}});
}

TEST(SERVER_TEST_DELETE, DELETE_TEST_DELETE_FROM_EMPTY_TABLE) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE e(id INT PRIMARY KEY, age int NOT NULL);", "Success"},
                              {"DELETE FROM e;", "Success"},
                              {"SELECT * from e;", "Success"}});
}

TEST(SERVER_TEST_DELETE, DELETE_TEST_DELETE_TWO_STRING_WITH_TWO_COLUMNS) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE f(id INT , age int NOT NULL);", "Success"},
                              {"INSERT INTO f values(10, 20);", "Success"},
                              {"INSERT INTO f values(1, 2);", "Success"},
                              {"DELETE FROM f;", "Success"},
                              {"SELECT * from f;", "Success"}});
}

TEST(SERVER_TEST_DELETE, DELETE_TEST_DELETE_TWO_STRING_WITH_THREE_COLUMNS) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE ff(id INT , age float NOT NULL, name char(75));", "Success"},
                              {"INSERT INTO ff values(10, 2.0, 'Vvv');", "Success"},
                              {"INSERT INTO ff values(1, 2.789, 'adsasdasdasdasd');", "Success"},
                              {"DELETE FROM ff;", "Success"},
                              {"SELECT * from ff;", "Success"}});
}

TEST(SERVER_TEST_DELETE, DELETE_TEST_DELETE_WITH_WHERE) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE fff(id INT , age float NOT NULL, name char(75));", "Success"},
                              {"INSERT INTO fff values(10, 2.0, 'Vvv');", "Success"},
                              {"INSERT INTO fff values(1, 2.789, 'adsasdasdasdasd');", "Success"},
                              {"DELETE FROM fff where id = 1;", "Success"},
                              {"SELECT * from fff;",
                               "\nid|age     |name |\n"
                               "10|2.000000|'Vvv'|\n"},
                              {"DELETE FROM fff where name = 'Vvv';", "Success"},
                              {"SELECT * from fff;", "Success"}});
}

TEST(SERVER_TEST_ALL, CRUD_TEST) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE fe(id INT , age int NOT NULL);", "Success"},
                              {"INSERT INTO fe values(10, 20);", "Success"},
                              {"INSERT INTO fe values(1, 2);", "Success"},
                              {"SELECT * from fe;",
                               "\nid|age|\n"
                               "10|20 |\n"
                               "1 |2  |\n"},
                              {"UPDATE fe SET id = 5;", "Success"},
                              {"SELECT * from fe;",
                               "\nid|age|\n"
                               "5 |20 |\n"
                               "5 |2  |\n"}});
}

TEST(SERVER_TEST_WHERE, WHERE_TEST) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE fr(id INT , age int NOT NULL);", "Success"},
                              {"INSERT INTO fr values(10, 20);", "Success"},
                              {"INSERT INTO fr values(1, 2);", "Success"},
                              {"SELECT * from fr where id = 2;", "Success"},
                              {"SELECT * from fr where id = 1;",
                               "\nid|age|\n"
                               "1 |2  |\n"},
                              {"UPDATE fr SET id = 5 where id = 4;", "Success"},
                              {"SELECT * from fr;",
                               "\nid|age|\n"
                               "10|20 |\n"
                               "1 |2  |\n"},
                              {"UPDATE fr SET id = 5 where not id = 10;", "Success"},
                              {"SELECT * from fr;",
                               "\nid|age|\n"
                               "10|20 |\n"
                               "5 |2  |\n"},
                              {"DELETE FROM fr where id = 5;", "Success"},
                              {"SELECT * from fr;",
                               "\nid|age|\n"
                               "10|20 |\n"}});
}

TEST(SERVER_TEST_WHERE, WHERE_TEST_WITH_MATH_EXPRESSIONS) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE ft(id INT , age int NOT NULL);", "Success"},
                              {"INSERT INTO ft values(10, 20);", "Success"},
                              {"INSERT INTO ft values(1, 2);", "Success"},
                              {"SELECT * from ft where id = 4*(7-2) - 10;",
                               "\nid|age|\n"
                               "10|20 |\n"},
                              {"SELECT * from ft where id = 1 or id = 2;",
                               "\nid|age|\n"
                               "1 |2  |\n"},
                              {"UPDATE ft SET id = 5 where id = 2 and id = 1;", "Success"},
                              {"SELECT * from ft;",
                               "\nid|age|\n"
                               "10|20 |\n"
                               "1 |2  |\n"},
                              {"UPDATE ft SET id = 5 where not id = 10;", "Success"},
                              {"SELECT * from ft;",
                               "\nid|age|\n"
                               "10|20 |\n"
                               "5 |2  |\n"}});
}

TEST(SERVER_TEST_WHERE, WHERE_TEST_WITH_LOGIC_EXPRESSIONS) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE fy(id INT NOT NULL , age float, name char(150));", "Success"},
                              {"INSERT INTO fy values(1, 2.9, 'sfsf');", "Success"},
                              {"INSERT INTO fy values(2, 3.789, 'qwerty');", "Success"},
                              {"INSERT INTO fy values(3, 0.789, 'hgfdsa');", "Success"},
                              {"SELECT * from fy where id + 1 = id + 1;",
                               "\nid|age     |name    |\n"
                               "1 |2.900000|'sfsf'  |\n"
                               "2 |3.789000|'qwerty'|\n"
                               "3 |0.789000|'hgfdsa'|\n"},
                              {"SELECT * from fy where id = 1 or id = 2;",
                               "\nid|age     |name    |\n"
                               "1 |2.900000|'sfsf'  |\n"
                               "2 |3.789000|'qwerty'|\n"},
                              {"SELECT * from fy where id = 3 and name = 'sfsf' or name = 'qwerty';",
                               "\nid|age     |name    |\n"
                               "2 |3.789000|'qwerty'|\n"},
                              {"SELECT * from fy where id = 3 or name = 'hgfdsa';",
                               "\nid|age     |name    |\n"
                               "3 |0.789000|'hgfdsa'|\n"},
                              {"SELECT * from fy where not age = 2.9 and name = 'qwerty';",
                               "\nid|age     |name    |\n"
                               "2 |3.789000|'qwerty'|\n"}});
}

TEST(SERVER_TEST_WHERE, WHERE_TEST_WITH_MATH_AND_LOGIC_EXPRESSIONS) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE fo(id INT NOT NULL , age float, name char(150));", "Success"},
                              {"INSERT INTO fo values(1, 2.9, 'sfsf');", "Success"},
                              {"INSERT INTO fo values(2, 3.789, 'qwerty');", "Success"},
                              {"INSERT INTO fo values(3, 0.789, 'hgfdsa');", "Success"},
                              {"SELECT * from fo where id + 5 = id + 1;", "Success"},
                              {"SELECT * from fo where not(id = (1+7)*2 - 13 or id = 2 and age = 3.789) and name = "
                               "'sfsf';",
                               "\nid|age     |name  |\n"
                               "1 |2.900000|'sfsf'|\n"},
                              {"SELECT * from fo where not id = 3 and (name = 'sfsf' or id = 1');",
                               "\nid|age     |name  |\n"
                               "1 |2.900000|'sfsf'|\n"},
                              {"SELECT * from fo where id = 3 or not name = 'hgfdsa' or age = '0.789';",
                               "\nid|age     |name    |\n"
                               "1 |2.900000|'sfsf'  |\n"
                               "2 |3.789000|'qwerty'|\n"
                               "3 |0.789000|'hgfdsa'|\n"},
                              {"SELECT * from fo where id = 18/(2+2*2) and not(age = 2.9 and name = 'sfsf');",
                               "\nid|age     |name    |\n"
                               "3 |0.789000|'hgfdsa'|\n"}});
}

TEST(SERVER_TEST_WHERE, WHERE_TEST_WITH_SIGNS_OF_INEQUALITY) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE fp(id INT NOT NULL , age float, name char(150));", "Success"},
                              {"INSERT INTO fp values(1, 2.9, 'sfsf');", "Success"},
                              {"INSERT INTO fp values(2, 3.789, 'qwerty');", "Success"},
                              {"INSERT INTO fp values(3, 0.789, 'hgfdsa');", "Success"},
                              {"SELECT * from fp where id + 5 > id + 1;",
                               "\nid|age     |name    |\n"
                               "1 |2.900000|'sfsf'  |\n"
                               "2 |3.789000|'qwerty'|\n"
                               "3 |0.789000|'hgfdsa'|\n"},
                              {"SELECT * from fp where id > 1;",
                               "\nid|age     |name    |\n"
                               "2 |3.789000|'qwerty'|\n"
                               "3 |0.789000|'hgfdsa'|\n"},
                              {"SELECT * from fp where name > 'a';",
                               "\nid|age     |name    |\n"
                               "1 |2.900000|'sfsf'  |\n"
                               "2 |3.789000|'qwerty'|\n"
                               "3 |0.789000|'hgfdsa'|\n"},
                              {"SELECT * from fp where id != 3 or not name != 'hgfdsa' and age <= 3.789;",
                               "\nid|age     |name    |\n"
                               "1 |2.900000|'sfsf'  |\n"
                               "2 |3.789000|'qwerty'|\n"
                               "3 |0.789000|'hgfdsa'|\n"},
                              {"SELECT * from fp where id >= 18/(2+2*2) and age != 2.9;",
                               "\nid|age     |name    |\n"
                               "3 |0.789000|'hgfdsa'|\n"}});
}

TEST(SERVER_TEST_WHERE, WHERE_TEST_INT_EQUALLY_CHAR) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE fn(id INT NOT NULL , age float, name char(150));", "Success"},
                              {"INSERT INTO fn values(1, 2.9, 'sfsf');", "Success"},
                              {"INSERT INTO fn values(2, 3.789, 'qwerty');", "Success"},
                              {"SELECT * from fn where id = name;", "Success"}});
}

TEST(INDEX_TEST, SIMPLE) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE t(id int);", "Success"},
                              {"INSERT INTO t values(2);", "Success"},
                              {"INSERT INTO t values(1);", "Success"},
                              {"SELECT * from t;", "\nid|\n2 |\n1 |\n"},
                              {"Create index on t(id);", "Success"},
                              {"Select * from t;", "\nid|\n2 |\n1 |\n"},
                              {"insert into t values(432);", "Success"},
                              {"select * from t;", "\nid |\n2  |\n1  |\n432|\n"},
                              {"select * from t where id > 0;", "\nid |\n1  |\n2  |\n432|\n"}});
}

// TEST(VER_TEST, TEST) {
//    TestUtils::clear();
//    TestUtils::checkRequests({{"CREATE TABLE tt(id int);", "Success"},
//                              {"INSERT INTO tt values(2);", "Success"},
//                              {"INSERT INTO tt values(1);", "Success"},
//                              {"SELECT * from tt;", "\nid|\n2 |\n1 |\n"},
//                              {"Select * from tt for system time from all;", "\nid|\n2 |\n1 |\n"},
//                              {"insert into tt values(43);", "Success"},
//                              {"update tt set id = 50;", "Success"},
//                              {"select * from tt for system time from 20-12-2019 16:40:37 to 29-12-2019 10:00:00;",
//                               "Success"}});
//}
