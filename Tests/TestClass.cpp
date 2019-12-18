#include <thread>

#include <gtest/gtest.h>
#include "../Client/Client.h"
#include "../src/Utils/Structures/Data/Variable.h"
#include "Headers/TestUtils.h"


//#define KILL
#define Request std::pair<std::string, std::string>

TEST(SERVER_TEST_CREATE, TEST1) {
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

TEST(SERVER_TEST_CREATE, TEST2) {
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

TEST(SERVER_TEST_CREATE, TEST3) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE bbbb(id INT, age float, name char(30));", "Success"},
                              {"INSERT INTO bbbb values(0, 10.5, 'Vasya');", "Success"},
                              {"SHOW CREATE TABLE bbbb;", "CREATE TABLE bbbb(id INT, age FLOAT, name CHAR(30));"},
                              {"SELECT * from bbbb;",
                               "\nid|age      |name   |\n"
                               "0 |10.500000|'Vasya'|\n"}});
}

TEST(SERVER_TEST_SELECT, TEST1) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE t(id INT PRIMARY KEY);", "Success"},
                              {"INSERT INTO t values(0);", "Success"},
                              {"INSERT INTO t values(1);", "Success"},
                              {"SELECT * from t;",
                               "\nid|\n"
                               "0 |\n"
                               "1 |\n"}});
}

TEST(SERVER_TEST_SELECT, TEST2) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE b(id INT PRIMARY KEY, age int NOT NULL);", "Success"},
                              {"INSERT INTO b values(0, 10);", "Success"},
                              {"INSERT INTO b values(1, 20);", "Success"},
                              {"SELECT * from b;",
                               "\nid|age|\n"
                               "0 |10 |\n"
                               "1 |20 |\n"}});
}

TEST(SERVER_TEST_SELECT, TEST3) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE bz(id INT PRIMARY KEY, age int NOT NULL);", "Success"},
                              {"INSERT INTO bz values(0, 10);", "Success"},
                              {"INSERT INTO bz values(1, 20);", "Success"},
                              {"SELECT id from bz;",
                               "\nid|\n"
                               "0 |\n"
                               "1 |\n"}});
}

TEST(SERVER_TEST_SELECT, TEST4) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE bx(id INT PRIMARY KEY, age int NOT NULL, name CHAR(50));", "Success"},
                              {"INSERT INTO bx values(0, 10, 'sdf');", "Success"},
                              {"INSERT INTO bx values(1, 20, 'tyty');", "Success"},
                              {"SELECT id, age, name from bx;",
                               "\nid|age|name  |\n"
                               "0 |10 |'sdf' |\n"
                               "1 |20 |'tyty'|\n"}});
}

TEST(SERVER_TEST_SELECT, TEST5) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE bc(id INT PRIMARY KEY, age int NOT NULL, name CHAR(50));", "Success"},
                              {"INSERT INTO bc values(0, 10, 'sdf');", "Success"},
                              {"INSERT INTO bc values(1, 20, 'tyty');", "Success"},
                              {"SELECT *, id, age, name, * from bc;",
                               "\nid|age|name  |id|age|name  |id|age|name  |\n"
                               "0 |10 |'sdf' |0 |10 |'sdf' |0 |10 |'sdf' |\n"
                               "1 |20 |'tyty'|1 |20 |'tyty'|1 |20 |'tyty'|\n"}});
}

TEST(SERVER_TEST_INSERT, TEST1) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE qwe(id CHAR(50));", "Success"},
                              {"insert into qwe values (1);", "Success"},
                              {"select * from qwe;",
                               "\nid |\n"
                               "'1'|\n"}});
}

TEST(SERVER_TEST_INSERT, TEST2) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE qd(id int);", "Success"},
                              {"insert into qd values (22.22);", "Success"},
                              {"select * from qd;",
                               "\nid|\n"
                               "22|\n"}});
}

TEST(SERVER_TEST_INSERT, TEST3) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE qh(id CHAR(20));", "Success"},
                              {"insert into qh values (2.2);", "Success"},
                              {"select * from qh;",
                               "\nid        |\n"
                               "'2.200000'|\n"}});
}

TEST(SERVER_TEST_INSERT, TEST4) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE qh(id int);", "Success"},
                              {"insert into qh values (1);", "Success"},
                              {"select * from qh;",
                               "\nid|\n"
                               "1 |\n"}});
}

TEST(SERVER_TEST_INSERT, TEST5) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE qh(id CHAR(20));", "Success"},
                              {"insert into qh values ('dgdf dfg %');", "Success"},
                              {"select * from qh;",
                               "\nid          |\n"
                               "'dgdf dfg %'|\n"}});
}

TEST(SERVER_TEST_INSERT, TEST6) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE qh(id float);", "Success"},
                              {"insert into qh values (55.55);", "Success"},
                              {"select * from qh;",
                               "\nid       |\n"
                               "55.550000|\n"}});
}

TEST(SERVER_TEST_INSERT, TEST7) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE qh(id CHAR(50));", "Success"},
                              {"insert into qh values ('ssdf 77 & % sdfsdf*/sdf''s');", "Success"},
                              {"select * from qh;",
                               "\nid                         |\n"
                               "'ssdf 77 & % sdfsdf*/sdf's'|\n"}});
}

TEST(SERVER_TEST_INSERT, TEST8) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE qh(id int NOT NULL UNIQUE, name char(10));", "Success"},
                              {"insert into qh(id) values (6663);", "Success"},
                              {"select * from qh;",
                               "\nid  |name|\n"
                               "6663|    |\n"}});
}

TEST(SERVER_TEST_INSERT, TEST9) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE qh(id int NOT NULL UNIQUE, name char(10));", "Success"},
                              {"insert into qh(name, id) values ('sdf', 6663);", "Success"},
                              {"select * from qh;",
                               "\nid  |name |\n"
                               "6663|'sdf'|\n"}});
}

TEST(SERVER_TEST_INSERT, TEST10) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE qh(id int NOT NULL UNIQUE, name char(10));", "Success"},
                              {"insert into qh values (666, 'sfd');", "Success"},
                              {"select * from qh;",
                               "\nid |name |\n"
                               "666|'sfd'|\n"}});
}

TEST(SERVER_TEST_INSERT, TEST11) {
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

TEST(SERVER_TEST_SHOW_CREATE, TEST1) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE g(id INT PRIMARY KEY, age int NOT NULL);", "Success"},
                              {"SHOW CREATE TABLE g;", "CREATE TABLE g(id INT PRIMARY KEY, age INT NOT NULL);"}});
}

TEST(SERVER_TEST_SHOW_CREATE, TEST2) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE h(id INT);", "Success"},
                              {"SHOW CREATE TABLE h;", "CREATE TABLE h(id INT);"}});
}

TEST(SERVER_TEST_UPDATE, TEST1) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE c(id INT PRIMARY KEY, age int NOT NULL);", "Success"},
                              {"INSERT INTO c values(1, 20);", "Success"},
                              {"UPDATE c SET id = 8;", "Success"},
                              {"SELECT * from c;",
                               "\nid|age|\n"
                               "8 |20 |\n"}});
}

TEST(SERVER_TEST_UPDATE, TEST2) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE d(id INT PRIMARY KEY, age int NOT NULL);", "Success"},
                              {"INSERT INTO d values(10, 20);", "Success"},
                              {"UPDATE d SET id = 15, age = 9;", "Success"},
                              {"SELECT * from d;",
                               "\nid|age|\n"
                               "15|9  |\n"}});
}

TEST(SERVER_TEST_UPDATE, TEST3) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE dd(age float PRIMARY KEY, name char(100));", "Success"},
                              {"INSERT INTO dd values(1.5, 'Petya');", "Success"},
                              {"UPDATE dd SET age = 5.5, name = 'Vasya';", "Success"},
                              {"SELECT * from dd;",
                               "\nage     |name   |\n"
                               "5.500000|'Vasya'|\n"}});
}

TEST(SERVER_TEST_UPDATE, TEST4) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE qg(id int unique);", "Success"},
                              {"insert into qg values (1);", "Success"},
                              {"insert into qg values (2);", "Success"},
                              {"update qg set id = 2 where id = 2;", "Success"},
                              {"select * from qg;",
                               "\nid|\n"
                               "1 |\n2 |\n"}});
}

TEST(SERVER_TEST_UPDATE, TEST5) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE qg(id int unique);", "Success"},
                              {"insert into qg values (1);", "Success"},
                              {"insert into qg values (2);", "Success"},
                              {"update qg set id = 3 where id = 2;", "Success"},
                              {"select * from qg;",
                               "\nid|\n"
                               "1 |\n"
                               "3 |\n"}});
}

