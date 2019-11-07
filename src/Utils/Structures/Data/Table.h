
#ifndef SELSQL_TABLE_H
#define SELSQL_TABLE_H

#include <map>
#include <string>
#include <unordered_map>
#include <utility>
#include "../../Headers/CommonUtils.h"
#include "../../Headers/Constants.h"
#include "Variable.h"

class Table {
   public:
    typedef std::vector<std::pair<std::string, Variable>> FieldsMap;

    size_t record_amount = 0;
    size_t record_size = 0;

    Table() = default;

    Table(std::string _name, FieldsMap _fields) : name(std::move(_name)), fields(std::move(_fields)) {}

    //    void addField(const std::string &field_name, Type type) {
    //        fields.emplace_back(std::make_pair(field_name, Variable(type)));
    //        //last_var_name = field_name;
    //    }

    //    void addFiledWithSize(const std::string &field_name, Type type, int size) {
    //        fields.emplace_back(std::make_pair(field_name, Variable(type, size)));
    //        last_var_name = field_name;
    //    }
    //
    void addField(const std::string &field_name, const Variable &var) {
        fields.emplace_back(std::make_pair(field_name, var));
        з
        // last_var_name = field_name;
    }

    int addConstraint(Constraint constraint) { return fields[fields.size() - 1].second.addConstraint(constraint); }

    //    FieldsMap getFields() { return fields; }

    [[nodiscard]] FieldsMap getFields() const { return fields; }

    void calcRecordSize() {
        record_size = 0;
        for (auto &field : fields) {
            record_size += Constants::TYPE_SIZE[field.second.type] + 1;
        }
    }

    //    void calcMaxDeleted() {
    //        calcRecordSize();
    //        max_deleted_amount = Constants::DATA_SIZE / record_size;
    //        deleted_pos = new int[max_deleted_amount];
    //    }
    void setFields(std::vector<std::pair<std::string, Variable>> &fields_) { this->fields = fields_; }

    void clear() {
        name.erase();
        fields.clear();
    }

    std::string name;
    std::vector<std::pair<std::string, Variable>> fields;
};

#endif  // SELSQL_TABLE_H
