//
// Created by sapiest on 07.10.2019.
//

#ifndef SELSQL_TABLE_H
#define SELSQL_TABLE_H

#include <map>
#include <string>
#include "../../Headers/CommonUtils.h"
#include "Variable.h"

class Table {
   public:
    typedef std::map<std::string, Variable> FieldsMap;

    Table() = default;

    Table(std::string &_name, FieldsMap &_fields) : name(_name), fields(_fields) {}

    void addField(const std::string &field_name, Type type) {
        fields.insert(std::make_pair(field_name, Variable(type)));
        last_var_name = field_name;
    }

    void addField(const std::string &field_name, const Variable &var) {
        fields.insert(std::make_pair(field_name, var));
        last_var_name = field_name;
    }

    int addConstraint(Constraint constraint) { return fields[last_var_name].addConstraint(constraint); }

    FieldsMap getFields() { return fields; }

    FieldsMap getFields() const { return fields; }

    void setFields(std::map<std::string, Variable> &fields_) { this->fields = fields_; }

    void clear() {
        name.erase();
        fields.clear();
    }

    std::string name;

   private:
    std::string last_var_name;
    std::map<std::string, Variable> fields;
};

#endif  // SELSQL_TABLE_H