TEST(SERVER_TEST_UPDATE, TEST6) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE qg(id int unique);", "Success"},
                              {"insert into qg values (1);", "Success"},
                              {"update qg set id = 1;", "Success"},
                              {"select * from qg;",
                               "\nid|\n"
                               "1 |\n"}});
}

TEST(SERVER_TEST_UPDATE, TEST7) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE qg(id int unique);", "Success"},
                              {"insert into qg values (1);", "Success"},
                              {"update qg set id = 2+2*2;", "Success"},
                              {"select * from qg;",
                               "\nid|\n"
                               "6 |\n"}});
}

TEST(SERVER_TEST_UPDATE, TEST8) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE qg(id int unique);", "Success"},
                              {"insert into qg values (1);", "Success"},
                              {"update qg set id = -3*(2+2*2)+6*2.5;", "Success"},
                              {"select * from qg;",
                               "\nid|\n"
                               "-3|\n"}});
}

TEST(SERVER_TEST_DELETE, TEST1) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE e(id INT PRIMARY KEY, age int NOT NULL);", "Success"},
                              {"DELETE FROM e;", "Success"},
                              {"SELECT * from e;", "Success"}});
}

TEST(SERVER_TEST_DELETE, TEST2) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE f(id INT , age int NOT NULL);", "Success"},
                              {"INSERT INTO f values(10, 20);", "Success"},
                              {"INSERT INTO f values(1, 2);", "Success"},
                              {"DELETE FROM f;", "Success"},
                              {"SELECT * from f;", "Success"}});
}

TEST(SERVER_TEST_DELETE, TEST3) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE ff(id INT , age float NOT NULL, name char(75));", "Success"},
                              {"INSERT INTO ff values(10, 2.0, 'Vvv');", "Success"},
                              {"INSERT INTO ff values(1, 2.789, 'adsasdasdasdasd');", "Success"},
                              {"DELETE FROM ff;", "Success"},
                              {"SELECT * from ff;", "Success"}});
}

TEST(SERVER_TEST_DELETE, TEST4) {
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

TEST(SERVER_TEST_ALL, TEST1) {
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

TEST(SERVER_TEST_WHERE, TEST1) {
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

TEST(SERVER_TEST_WHERE, TEST2) {
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

TEST(SERVER_TEST_WHERE, TEST3) {
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

TEST(SERVER_TEST_WHERE, TEST4) {
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

TEST(SERVER_TEST_WHERE, TEST5) {
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

TEST(SERVER_TEST_WHERE, TEST6) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE fn(id INT NOT NULL , age float, name char(150));", "Success"},
                              {"INSERT INTO fn values(1, 2.9, 'sfsf');", "Success"},
                              {"INSERT INTO fn values(2, 3.789, 'qwerty');", "Success"},
                              {"SELECT * from fn where id = name;", "Success"}});
}

TEST(SERVER_TEST_JOIN, TEST1) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE jj(id INT NOT NULL , age float, name char(150));", "Success"},
                              {"CREATE TABLE jg(id INT NOT NULL , age float);", "Success"},
                              {"INSERT INTO jj values(1, 2.9, 'sfsf');", "Success"},
                              {"INSERT INTO jj values(2, 3.789, 'qwerty');", "Success"},
                              {"INSERT INTO jj values(5, 3.7, 'qwesdfy');", "Success"},
                              {"INSERT INTO jg values(1, 3.5);", "Success"},
                              {"INSERT INTO jg values(2, 3.7);", "Success"},
                              {"INSERT INTO jg values(3, 2.9);", "Success"},
                              {"SELECT * from jj join jg on jj.id = jg.id;",
                               "\njj.id|jj.age  |jj.name |jg.id|jg.age  |\n"
                               "1    |2.900000|'sfsf'  |1    |3.500000|\n"
                               "2    |3.789000|'qwerty'|2    |3.700000|\n"},
                              {"SELECT j1.id, j2.name from jj as j1 join jj as j2 on j1.id = j2.id;",
                               "\nj1.id|j2.name  |\n"
                               "1    |'sfsf'   |\n"
                               "2    |'qwerty' |\n"
                               "5    |'qwesdfy'|\n"}});
}

TEST(SERVER_TEST_JOIN, TEST2) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE jj(id INT NOT NULL , age float, name char(150));", "Success"},
                              {"CREATE TABLE jg(id INT NOT NULL , age float);", "Success"},
                              {"CREATE TABLE jh(id INT NOT NULL , some float);", "Success"},
                              {"INSERT INTO jj values(1, 2.9, 'sfsf');", "Success"},
                              {"INSERT INTO jj values(2, 3.789, 'qwerty');", "Success"},
                              {"INSERT INTO jj values(5, 3.7, 'qwesdfy');", "Success"},
                              {"INSERT INTO jg values(1, 3.5);", "Success"},
                              {"INSERT INTO jg values(2, 3.7);", "Success"},
                              {"INSERT INTO jg values(3, 2.9);", "Success"},
                              {"INSERT INTO jh values(3, 9.5);", "Success"},
                              {"INSERT INTO jh values(1, 3.7);", "Success"},
                              {"INSERT INTO jh values(2, 3.789);", "Success"},
                              {"SELECT * from (jj as j1 join jg as j2 on j1.id = j2.id) join jh on j1.age = jh.some;",
                               "\njh.id|jh.some |j1.id|j1.age  |j1.name |j2.id|j2.age  |\n"
                               "2    |3.789000|2    |3.789000|'qwerty'|2    |3.700000|\n"},
                              {"SELECT * from jh as j3 join (jj as j1 join jg as j2 on j1.age = j2.age) on j3.id = "
                               "j2.id;",
                               "\nj3.id|j3.some |j1.id|j1.age  |j1.name  |j2.id|j2.age  |\n"
                               "3    |9.500000|1    |2.900000|'sfsf'   |3    |2.900000|\n"
                               "2    |3.789000|5    |3.700000|'qwesdfy'|2    |3.700000|\n"}});
}

// TEST(SERVER_TEST_JOIN, TEST3) {
//    TestUtils::clear();
//    TestUtils::checkRequests({{"CREATE TABLE jj(id INT NOT NULL , age float, name char(150));", "Success"},
//                              {"CREATE TABLE jg(id INT NOT NULL , age float);", "Success"},
//                              {"CREATE TABLE jh(id INT NOT NULL , some float);", "Success"},
//                              {"CREATE TABLE jk(id INT NOT NULL , some float, name char(150));", "Success"},
//                              {"INSERT INTO jj values(1, 2.9, 'sfsf');", "Success"},
//                              {"INSERT INTO jj values(2, 3.789, 'qwerty');", "Success"},
//                              {"INSERT INTO jj values(5, 3.7, 'qwesdfy');", "Success"},
//                              {"INSERT INTO jg values(1, 3.5);", "Success"},
//                              {"INSERT INTO jg values(2, 3.7);", "Success"},
//                              {"INSERT INTO jg values(3, 2.9);", "Success"},
//                              {"INSERT INTO jh values(3, 9.5);", "Success"},
//                              {"INSERT INTO jh values(1, 3.7);", "Success"},
//                              {"INSERT INTO jh values(2, 3.789);", "Success"},
//                              {"INSERT INTO jk values(1, 3, 'sfsf');", "Success"},
//                              {"SELECT * from (jj as j1 join jg as j2 on j1.age = j2.age) join (jk join jh on jk.id =
//                              "
//                               "jh.id) on j1.name = jk.name;",
//                               "\nj1.id|j1.age  |j1.name|j2.id|j2.age  |jh.id|jh.some |jk.id|jk.some |jk.name|\n"
//                               "1    |2.900000|'sfsf' |3    |2.900000|1    |3.700000|1    |3.000000|'sfsf' |\n"},
//                              {"SELECT * from ((jk join jg on jk.id = jg.id) join (jj as j1 join jg as j2 on j1.id = "
//                               "j2.id) on jk.id = jg.id) join (jj as j3 join jg as j4 on j3.age = j4.age) on j1.id = "
//                               "j4.id;",
//                               "\njg.id|jg.age  |jk.id|jk.some |jk.name|j1.id|j1.age  |j1.name |j2.id|j2.age  "
//                               "|j3.id|j3.age  |j3.name  |j4.id|j4.age  |\n"
//                               "1    |3.500000|1    |3.000000|'sfsf' |2    |3.789000|'qwerty'|2    |3.700000|5    "
//                               "|3.700000|'qwesdfy'|2    |3.700000|\n"}});
//} //TODO падает с сигсегом

