#include <gtest/gtest.h>
#include "../../Client/Client.h"
#include "../Headers/TestUtils.h"

TEST(SERVER_TEST_JOIN, JOIN_TEST) {
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

TEST(SERVER_TEST_JOIN, JOIN_TEST_ONE_NESTED_JOIN) {
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

TEST(SERVER_TEST_JOIN, JOIN_TEST_SEVERAL_NESTED_JOIN) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE jj(id INT NOT NULL , age float, name char(150));", "Success"},
                              {"CREATE TABLE jg(id INT NOT NULL , age float);", "Success"},
                              {"CREATE TABLE jh(id INT NOT NULL , some float);", "Success"},
                              {"CREATE TABLE jk(id INT NOT NULL , some float, name char(150));", "Success"},
                              {"INSERT INTO jj values(1, 2.9, 'sfsf');", "Success"},
                              {"INSERT INTO jj values(2, 3.789, 'qwerty');", "Success"},
                              {"INSERT INTO jj values(5, 3.7, 'qwesdfy');", "Success"},
                              {"INSERT INTO jg values(1, 3.5);", "Success"},
                              {"INSERT INTO jg values(2, 3.7);", "Success"},
                              {"INSERT INTO jg values(3, 2.9);", "Success"},
                              {"INSERT INTO jh values(3, 9.5);", "Success"},
                              {"INSERT INTO jh values(1, 3.7);", "Success"},
                              {"INSERT INTO jh values(2, 3.789);", "Success"},
                              {"INSERT INTO jk values(1, 3, 'sfsf');", "Success"},
                              {"SELECT * from (jj as j1 join jg as j2 on j1.age = j2.age) join (jk join jh on jk.id ="
                               "jh.id) on j1.name = jk.name;",
                               "\nj1.id|j1.age  |j1.name|j2.id|j2.age  |jh.id|jh.some |jk.id|jk.some |jk.name|\n"
                               "1    |2.900000|'sfsf' |3    |2.900000|1    |3.700000|1    |3.000000|'sfsf' |\n"},
                              {"SELECT * from ((jk join jg on jk.id = jg.id) join (jj as j1 join jg as j2 on j1.id = "
                               "j2.id) on jk.id = jg.id) join (jj as j3 join jg as j4 on j3.age = j4.age) on j1.id = "
                               "j4.id;",
                               "\njg.id|jg.age  |jk.id|jk.some |jk.name|j1.id|j1.age  |j1.name |j2.id|j2.age  "
                               "|j3.id|j3.age  |j3.name  |j4.id|j4.age  |\n"
                               "1    |3.500000|1    |3.000000|'sfsf' |2    |3.789000|'qwerty'|2    |3.700000|5    "
                               "|3.700000|'qwesdfy'|2    |3.700000|\n"}});
}

TEST(SERVER_TEST_JOIN, TEST_JOIN_WITH_LOGIC_EXPRESSIONS) {
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

TEST(SERVER_TEST_LEFT_JOIN, LEFT_JOIN_TEST) {
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

TEST(SERVER_TEST_RIGHT_JOIN, RIGHT_JOIN_TEST) {
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
                              {"SELECT t1.id, t2.id, t1.name, t2.age from table1 as t1 right join table2 as t2 on "
                               "t1.id = t2.id;",
                               "\n"
                               "t1.id|t2.id|t1.name |t2.age  |\n"
                               "1    |1    |'sfsf'  |3.500000|\n"
                               "2    |2    |'qwerty'|3.700000|\n"
                               "null |3    |null    |2.900000|\n"},
                              {"SELECT * from table3 as t3 right join (table1 as t1 right join table2 as t2 on "
                               "t1.age = t2.age) on t3.id = t2.id;",
                               "\nt3.id|t3.some |t1.id|t1.age  |t1.name  |t2.id|t2.age  |\n"
                               "1    |3.700000|null |null    |null     |1    |3.500000|\n"
                               "2    |3.789000|5    |3.700000|'qwesdfy'|2    |3.700000|\n"
                               "3    |9.500000|1    |2.900000|'sfsf'   |3    |2.900000|\n"},
                              {"SELECT table1.id, table2.age from table1 right join table2 on table1.id = table1.id "
                               "and table1.age = table2.age;",
                               "\ntable1.id|table2.age|\n"
                               "null     |3.500000  |\n"
                               "5        |3.700000  |\n"
                               "1        |2.900000  |\n"}});
}

