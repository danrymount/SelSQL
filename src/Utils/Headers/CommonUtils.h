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

enum Action { SHOW_CREATE, DROP, CREATE };

struct Variable {
    typedef std::vector<Constraint> ConstraintsVector;

    Variable() = default;
    Variable(Type _type, ConstraintsVector _constraints) : type(_type), constraints(_constraints){};
    explicit Variable(Type type_) : type(type_) {}

    Type type = none;
    int addConstraint(Constraint constraint) {
        for (auto &constraint_ : constraints) {
            if (constraint_ == constraint)
                return 1;
        }
        constraints.push_back(constraint);
        return 0;
    }

    ConstraintsVector getConstraints() { return constraints; }
    ConstraintsVector getConstraints() const { return constraints; }

    void setConstraints(ConstraintsVector &constraints_) { this->constraints = constraints_; }

   private:
    std::vector<Constraint> constraints;
};

struct Table {
    typedef std::map<std::string, Variable> FieldsMap;

    Table() = default;
    Table(std::string _name, FieldsMap _fields) : name(_name), fields(_fields) {}

    std::string name;
    void addField(const std::string &field_name, Type type) {
        fields.insert(std::make_pair(field_name, Variable(type)));
        last_var_name = field_name;
    }

    void addField(const std::string &field_name, const Variable &var) {
        fields.insert(std::make_pair(field_name, var));
        last_var_name = field_name;
    }



    int addConstraint(Constraint constraint) { return fields[last_var_name].addConstraint(constraint); }

    void clear() {
        name.erase();
        fields.clear();
    }

    FieldsMap getFields() { return fields; }

    FieldsMap getFields() const { return fields; }

    void setFields(std::map<std::string, Variable> &fields_) { this->fields = fields_; }


   private:
    std::string last_var_name;
    std::map<std::string, Variable> fields;

};

struct Response {
    Response() = default;
    Response(Action _action, Table _table, int _code, std::string _errorMsg, std::string _returnMsg)
                                                                                                        : action(_action),
                                                                                                          table(_table),
                                                                                                          code(_code),
                                                                                                          errorMsg(_errorMsg),
                                                                                                          returnMsg(_returnMsg) {
    }

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