TEST(SERVER_TEST_JOIN, TEST4) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE jl(id INT NOT NULL , age float, name char(150));", "Success"},
                              {"CREATE TABLE jp(id INT NOT NULL , age float);", "Success"},
                              {"INSERT INTO jl values(1, 2.9, 'sfsf');", "Success"},
                              {"INSERT INTO jl values(2, 3.789, 'qwerty');", "Success"},
                              {"INSERT INTO jl values(5, 3.7, 'qwesdfy');", "Success"},
                              {"INSERT INTO jp values(1, 3.5);", "Success"},
                              {"INSERT INTO jp values(2, 3.7);", "Success"},
                              {"INSERT INTO jp values(3, 2.9);", "Success"},
                              {"SELECT * from jl join jp on jl.id = jl.id and jl.age = jp.age;",
                               "\njl.id|jl.age  |jl.name  |jp.id|jp.age  |\n"
                               "1    |2.900000|'sfsf'   |3    |2.900000|\n"
                               "5    |3.700000|'qwesdfy'|2    |3.700000|\n"},
                              {"SELECT j1.id, j2.age from jl as j1 join jp as j2 on j1.id = j2.id or j1.age = j2.age;",
                               "\nj1.id|j2.age  |\n"
                               "1    |3.500000|\n"
                               "1    |2.900000|\n"
                               "2    |3.700000|\n"
                               "5    |3.700000|\n"}});
}

TEST(SERVER_TEST_LEFT_JOIN, TEST1) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE table1(id INT NOT NULL , age float, name char(150));", "Success"},
                              {"CREATE TABLE table2(id INT NOT NULL , age float);", "Success"},
                              {"CREATE TABLE table3(id INT NOT NULL , some float);", "Success"},
                              {"INSERT INTO table1 values(1, 2.9, 'sfsf');", "Success"},
                              {"INSERT INTO table1 values(2, 3.789, 'qwerty');", "Success"},
                              {"INSERT INTO table1 values(5, 3.7, 'qwesdfy');", "Success"},
                              {"INSERT INTO table2 values(1, 3.5);", "Success"},
                              {"INSERT INTO table2 values(2, 3.7);", "Success"},
                              {"INSERT INTO table2 values(3, 2.9);", "Success"},
                              {"INSERT INTO table3 values(3, 9.5);", "Success"},
                              {"INSERT INTO table3 values(1, 3.7);", "Success"},
                              {"INSERT INTO table3 values(2, 3.789);", "Success"},
                              {"SELECT * from table1 left join table2 on table1.id = table2.id;",
                               "\ntable1.id|table1.age|table1.name|table2.id|table2.age|\n"
                               "1        |2.900000  |'sfsf'     |1        |3.500000  |\n"
                               "2        |3.789000  |'qwerty'   |2        |3.700000  |\n"
                               "5        |3.700000  |'qwesdfy'  |null     |null      |\n"},
                              {"SELECT * from (table1 as t1 left join table2 as t2 on t1.id = t2.id) left join table3"
                               " on t1.age = table3.some;",
                               "\nt1.id|t1.age  |t1.name  |t2.id|t2.age  |table3.id|table3.some|\n"
                               "1    |2.900000|'sfsf'   |1    |3.500000|null     |null       |\n"
                               "2    |3.789000|'qwerty' |2    |3.700000|2        |3.789000   |\n"
                               "5    |3.700000|'qwesdfy'|null |null    |1        |3.700000   |\n"},
                              {"SELECT t1.id, t2.age from table1 as t1 left join table2 as t2 on t1.id = t2.id or "
                               "t1.age = t2.age;",
                               "\nt1.id|t2.age  |\n"
                               "1    |3.500000|\n"
                               "1    |2.900000|\n"
                               "2    |3.700000|\n"
                               "5    |3.700000|\n"}});
}

// TEST(SERVER_TEST_RIGHT_JOIN, TEST1) {
//    TestUtils::clear();
//    TestUtils::checkRequests({{"CREATE TABLE table1(id INT NOT NULL , age float, name char(150));", "Success"},
//                              {"CREATE TABLE table2(id INT NOT NULL , age float);", "Success"},
//                              {"CREATE TABLE table3(id INT NOT NULL , some float);", "Success"},
//                              {"INSERT INTO table1 values(1, 2.9, 'sfsf');", "Success"},
//                              {"INSERT INTO table1 values(2, 3.789, 'qwerty');", "Success"},
//                              {"INSERT INTO table1 values(5, 3.7, 'qwesdfy');", "Success"},
//                              {"INSERT INTO table2 values(1, 3.5);", "Success"},
//                              {"INSERT INTO table2 values(2, 3.7);", "Success"},
//                              {"INSERT INTO table2 values(3, 2.9);", "Success"},
//                              {"INSERT INTO table3 values(3, 9.5);", "Success"},
//                              {"INSERT INTO table3 values(1, 3.7);", "Success"},
//                              {"INSERT INTO table3 values(2, 3.789);", "Success"},
//                              {"SELECT t1.id, t2.id, t1.name, t2.age from table1 as t1 right join table2 as t2 on "
//                               "t1.id = t2.id;",
//                               "\n"
//                               "t1.id|t2.id|t1.name |t2.age  |\n"
//                               "1    |1    |'sfsf'  |3.500000|\n"
//                               "2    |2    |'qwerty'|3.700000|\n"
//                               "null |3    |null    |2.900000|\n"},
//                              {"SELECT * from table3 as t3 right join (table1 as t1 right join table2 as t2 on "
//                               "t1.age = t2.age) on t3.id = t2.id;",
//                               "\nt3.id|t3.some |t1.id|t1.age  |t1.name  |t2.id|t2.age  |\n"
//                               "1    |3.700000|null |null    |null     |1    |3.500000|\n"
//                               "2    |3.789000|5    |3.700000|'qwesdfy'|2    |3.700000|\n"
//                               "3    |9.500000|1    |2.900000|'sfsf'   |3    |2.900000|\n"},
//                              {"SELECT table1.id, table2.age from table1 right join table2 on table1.id = table1.id "
//                               "and table1.age = table2.age;",
//                               "\ntable1.id|table2.age|\n"
//                               "null     |3.500000  |\n"
//                               "5        |3.700000  |\n"
//                               "1        |2.900000  |\n"}});
//} //TODO сигсег в дебаге

TEST(SERVER_TEST_LEFT_RIGHT_JOIN, TEST1) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE table1(id INT NOT NULL , age float, name char(150));", "Success"},
                              {"CREATE TABLE table2(id INT NOT NULL , age float);", "Success"},
                              {"CREATE TABLE table3(id INT NOT NULL , some float);", "Success"},
                              {"CREATE TABLE table4(id INT NOT NULL , some float, name char(150));", "Success"},
                              {"INSERT INTO table1 values(1, 2.9, 'sfsf');", "Success"},
                              {"INSERT INTO table1 values(2, 3.789, 'qwerty');", "Success"},
                              {"INSERT INTO table1 values(5, 3.7, 'qwesdfy');", "Success"},
                              {"INSERT INTO table2 values(1, 3.5);", "Success"},
                              {"INSERT INTO table2 values(2, 3.7);", "Success"},
                              {"INSERT INTO table2 values(3, 2.9);", "Success"},
                              {"INSERT INTO table3 values(3, 9.5);", "Success"},
                              {"INSERT INTO table3 values(1, 3.7);", "Success"},
                              {"INSERT INTO table3 values(2, 3.789);", "Success"},
                              {"INSERT INTO table4 values(1, 3, 'sfsf');", "Success"},
                              {"SELECT t1.age, table4.id, table4.name, t2.age from (table1 as t1 right join table2 as "
                               "t2 on t1.age = t2.age) left join (table4 right join table3 on table4.id = table3.id) "
                               "on t1.name = table4.name;",
                               "\nt1.age  |table4.id|table4.name|t2.age  |\n"
                               "null    |null     |null       |3.500000|\n"
                               "3.700000|null     |null       |3.700000|\n"
                               "2.900000|1        |'sfsf'     |2.900000|\n"},
                              {"SELECT * from ((table4 left join table2 on table4.id = table2.id) right join (table1 "
                               "as t1 right join table2 as t2 on t1.id = t2.id) on table4.id = table2.id) left join "
                               "(table1 as t3 right join table2 as t4 on t3.age = t4.age) on t1.id = t4.id;",
                               "\ntable4.id|table4.some|table4.name|table2.id|table2.age|t1.id|t1.age  |t1.name "
                               "|t2.id|t2.age  |t3.id|t3.age  |t3.name  |t4.id|t4.age  |\n"
                               "1        |3.000000   |'sfsf'     |1        |3.500000  |1    |2.900000|'sfsf'  |1    "
                               "|3.500000|null |null    |null     |1    |3.500000|\n"
                               "1        |3.000000   |'sfsf'     |1        |3.500000  |2    |3.789000|'qwerty'|2    "
                               "|3.700000|5    |3.700000|'qwesdfy'|2    |3.700000|\n"
                               "1        |3.000000   |'sfsf'     |1        |3.500000  |null |null    |null    |3    "
                               "|2.900000|null |null    |null     |null |null    |\n"}});
}