TEST(SERVER_TEST_LEFT_RIGHT_JOIN, LEFT_RIGHT_JOIN_TEST) {
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

TEST(SERVER_TEST_FULL_JOIN, FULL_JOIN_TEST) {
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

TEST(SERVER_TEST_UNION, UNION_TEST_ONE_UNION) {
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
                               "3.700000|\n"},
                              {"SELECT id, age from table1 UNION SELECT id, age from table2;",
                               "\nid|age     |\n"
                               "1 |3.500000|\n"
                               "2 |3.789000|\n"
                               "3 |2.900000|\n"
                               "1 |2.900000|\n"
                               "5 |3.700000|\n"}});
}

TEST(SERVER_TEST_UNION, UNION_TEST_TWO_UNION_AND_SELECT_ALL) {
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

TEST(SERVER_TEST_INTERSECT, INTERSECT_TEST_ONE_INTERSECT) {
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

TEST(SERVER_TEST_INTERSECT, INTERSECT_TEST_TWO_INTERSECT_AND_SELECT_ALL) {
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

TEST(SERVER_TEST_JOIN_UNION_INTERSECT, TEST_UNION_AND_INTERSECT_WITH_JOIN) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE jj(id INT NOT NULL , age float, name char(150));", "Success"},
                              {"CREATE TABLE jg(id INT NOT NULL , age float);", "Success"},
                              {"INSERT INTO jj values(1, 2.9, 'sfsf');", "Success"},
                              {"INSERT INTO jj values(2, 3.789, 'qwerty');", "Success"},
                              {"INSERT INTO jj values(5, 3.7, 'qwesdfy');", "Success"},
                              {"INSERT INTO jg values(1, 3.5);", "Success"},
                              {"INSERT INTO jg values(2, 3.7);", "Success"},
                              {"INSERT INTO jg values(3, 2.9);", "Success"},
                              {"SELECT * from jj join jg on jj.id = jg.id UNION "
                               "SELECT * from jj join jg on jj.age = jg.age;",
                               "\njj.id|jj.age  |jj.name  |jg.id|jg.age  |\n"
                               "1    |2.900000|'sfsf'   |3    |2.900000|\n"
                               "5    |3.700000|'qwesdfy'|2    |3.700000|\n"
                               "1    |2.900000|'sfsf'   |1    |3.500000|\n"
                               "2    |3.789000|'qwerty' |2    |3.700000|\n"},
                              {"SELECT * from jj join jg on jj.id = jg.id INTERSECT "
                               "SELECT * from jj join jg on jj.age = jg.age;",
                               "Success"}});
}

TEST(SERVER_TEST_JOIN_UNION_INTERSECT, TEST_UNION_AND_INTERSECT_WITH_LEFT_RIGHT_JOIN) {
    TestUtils::clear();
    TestUtils::checkRequests({{"CREATE TABLE jj(id INT NOT NULL , age float, name char(150));", "Success"},
                              {"CREATE TABLE jg(id INT NOT NULL , age float);", "Success"},
                              {"INSERT INTO jj values(1, 2.9, 'sfsf');", "Success"},
                              {"INSERT INTO jj values(2, 3.789, 'qwerty');", "Success"},
                              {"INSERT INTO jj values(5, 3.7, 'qwesdfy');", "Success"},
                              {"INSERT INTO jg values(1, 2.9);", "Success"},
                              {"INSERT INTO jg values(2, 3.7);", "Success"},
                              {"INSERT INTO jg values(3, 2.9);", "Success"},
                              {"SELECT * from jj right join jg on jj.id = jg.id UNION "
                               "SELECT * from jj left join jg on jj.age = jg.age;",
                               "\njj.id|jj.age  |jj.name  |jg.id|jg.age  |\n"
                               "1    |2.900000|'sfsf'   |1    |2.900000|\n"
                               "1    |2.900000|'sfsf'   |3    |2.900000|\n"
                               "2    |3.789000|'qwerty' |null |null    |\n"
                               "5    |3.700000|'qwesdfy'|2    |3.700000|\n"
                               "2    |3.789000|'qwerty' |2    |3.700000|\n"
                               "null |null    |null     |3    |2.900000|\n"},
                              {"SELECT * from jj left join jg on jj.id = jg.id INTERSECT "
                               "SELECT * from jj right join jg on jj.age = jg.age;",
                               "\njj.id|jj.age  |jj.name|jg.id|jg.age  |\n"
                               "1    |2.900000|'sfsf' |1    |2.900000|\n"}});
}