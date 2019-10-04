//
// Created by toxaxab on 30.09.2019.
//

#ifndef SELSQL_COMMONUTILS_H
#define SELSQL_COMMONUTILS_H

#include <map>
#include <string>
#include <vector>
enum Type { none, INT, FLOAT, BOOLEAN, CHAR };

enum Constraint { NOT_NULL, PRIMARY_KEY, UNIQUE };

enum Action { SHOW_CREATE, DROP, CREATE };  // TODO make these Classes

struct Variable {
    Type type = none;

    Variable() = default;
    explicit Variable(Type type_) : type(type_) {}

    int addConstraint(Constraint constraint) {
        for (auto &constraint_ : constraints) {
            if (constraint_ == constraint)
                return 1;
        }
        constraints.push_back(constraint);
        return 0;
    }

    std::vector<Constraint> getConstraints() { return constraints; }

   private:
    std::vector<Constraint> constraints;
};

struct Table {
    std::string name;
    // std::map<std::string, Variable> fields;
    void addField(const std::string &field_name, Type type) {
        fields.insert(std::make_pair(field_name, Variable(type)));
        last_var_name = field_name;
    }

    int addConstraint(Constraint constraint) {
        return fields[last_var_name].addConstraint(constraint);
    }

    void clear() {
        name.erase();
        fields.clear();
    }

    std::map<std::string, Variable> getFields() { return fields; }

   private:
    std::string last_var_name;
    std::map<std::string, Variable> fields;
};

struct Response {
    Action action;
    Table table;
    int code = 0;
    std::string errorMsg;
    std::string returnMsg;

    void clear() {
        this->code = 0;
        this->errorMsg.erase();
        this->returnMsg.erase();
        this->table.clear();
    }
};

#endif  // SELSQL_COMMONUTILS_H