TEST(SERVER_TEST_FULL_JOIN, TEST1) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE table1(id INT NOT NULL , age float, name char(150));", "Success"},
                              {"CREATE TABLE table2(id INT NOT NULL , age float);", "Success"},
                              {"INSERT INTO table1 values(1, 2.9, 'sfsf');", "Success"},
                              {"INSERT INTO table1 values(2, 3.789, 'qwerty');", "Success"},
                              {"INSERT INTO table1 values(5, 3.7, 'qwesdfy');", "Success"},
                              {"INSERT INTO table2 values(1, 3.5);", "Success"},
                              {"INSERT INTO table2 values(2, 3.7);", "Success"},
                              {"INSERT INTO table2 values(3, 2.9);", "Success"},
                              {"SELECT * from table1 full join table2 on table1.id = table2.id;",
                               "\ntable1.id|table1.age|table1.name|table2.id|table2.age|\n"
                               "1        |2.900000  |'sfsf'     |1        |3.500000  |\n"
                               "2        |3.789000  |'qwerty'   |2        |3.700000  |\n"
                               "5        |3.700000  |'qwesdfy'  |null     |null      |\n"
                               "null     |null      |null       |3        |2.900000  |\n"},
                              {"SELECT t1.id, t2.age from table1 as t1 full join table2 as t2 on t1.id = t2.id or "
                               "t1.age = t2.age;",
                               "\nt1.id|t2.age  |\n"
                               "1    |3.500000|\n"
                               "1    |2.900000|\n"
                               "2    |3.700000|\n"
                               "5    |3.700000|\n"}});
}

TEST(SERVER_TEST_UNION, TEST1) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE table1(id INT NOT NULL, age float, name char(150));", "Success"},
                              {"CREATE TABLE table2(id INT NOT NULL, age float);", "Success"},
                              {"INSERT INTO table1 values(1, 2.9, 'sfsf');", "Success"},
                              {"INSERT INTO table1 values(2, 3.789, 'qwerty');", "Success"},
                              {"INSERT INTO table1 values(5, 3.7, 'qwesdfy');", "Success"},
                              {"INSERT INTO table2 values(1, 3.5);", "Success"},
                              {"INSERT INTO table2 values(2, 3.789);", "Success"},
                              {"INSERT INTO table2 values(3, 2.9);", "Success"},
                              {"SELECT age from table1 UNION SELECT age from table2;",
                               "\nage     |\n"
                               "3.500000|\n"
                               "3.789000|\n"
                               "2.900000|\n"
                                                                                                         "3.700000|\n"}
                                                                                                        //                              {"SELECT id, age from table1 UNION SELECT id, age from table2;",
                                                                                                        //                               "\nid|age     |\n"
                                                                                                        //                               "1 |3.500000|\n"
                                                                                                        //                               "2 |3.789000|\n"
                                                                                                        //                               "3 |2.900000|\n"
                                                                                                        //                               "1 |2.900000|\n"
                                                                                                        //                               "5 |3.700000|\n"}
    });
}

TEST(SERVER_TEST_UNION, TEST2) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE table1(id INT NOT NULL, age float, name char(150));", "Success"},
                              {"CREATE TABLE table2(id INT NOT NULL, age float, name char(150));", "Success"},
                              {"CREATE TABLE table3(id INT NOT NULL, age float, name char(150));", "Success"},
                              {"INSERT INTO table1 values(1, 2.9, 'sfsf');", "Success"},
                              {"INSERT INTO table1 values(2, 3.789, 'qwerty');", "Success"},
                              {"INSERT INTO table1 values(5, 3.7, 'qwesdfy');", "Success"},
                              {"INSERT INTO table2 values(1, 3.5, 'qwerty');", "Success"},
                              {"INSERT INTO table2 values(2, 3.789, 'qwerty');", "Success"},
                              {"INSERT INTO table2 values(3, 2.9, 'sfsf');", "Success"},
                              {"INSERT INTO table3 values(3, 9.5, 'qwesdfy');", "Success"},
                              {"INSERT INTO table3 values(1, 3.7, 'sfsf');", "Success"},
                              {"INSERT INTO table3 values(2, 3.789, 'sfsf');", "Success"},
                              {"SELECT * from table1 UNION SELECT * from table2;",
                               "\nid|age     |name     |\n"
                               "1 |3.500000|'qwerty' |\n"
                               "2 |3.789000|'qwerty' |\n"
                               "3 |2.900000|'sfsf'   |\n"
                               "1 |2.900000|'sfsf'   |\n"
                               "5 |3.700000|'qwesdfy'|\n"},
                              {"SELECT id, age from table1 where id = 1 UNION SELECT id, age from table2 UNION "
                               "SELECT id, age from table3;",
                               "\nid|age     |\n"
                               "3 |9.500000|\n"
                               "1 |3.700000|\n"
                               "2 |3.789000|\n"
                               "1 |3.500000|\n"
                               "3 |2.900000|\n"
                               "1 |2.900000|\n"}});
}

TEST(SERVER_TEST_INTERSECT, TEST1) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE table1(id INT NOT NULL, age float, name char(150));", "Success"},
                              {"CREATE TABLE table2(id INT NOT NULL, age float);", "Success"},
                              {"INSERT INTO table1 values(1, 2.9, 'sfsf');", "Success"},
                              {"INSERT INTO table1 values(2, 3.789, 'qwerty');", "Success"},
                              {"INSERT INTO table1 values(5, 3.7, 'qwesdfy');", "Success"},
                              {"INSERT INTO table2 values(1, 3.5);", "Success"},
                              {"INSERT INTO table2 values(2, 3.789);", "Success"},
                              {"INSERT INTO table2 values(3, 2.9);", "Success"},
                              {"SELECT age from table1 INTERSECT SELECT age from table2;",
                               "\nage     |\n"
                               "3.789000|\n"
                               "2.900000|\n"},
                              {"SELECT id, age from table1 INTERSECT SELECT id, age from table2;",
                               "\nid|age     |\n"
                               "2 |3.789000|\n"}});
}

TEST(SERVER_TEST_INTERSECT, TEST2) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE table1(id INT NOT NULL, age float, name char(150));", "Success"},
                              {"CREATE TABLE table2(id INT NOT NULL, age float, name char(150));", "Success"},
                              {"CREATE TABLE table3(id INT NOT NULL, age float, name char(150));", "Success"},
                              {"INSERT INTO table1 values(1, 2.9, 'sfsf');", "Success"},
                              {"INSERT INTO table1 values(2, 3.789, 'qwerty');", "Success"},
                              {"INSERT INTO table1 values(5, 3.7, 'qwesdfy');", "Success"},
                              {"INSERT INTO table2 values(1, 3.5, 'qwerty');", "Success"},
                              {"INSERT INTO table2 values(2, 3.789, 'qwerty');", "Success"},
                              {"INSERT INTO table2 values(3, 2.9, 'sfsf');", "Success"},
                              {"INSERT INTO table3 values(3, 9.5, 'qwesdfy');", "Success"},
                              {"INSERT INTO table3 values(1, 3.7, 'sfsf');", "Success"},
                              {"INSERT INTO table3 values(2, 3.789, 'sfsf');", "Success"},
                              {"SELECT * from table1 INTERSECT SELECT * from table2;",
                               "\nid|age     |name    |\n"
                               "2 |3.789000|'qwerty'|\n"},
                              {"SELECT id, age from table1 where id = 1 INTERSECT SELECT id, age from table2 INTERSECT "
                               "SELECT id, age from table3;",
                               "Success"}});
}

TEST(SERVER_TEST_ERROR, TEST1) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE i(id INT);", "Success"},
                              {"CREATE TABLE i(id INT);", "Table already exists ERROR: 1"}});
    TestUtils::clear();
}

TEST(SERVER_TEST_ERROR, TEST2) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE asa(id INT UNIQUE UNIQUE, id1 FLOAT NOT NULL);",
                               "Constraint already exists ERROR: 3"}});
}

TEST(SERVER_TEST_ERROR, TEST3) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE ac(id INT UNIQUE, id FLOAT NOT NULL);",
                               "Field name already used ERROR: 4"}});
}

TEST(SERVER_TEST_ERROR, TEST4) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE m(id INT UNIQUE, di FLOAT NOT NULL);", "Success"},
                              {"insert into m values(1, 2);", "Success"},
                              {"insert into m values(null, null);", "Null values unavailable ERROR: 8"}});
}

TEST(SERVER_TEST_ERROR, TEST5) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE mm(id INT UNIQUE, di FLOAT NOT NULL);", "Success"},
                              {"insert into mm values(1, 2);", "Success"},
                              {"insert into mm values(3, 4);", "Success"},
                              {"update mm set di = null, id = 3;", "Null values unavailable ERROR: 8"}});
}

TEST(SERVER_TEST_ERROR, TEST6) {
    TestUtils::clear();
    TestUtils::checkRequests({{"insert into qqq values(1, 2);", "Table doesn`t exist ERROR: 2"}});
}

TEST(SERVER_TEST_ERROR, TEST7) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE ac(id INT UNIQUE, di FLOAT NOT NULL);", "Success"},
                              {"insert into ac(id) values(1, 2);", "Invalid count of columns and values ERROR: 5"}});
}

TEST(SERVER_TEST_ERROR, TEST8) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE k(id INT UNIQUE);", "Success"},
                              {"insert into k values(1);", "Success"},
                              {"insert into k values(2);", "Success"},
                              {"insert into k values(1);", "Value already exists, use UNIQUE value ERROR: 9"}});
}

TEST(SERVER_TEST_ERROR, TEST9) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE j(id INT);", "Success"},
                              {"insert into j values('sdfsdf');",
                               "Value is out of range OR is not a number ERROR: 12"}});
}

TEST(SERVER_TEST_ERROR, TEST10) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE asas(id INT UNIQUE, id1 FLOAT NOT NULL);", "Success"},
                              {"insert into asas(id) values(1);", "Null values unavailable ERROR: 8"}});
}

TEST(SERVER_TEST_ERROR, TEST11) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE asas(id INT UNIQUE, id1 FLOAT);", "Success"},
                              {"insert into asas(isasfdasf, id) values(1, 2);", "Field doesnt exist ERROR: 6"}});
}

TEST(SERVER_TEST_ERROR, TEST12) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE asas(id INT UNIQUE, id1 FLOAT);", "Success"},
                              {"DROP TABLE asas;", "Success"},
                              {"insert into asas values (1, 5.5);", "Table doesn`t exist ERROR: 2"}});
}

TEST(SERVER_TEST_ERROR, TEST13) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE j(id INT);", "Success"},
                              {"insert into j values(159753159753);",
                               "Int oversize (Str num 1, sym num 30): 159753159753"}});
}

TEST(SERVER_TEST_ERROR, TEST14) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE qw(id INT UNIQUE, id1 FLOAT);", "Success"},
                              {"insert into qw(id, id) values (1, 3);", "Same column in request ERROR: 7"}});
}

TEST(SERVER_TEST_ERROR, TEST15) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE qe(id CHAR(10));", "Success"},
                              {"insert into qe values ('qwertyuiopqwertyuio');", "CHAR IS VERY BIG ERROR: 13"}});
}

TEST(SERVER_TEST_ERROR, TEST16) {
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

TEST(SERVER_TEST_ERROR, TEST17) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE qh(id CHAR(1));", "Success"},
                              {"insert into qh values ('');", "Success"},
                              {"insert into qh values ('a');", "Success"},
                              {"insert into qh values ('aa');", "CHAR IS VERY BIG ERROR: 13"}});
}

TEST(SERVER_TEST_ERROR, TEST18) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE fn(id INT NOT NULL , age float, name char(150));", "Success"},
                              {"INSERT INTO fn values(1, 2.9);", "Invalid count of columns and values ERROR: 5"}});
}

TEST(SERVER_TEST_ERROR, TEST19) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE fn(age float);", "Success"},
                              {"INSERT INTO fn values('dfgdg');",
                               "Value is out of range OR is not a number ERROR: 12"}});
}

TEST(SERVER_TEST_ERROR, TEST20) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE qg(id int unique);", "Success"},
                              {"insert into qg values (1);", "Success"},
                              {"insert into qg values (2);", "Success"},
                              {"update qg set id = 2 where id = 1;",
                               "Value already exists, use UNIQUE value ERROR: 9"}});
}

TEST(SERVER_TEST_ERROR, TEST21) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE qg(id int unique);", "Success"},
                              {"insert into qg values (1);", "Success"},
                              {"insert into qg values (2);", "Success"},
                              {"update qg set id = 3;", "Value already exists, use UNIQUE value ERROR: 9"}});
}

TEST(SERVER_TEST_ERROR, TEST22) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE qg(id int unique);", "Success"},
                              {"insert into qg values (1);", "Success"},
                              {"insert into qg values (2);", "Success"},
                              {"update qg set id = 2;", "Value already exists, use UNIQUE value ERROR: 9"}});
}

TEST(SERVER_TEST_ERROR, TEST23) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE qg(id int unique, name char(50));", "Success"},
                              {"insert into qg values (1, 'qwe');", "Success"},
                              {"insert into qg values (2, 'ewq');", "Success"},
                              {"select * from qg where id = name + 1;", "Types mismatch ERROR: 11"}});
}

// TEST(SERVER_TEST_ERROR, TEST24) {
//    TestUtils::clear();
//    TestUtils::checkRequests({{"CREATE TABLE fn(id INT NOT NULL , age float, name char(150));", "Success"},
//                              {"INSERT INTO fn values(1, 2.9, 'sfsf');", "Success"},
//                              {"INSERT INTO fn values(2, 3.789, 'qwerty');", "Success"},
//                              {"SELECT * from fn where qqq = poi;",
//                               "Table fn:\n"}});  // TODO проверять существование колонки
//}

TEST(SERVER_TEST_ERROR, TEST25) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE fn(id INT NOT NULL , age float, name char(150));", "Success"},
                              {"INSERT INTO fn values(1, 2.9, 'sfsf');", "Success"},
                              {"INSERT INTO fn values(2, 3.789, 'qwerty');", "Success"},
                              {"SELECT * from fn join nf on fn.id = nf.id;", "Table doesn`t exist ERROR: 2"}});
}

TEST(SERVER_TEST_ERROR, TEST26) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE fn(id INT NOT NULL , age float, name char(150));", "Success"},
                              {"INSERT INTO fn values(1, 2.9, 'sfsf');", "Success"},
                              {"INSERT INTO fn values(2, 3.789, 'qwerty');", "Success"},
                              {"SELECT * from fn as f1 join fn on f1.id = fn.id11;", "Field doesnt exist ERROR: 6"}});
}

// TEST(SERVER_TEST_ERROR, TEST27) {
//    TestUtils::clear();
//    TestUtils::checkRequests({{"CREATE TABLE fn(id INT NOT NULL , age float, name char(150));", "Success"},
//                              {"INSERT INTO fn values(1, 2.9, 'sfsf');", "Success"},
//                              {"INSERT INTO fn values(2, 3.789, 'qwerty');", "Success"},
//                              {"SELECT * from fn join fn on fn.id = fn.id;",
//                               "Field doesnt exist ERROR: 6"}});  // TODO чекать на алиас в одной и той же таблице
//}

TEST(SERVER_TEST_ERROR, TEST28) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE fn(id INT NOT NULL , age float, name char(150));", "Success"},
                              {"INSERT INTO fn values(1, 2.9, 'sfsf');", "Success"},
                              {"INSERT INTO fn values(2, 3.789, 'qwerty');", "Success"},
                              {"SELECT * from fn join fn on f1.id = fn.id;", "Field doesnt exist ERROR: 6"}});
}

TEST(SERVER_TEST_ERROR, TEST29) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE fn(id INT NOT NULL , age float, name char(150));", "Success"},
                              {"INSERT INTO fn values(1, 2.9, 'sfsf');", "Success"},
                              {"SELECT dgdfg, id from fn;", "Field doesnt exist ERROR: 6"}});
}

TEST(SERVER_TEST_ERROR, TEST30) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE table1(id INT NOT NULL, age float, name char(150));", "Success"},
                              {"CREATE TABLE table2(id INT NOT NULL, age float);", "Success"},
                              {"INSERT INTO table1 values(1, 2.9, 'sfsf');", "Success"},
                              {"INSERT INTO table2 values(1, 3.5);", "Success"},
                              {"SELECT age from table1 UNION SELECT id from table2;", "Field doesnt exist ERROR: 6"}});
}

TEST(SERVER_TEST_ERROR, TEST31) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE table1(id INT NOT NULL, age float, name char(150));", "Success"},
                              {"CREATE TABLE table2(id INT NOT NULL, age float);", "Success"},
                              {"INSERT INTO table1 values(1, 2.9, 'sfsf');", "Success"},
                              {"INSERT INTO table2 values(1, 3.5);", "Success"},
                              {"SELECT age from table1 INTERSECT SELECT id from table2;",
                               "Field doesnt exist ERROR: 6"}});
}

TEST(SERVER_TEST_ERROR, TEST32) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE table1(id INT NOT NULL, age float, name char(150));", "Success"},
                              {"CREATE TABLE table2(id INT NOT NULL, age float);", "Success"},
                              {"INSERT INTO table1 values(1, 2.9, 'sfsf');", "Success"},
                              {"INSERT INTO table2 values(1, 3.5);", "Success"},
                              {"SELECT * from table1 UNION SELECT * from table2;", "Field doesnt exist ERROR: 6"}});
}

TEST(SERVER_TEST_SYN_ERROR, TEST1) {
    TestUtils::clear();
    TestUtils::checkRequests({{"", "syntax error, unexpected $end (Str num 1, sym num 0): "}});
}

TEST(SERVER_TEST_SYN_ERROR, TEST2) {
    TestUtils::clear();
    TestUtils::checkRequests({{"с", "syntax error, unexpected $end (Str num 1, sym num 0): "}});
}

TEST(SERVER_TEST_SYN_ERROR, TEST3) {
    TestUtils::clear();
    TestUtils::checkRequests({{"create table t ();",
                               "syntax error, unexpected RBRACKET, expecting IDENT (Str num 1, sym num 14): )"}});
}

TEST(SERVER_TEST_SYN_ERROR, TEST4) {
    TestUtils::clear();
    TestUtils::checkRequests({{"creat table t (id int);",
                               "syntax error, unexpected IDENT (Str num 1, sym num 5): creat"}});
}

TEST(SERVER_TEST_SYN_ERROR, TEST5) {
    TestUtils::clear();
    TestUtils::checkRequests({{"create tale t (id int);",
                               "syntax error, unexpected IDENT, expecting TABLE (Str num 1, sym num 10): tale"}});
}

TEST(SERVER_TEST_SYN_ERROR, TEST6) {
    TestUtils::clear();
    TestUtils::checkRequests({{"create table 555 (id int);",
                               "syntax error, unexpected NUMBER, expecting IDENT (Str num 1, sym num 14): 555"}});
}

TEST(SERVER_TEST_SYN_ERROR, TEST7) {
    TestUtils::clear();
    TestUtils::checkRequests({{"create table t (id innt);",
                               "syntax error, unexpected IDENT, expecting INT_TYPE or FLOAT_TYPE or CHAR_TYPE (Str "
                               "num "
                               "1, sym num 19): innt"}});
}

TEST(SERVER_TEST_SYN_ERROR, TEST8) {
    TestUtils::clear();
    TestUtils::checkRequests({{"create table t (id charrrr(50));",
                               "syntax error, unexpected IDENT, expecting INT_TYPE or FLOAT_TYPE or CHAR_TYPE (Str "
                               "num "
                               "1, sym num 22): charrrr"}});
}

TEST(SERVER_TEST_SYN_ERROR, TEST9) {
    TestUtils::clear();
    TestUtils::checkRequests({{"create table t (name char(45454554525452452452452));",
                               "syntax error, unexpected $undefined, expecting INT_TYPE or FLOAT_TYPE or CHAR_TYPE "
                               "(Str num 1, sym num 46): char(45454554525452452452452)"}});
}

TEST(SERVER_TEST_SYN_ERROR, TEST10) {
    TestUtils::clear();
    TestUtils::checkRequests({{"create table t (name char54545(20));",
                               "syntax error, unexpected IDENT, expecting INT_TYPE or FLOAT_TYPE or CHAR_TYPE (Str "
                               "num "
                               "1, sym num 26): char54545"}});
}

TEST(SERVER_TEST_SYN_ERROR, TEST11) {
    TestUtils::clear();
    TestUtils::checkRequests({{"create table t (name 456456);",
                               "syntax error, unexpected NUMBER, expecting INT_TYPE or FLOAT_TYPE or CHAR_TYPE "
                               "(Str "
                               "num 1, sym num 23): 456456"}});
}

TEST(SERVER_TEST_SYN_ERROR, TEST12) {
    TestUtils::clear();
    TestUtils::checkRequests({{"create table 645 (name int);",
                               "syntax error, unexpected NUMBER, expecting IDENT (Str num 1, sym num 14): 645"}});
}

TEST(SERVER_TEST_SYN_ERROR, TEST13) {
    TestUtils::clear();
    TestUtils::checkRequests({{"create table t (name int, nn char);",
                               "syntax error, unexpected IDENT, expecting INT_TYPE or FLOAT_TYPE or CHAR_TYPE (Str "
                               "num "
                               "1, sym num 27): char"}});
}

TEST(SERVER_TEST_SYN_ERROR, TEST14) {
    TestUtils::clear();
    TestUtils::checkRequests({{"create table t (name int;",
                               "syntax error, unexpected SEMICOLON, expecting RBRACKET or COMMA (Str num 1, sym "
                               "num "
                               "21): ;"}});
}

TEST(SERVER_TEST_SYN_ERROR, TEST15) {
    TestUtils::clear();
    TestUtils::checkRequests({{"dro table t;", "syntax error, unexpected IDENT (Str num 1, sym num 3): dro"}});
}

TEST(SERVER_TEST_SYN_ERROR, TEST16) {
    TestUtils::clear();
    TestUtils::checkRequests({{"drop table t, y;",
                               "syntax error, unexpected COMMA, expecting SEMICOLON (Str num 1, sym num 11): ,"}});
}

TEST(SERVER_TEST_SYN_ERROR, TEST17) {
    TestUtils::clear();
    TestUtils::checkRequests({{"drop table t",
                               "syntax error, unexpected $end, expecting SEMICOLON (Str num 1, sym num 10): "}});
}

TEST(SERVER_TEST_SYN_ERROR, TEST18) {
    TestUtils::clear();
    TestUtils::checkRequests({{"shw create table t;", "syntax error, unexpected IDENT (Str num 1, sym num 3): shw"}});
}

TEST(SERVER_TEST_SYN_ERROR, TEST19) {
    TestUtils::clear();
    TestUtils::checkRequests({{"show crate table t;",
                               "syntax error, unexpected IDENT, expecting CREATE_ACTION (Str num 1, sym num 9): "
                               "crate"}});
}

TEST(SERVER_TEST_SYN_ERROR, TEST20) {
    TestUtils::clear();
    TestUtils::checkRequests({{"show create table (t);",
                               "syntax error, unexpected LBRACKET, expecting IDENT (Str num 1, sym num 16): ("}});
}

TEST(SERVER_TEST_SYN_ERROR, TEST21) {
    TestUtils::clear();
    TestUtils::checkRequests({{"insert into 'tt' values(1);",
                               "syntax error, unexpected STRVAL, expecting IDENT (Str num 1, sym num 14): 'tt'"}});
}

TEST(SERVER_TEST_SYN_ERROR, TEST22) {
    TestUtils::clear();
    TestUtils::checkRequests({{"inser into t values(1);",
                               "syntax error, unexpected IDENT (Str num 1, sym num 5): inser"}});
}

TEST(SERVER_TEST_SYN_ERROR, TEST23) {
    TestUtils::clear();
    TestUtils::checkRequests({{"insert ito t values(1);",
                               "syntax error, unexpected IDENT, expecting INTO (Str num 1, sym num 9): ito"}});
}

TEST(SERVER_TEST_SYN_ERROR, TEST24) {
    TestUtils::clear();
    TestUtils::checkRequests({{"insert into t(1);",
                               "syntax error, unexpected NUMBER, expecting IDENT (Str num 1, sym num 13): 1"}});
}

TEST(SERVER_TEST_SYN_ERROR, TEST25) {
    TestUtils::clear();
    TestUtils::checkRequests({{"insert into t(col) valus;",
                               "syntax error, unexpected IDENT, expecting VALUES (Str num 1, sym num 21): valus"}});
}

TEST(SERVER_TEST_SYN_ERROR, TEST26) {
    TestUtils::clear();
    TestUtils::checkRequests({{"insert into t(col) (5);",
                               "syntax error, unexpected LBRACKET, expecting VALUES (Str num 1, sym num 17): ("}});
}

TEST(SERVER_TEST_SYN_ERROR, TEST27) {
    TestUtils::clear();
    TestUtils::checkRequests({{"insert into t (col) (col);",
                               "syntax error, unexpected LBRACKET, expecting VALUES (Str num 1, sym num 17): ("}});
}

TEST(SERVER_TEST_SYN_ERROR, TEST28) {
    TestUtils::clear();
    TestUtils::checkRequests({{"insert into t (col) ();",
                               "syntax error, unexpected LBRACKET, expecting VALUES (Str num 1, sym num 17): ("}});
}

TEST(SERVER_TEST_SYN_ERROR, TEST29) {
    TestUtils::clear();
    TestUtils::checkRequests({{"insert into t values;",
                               "syntax error, unexpected SEMICOLON, expecting LBRACKET (Str num 1, sym num 18): "
                               ";"}});
}

TEST(SERVER_TEST_SYN_ERROR, TEST30) {
    TestUtils::clear();
    TestUtils::checkRequests({{"insert into t values();",
                               "syntax error, unexpected RBRACKET (Str num 1, sym num 19): )"}});
}

TEST(SERVER_TEST_SYN_ERROR, TEST31) {
    TestUtils::clear();
    TestUtils::checkRequests({{"insert t values(7);",
                               "syntax error, unexpected IDENT, expecting INTO (Str num 1, sym num 7): t"}});
}

TEST(SERVER_TEST_SYN_ERROR, TEST32) {
    TestUtils::clear();
    TestUtils::checkRequests({{"select * from t join tt;",
                               "syntax error, unexpected SEMICOLON, expecting ON (Str num 1, sym num 19): ;"}});
}

TEST(SERVER_TEST_SYN_ERROR, TEST33) {
    TestUtils::clear();
    TestUtils::checkRequests({{"select * from t jon tt on t.id = tt.id;",
                               "syntax error, unexpected IDENT, expecting UNION or INTERSECT or SEMICOLON (Str num 1, "
                               "sym num 15): "
                               "jon"}});
}

TEST(SERVER_TEST_SYN_ERROR, TEST34) {
    TestUtils::clear();
    TestUtils::checkRequests({{"select * from t as join tt on t.id = tt.id;",
                               "syntax error, unexpected JOIN, expecting IDENT (Str num 1, sym num 18): join"}});
}

TEST(SERVER_TEST_SYN_ERROR, TEST35) {
    TestUtils::clear();
    TestUtils::checkRequests({{"select * from t a t1 join tt on t.id = tt.id;",
                               "syntax error, unexpected IDENT, expecting UNION or INTERSECT or SEMICOLON (Str num 1, "
                               "sym num 13): a"}});
}

TEST(SERVER_TEST_SYN_ERROR, TEST36) {
    TestUtils::clear();
    TestUtils::checkRequests({{"select * from t as t1 join tt t.id = tt.id;",
                               "syntax error, unexpected IDENT, expecting ON (Str num 1, sym num 23): t"}});
}

TEST(SERVER_TEST_SYN_ERROR, TEST37) {
    TestUtils::clear();
    TestUtils::checkRequests({{"select * from t unon select * from t1;",
                               "syntax error, unexpected IDENT, expecting UNION or INTERSECT or SEMICOLON (Str num 1, "
                               "sym num 16): unon"}});
}

TEST(SERVER_TEST_SYN_ERROR, TEST38) {
    TestUtils::clear();
    TestUtils::checkRequests({{"select * from t interect select * from t1;",
                               "syntax error, unexpected IDENT, expecting UNION or INTERSECT or SEMICOLON (Str num 1, "
                               "sym num 20): interect"}});
}

TEST(SERVER_TEST_SYN_ERROR, TEST39) {
    TestUtils::clear();
    TestUtils::checkRequests({{"select * from t union;",
                               "syntax error, unexpected SEMICOLON, expecting SELECT_ACTION (Str num 1, sym num 18): "
                               ";"}});
}

TEST(SERVER_TEST_SYN_ERROR, TEST40) {
    TestUtils::clear();
    TestUtils::checkRequests({{"select * from t intersect insert into t values(1);",
                               "syntax error, unexpected INSERT_ACTION, expecting SELECT_ACTION (Str num 1, sym num "
                               "27): insert"}});
}
//
// TEST(SERVER_TEST_THREAD, TEST1) {
//    TestUtils::clear();
//    TestUtils::checkRequests({{"create table t(id int, name char(255), city char(255), age float);", "Success"},
//                              {"create table t1(id int, name char(255), city char(255), age float);", "Success"}});
//    std::vector<Request> request1{{"begin;", "Success"},
//                                  {"insert into t values(1, 'Vasya', 'Gorod',  7.5);", "Success"},
//                                  {"insert into t values(1, 'Vasya', 'Gorod',  7.5);", "Success"},
//                                  {"select * from t;", ""},
//                                  {"commit;", "Success"}};
//    std::vector<Request> request2{{"begin;", "Success"},
//                                  {"insert into t1 values(1, 'Vasya', 'Gorod',  7.5);", "Success"},
//                                  {"insert into t1 values(1, 'Vasya', 'Gorod',  7.5);", "Success"},
//                                  {"select * from t1;", ""},
//                                  {"commit;", "Success"}};
//    std::thread client1(TestUtils::checkRequests, request1);
//    std::thread client2(TestUtils::checkRequests, request2);
//    client1.join();
//    client2.join();
//    TestUtils::checkRequests({{"select * from t;", ""}});
//    TestUtils::checkRequests({{"select * from t1;", ""}});
//}
//
// TEST(SERVER_TEST_THREAD, TEST2) {
//    TestUtils::clear();
//    Client client1;
//    Client client2;
//    std::vector<Request> request1{{"create table t(id int, name char(255), city char(255), age float);", "Success"},
//                                  {"begin;", "Success"},
//                                  {"insert into t values(1, 'Vasya', 'Gorod',  7.5);", "Success"},
//                                  {"select * from t;", ""}};
//    std::vector<Request> request2{{"begin;", "Success"},
//                                  {"update t set id = 2;", "Success"},
//                                  {"select * from t;", ""},
//                                  {"commit;", "Success"}};
//    std::vector<Request> request3{{"commit;", "Success"}, {"select * from t", ""}};
//    for (const auto &request : request1) {
//        client1.execRequest(request.first);
//        EXPECT_EQ(client1.response, request.second);
//    }
//    for (const auto &request : request2) {
//        client2.execRequest(request.first);
//        EXPECT_EQ(client2.response, request.second);
//    }
//    for (const auto &request : request3) {
//        client1.execRequest(request.first);
//        EXPECT_EQ(client1.response, request.second);
//    }
//    EXPECT_EQ(client1.response, "Success");
//}
//
// TEST(SERVER_TEST_THREAD, TEST3) {
//    TestUtils::clear();
//    Client client1;
//    Client client2;
//    std::vector<Request> request1{{"create table t(id int, name char(255), city char(255), age float);", "Success"},
//                                  {"begin;", "Success"},
//                                  {"insert into t values(1, 'Vasya', 'Gorod',  7.5);", "Success"},
//                                  {"select * from t;", ""}};
//    std::vector<Request> request2{{"select * from t;", ""},
//                                  {"create table b(id int, name char(255), city char(255), age float);", "Success"},
//                                  {"begin;", "Success"},
//                                  {"insert into b values(1, 'Vasya', 'Gorod',  7.5);", "Success"},
//                                  {"select * from b;", ""},
//                                  {"commit;", "Success"}};
//    std::vector<Request> request3{{"commit;", "Success"}, {"select * from b;", ""}};
//    // всё понятно
//    for (const auto &request : request1) {
//        client1.execRequest(request.first);
//        EXPECT_EQ(client1.response, request.second);
//    }
//    for (const auto &request : request2) {
//        client2.execRequest(request.first);
//        EXPECT_EQ(client2.response, request.second);
//    }
//    for (const auto &request : request3) {
//        client1.execRequest(request.first);
//        EXPECT_EQ(client1.response, request.second);
//    }
//}
//
// TEST(SERVER_TEST_THREAD, TEST4) {
//    TestUtils::clear();
//    TestUtils::checkRequests({{"create table t(id int, name char(255), city char(255), age float);", "Success"},
//                              {"insert into t values(1, 'Vasya', 'Gorod',  7.5);", "Success"}});
//    std::vector<Request> request1{{"begin;", "Success"},
//                                  {"insert into t values(2, 'Vas', 'Gorodok',  4.5);", "Success"},
//                                  {"select * from t;", ""},
//                                  {"commit;", "Success"}};
//    std::vector<Request> request2{{"begin;", "Success"},
//                                  {"delete from t where id = 1;", "Success"},
//                                  {"delete from t where id = 2;", "Success"},
//                                  {"select * from t;", ""},
//                                  {"commit;", "Success"}};
//    // в этом тесте предполагается, что делит у второго клиента выполнится во время работы первого и соответственно не
//    // удаит значение с айди равным 2
//    std::thread client1(TestUtils::checkRequests, request1);
//    std::thread client2(TestUtils::checkRequests, request2);
//    client1.join();
//    client2.join();
//    TestUtils::checkRequests({{"select * from t;", ""}});
//}
//
// TEST(SERVER_TEST_THREAD, TEST5) {
//    TestUtils::clear();
//    Client client1;
//    Client client2;
//    std::vector<Request> request1{{"create table t(id int, name char(255), city char(255), age float);", "Success"},
//                                  {"insert into t values(1, 'Vasya', 'Gorod',  7.5);", "Success"},
//                                  {"begin;", "Success"},
//                                  {"update t set id = 3;", "Success"},
//                                  {"select * from t;", ""}};
//    std::vector<Request> request2{{"select * from t;", ""},
//                                  {"begin;", "Success"},
//                                  {"update t set id = 3;", "Success"},
//                                  {"select * from t;", ""},
//                                  {"commit;", "Success"}};
//    std::vector<Request> request3{{"commit;", "Success"}, {"select * from t;", ""}};
//    // должна быть ошибка, апдейт сразу в двух транзакциях
//    for (const auto &request : request1) {
//        client1.execRequest(request.first);
//        EXPECT_EQ(client1.response, request.second);
//    }
//    for (const auto &request : request2) {
//        client2.execRequest(request.first);
//        EXPECT_EQ(client2.response, request.second);
//    }
//    for (const auto &request : request3) {
//        client1.execRequest(request.first);
//        EXPECT_EQ(client1.response, request.second);
//    }
//    EXPECT_EQ(client1.response, "Success");
//}
//
// TEST(SERVER_TEST_THREAD, TEST6) {
//    TestUtils::clear();
//    TestUtils::checkRequests({{"create table t(id int, name char(255), city char(255), age float);", "Success"},
//                              {"insert into t values(1, 'Vasya', 'Gorod',  7.5);", "Success"}});
//    std::vector<Request> request1{{"begin;", "Success"},
//                                  {"insert into t values(2, 'Vas', 'Gorodok',  4.5);", "Success"},
//                                  {"update t set id = 10 where id = 2;", "Success"},
//                                  {"select * from t;", ""},
//                                  {"commit;", "Success"}};
//    std::vector<Request> request2{{"begin;", "Success"},    {"delete from t where id = 10;", "Success"},
//                                  {"select * from t;", ""}, {"update t set id = 9 where id = 1;", "Success"},
//                                  {"select * from t;", ""}, {"commit;", "Success"}};
//    // в этом тесте предполагается, что делит у второго клиента выполнится во время работы первого и соответственно не
//    // удаит значение с айди равным 10 а также выполнятся апдейты в обоих транзакциях, т.к. они апдейтят разные поля
//    std::thread client1(TestUtils::checkRequests, request1);
//    std::thread client2(TestUtils::checkRequests, request2);
//    client1.join();
//    client2.join();
//    TestUtils::checkRequests({{"select * from t;", ""}});
//}

#ifdef KILL
TEST(SERVER_TEST_SYN_STRESS, TEST1) {
    TestUtils::kill();
    TestUtils::clear();
    TestUtils::run();
    TestUtils::checkRequests({{"CREATE TABLE jj(id INT NOT NULL , age float, name char(150), col1 int, col2 int, col3 "
                               "int);",
                               "Success"}});
    std::string answerFirst = "\nid|age     |name  |col1|col2|col3|\n";
    std::string answerSecond = "\nid|age     |name    |col1|col2|col3|\n";
    std::vector<Request> allRequest;
    for (int i = 0; i < 71; i++) {
        allRequest.emplace_back(std::make_pair("INSERT INTO jj values(1, 2.9, 'sfsf', 1, 1, 1);", "Success"));
        answerFirst += "1 |2.900000|'sfsf'|1   |1   |1   |\n";
        answerSecond += "8 |3.780000|'sdfsdf'|5   |9   |6   |\n";
    };
    TestUtils::checkRequests(allRequest);
    TestUtils::checkDrop({{"UPDATE jj SET id = (8+3-3)*2/2, age = 3.78, name = 'sdfsdf', col1 = 5, "
                           "col2 = 9, col3 = 6 where id = 5 or not(id = 7 or id = 9);",
                           "SELECT * FROM jj;"},
                          {answerFirst, answerSecond}});
}

TEST(SERVER_TEST_SYN_STRESS, TEST2) {
    TestUtils::kill();
    TestUtils::clear();
    TestUtils::run();
    TestUtils::checkRequests({{"CREATE TABLE jj(id INT NOT NULL , age float, name char(150), col1 int, col2 int, col3 "
                               "int);",
                               "Success"}});
    std::string answerFirst = "\nid|age     |name  |col1|col2|col3|\n";
    std::vector<Request> allRequest;
    // TODO FIX ON LINUX
    for (int i = 0; i < 3700; i++) {
        allRequest.emplace_back(std::make_pair("INSERT INTO jj values(1, 2.9, 'sfsf', 1, 1, 1);", "Success"));
        answerFirst += "1 |2.900000|'sfsf'|1   |1   |1   |\n";
    };
    TestUtils::checkRequests(allRequest);
    TestUtils::checkDrop({{"delete from jj where (id = 5 or not(id = 7-7*1 or id = 9) or "
                           "(id = 2+3*(id-75+63)+47 and age >= 5.15) or name = 'sdfsdfsdfsdf' or "
                           "not age = id + age - 5*8 or name >= 'aaaaadddd') and id = 8 or id = 1 or age = 3.7 or "
                           "col1 = 1 or col2 = 1 or col3 = 1 or name = 'qwerty';",
                           "SELECT * FROM jj;"},
                          {answerFirst, "Success"}});
}

TEST(SERVER_TEST_SYN_STRESS, TEST3) {
    TestUtils::kill();
    TestUtils::clear();
    TestUtils::run();
    TestUtils::checkRequests({{"CREATE TABLE jj(id INT NOT NULL , age float, name char(300), "
                               "col1 int NOT NULL, col2 int NOT NULL, col3 int NOT NULL, "
                               "col4 int NOT NULL, col5 int NOT NULL, col6 int NOT NULL, "
                               "col7 int NOT NULL, col8 int NOT NULL, col9 int NOT NULL, "
                               "col10 int NOT NULL, col11 int NOT NULL, col12 int NOT NULL);",
                               "Success"}});
    std::string answerFirst = "\nid |age     |name  |col1|col2|col3|col4|col5|col6|col7|col8|col9|col10|col11|col12|\n";
    std::vector<Request> allRequest;
    for (int i = 1; i < 999; i++) {
        std::string request = "INSERT INTO jj values(" + std::to_string(i) + ", 2.9, 'sfsf', ";
        answerFirst += std::to_string(i);
        for (int j = std::to_string(i).length(); j < 3; j++) {
            answerFirst += " ";
        }
        answerFirst += "|2.900000|'sfsf'|";
        for (int j = 0; j < 12; j++) {
            request += std::to_string(i);
            if (j != 11)
                request += ", ";
            answerFirst += std::to_string(i);
            for (int k = std::to_string(i).length(); k < 4; k++) {
                answerFirst += " ";
            }
            if (j > 8)
                answerFirst += " ";
            answerFirst += "|";
        }
        answerFirst += "\n";
        request += ");";
        allRequest.emplace_back(std::make_pair(request, "Success"));
    };
    TestUtils::checkRequests(allRequest);
    std::string answerSecond = answerFirst +
                               "999|7.500000|'dfdf'|999 |999 |999 |999 |999 |999 |999 |999 |999 |999  |999  |999  |\n";
    TestUtils::checkDrop({{"insert into jj values(999, 7.5, 'dfdf', 999, 999, 999, 999, 999, 999, "
                           "999, 999, 999, 999, 999, 999);",
                           "SELECT * FROM jj;"},
                          {answerFirst, answerSecond}});
}
#endif

TEST(SERVER_TEST_BIG, MANY_REQUESTS) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE jj(id INT NOT NULL , age float, name char(150));", "Success"}});
    std::string answer = "\nid|age     |name  |\n";
    std::string answerUpdate = "\nid|age     |name  |\n";
    for (int i = 0; i < 100; i++) {
        TestUtils::checkRequests({{"INSERT INTO jj values(1, 2.9, 'sfsf');", "Success"}});
        answer += "1 |2.900000|'sfsf'|\n";
        answerUpdate += "15|0.500000|'sfsf'|\n";
    };
    TestUtils::checkRequests({
                                                                                                        {"select * "
                                                                                                         "from jj;",
                                                                                                         answer}
                                                                                                        /*                              {"update
                                                                                                           jj set id =
                                                                                                           15, age =
                                                                                                           0.5;",
                                                                                                           "Success"},
                                                                                                                                      {"select * from jj;", answerUpdate},
                                                                                                                                      {"delete from jj;", "Success"},
                                                                                                                                      {"select * from jj;", "Success"},
                                                                                                                                      {"INSERT INTO jj values(6, 0.9, 'sf');", "Success"},
                                                                                                                                      {"select * from jj;",
                                                                                                                                                                                                                                          "\nid|age     |name|\n"
                                                                                                                                                                                                                                          "6 |0.900000|'sf'|\n"}*/
    });
}