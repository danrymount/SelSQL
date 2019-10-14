#include "Headers/Cursor.h"
#include <cstring>

void convert(unsigned char* dist, std::string val, Type type) {
    unsigned char null_flag = 'n';
    switch (type) {
        case INT: {
            if (!val.empty() and val != "null") {
                int v = std::stoi(val);
                std::memcpy(&dist[1], &v, Constants::TYPE_SIZE[type]);
            } else {
                null_flag = 'N';
            }
            break;
        }
        case FLOAT: {
            if (!val.empty() and val != "null") {
                double fl = std::stod(val);
                std::memcpy(&dist[1], &fl, Constants::TYPE_SIZE[type]);
            } else {
                null_flag = 'N';
            }
            break;
        }
        case CHAR: {
            if (!val.empty()) {
                val.reserve(Constants::TYPE_SIZE[type]);
                std::memcpy(&dist[1], val.c_str(), Constants::TYPE_SIZE[type]);
            } else {
                null_flag = 'N';
            }
            break;
        }
    }
    dist[0] = null_flag;
}

void Cursor::SaveFieldData(std::string val, Type type, unsigned char* dist, int start_pos) {
    unsigned char null_flag = 'n';
    unsigned char temp_field[Constants::TYPE_SIZE[type] + 1];
    auto temp_val = new unsigned char[Constants::TYPE_SIZE[type]];
    convert(temp_field, val, type);
    std::memcpy(&dist[start_pos], temp_field, Constants::TYPE_SIZE[type] + 1);
}

int Cursor::Insert(std::vector<std::string> cols, std::vector<std::string> new_data) {
    int position = table->last_record_pos * table->record_size;
    int count = 0;
    for (auto& i : vals) {
        if (cols.empty()) {
            if (count < new_data.size()) {
                i.second = new_data[count++];
                continue;
            }
        }
        for (size_t j = 0; j < cols.size(); ++j) {
            if (i.first == cols[j]) {
                i.second = new_data[j];
                break;
            }
        }
    }
    auto fields = table->getFields();
    unsigned char record[table->record_size];
    int next_pos = 0;
    for (size_t i = 0; i < vals.size(); ++i) {
        Type type = fields[i].second.type;
        SaveFieldData(vals[i].second, type, record, next_pos);
        next_pos += Constants::TYPE_SIZE[type] + 1;
    }

    table->record_amount++;
    table->last_record_pos++;
    std::memcpy(&data[position], record, table->record_size);

    return 0;
}
int Cursor::Commit() {
    fileManager->UpdateFile(table, data);
    return 0;
}
std::vector<std::pair<std::string, std::string>> Cursor::Fetch() {
    unsigned char record[table->record_size];
    std::vector<std::pair<std::string, std::string>> values;
    std::memcpy(record, &data[pos * table->record_size], table->record_size);
    int field_pos = 0;
    for (int i = 0; i < table->fields.size(); ++i) {
        unsigned char field[Constants::TYPE_SIZE[table->fields[i].second.type] + 1];
        Type type = table->fields[i].second.type;
        std::string value = "";
        std::memcpy(field, &data[field_pos + pos * table->record_size],
                    Constants::TYPE_SIZE[table->fields[i].second.type] + 1);
        if (field[0] == '0' or field[0] == '\000') {
            return std::vector<std::pair<std::string, std::string>>();
        }
        if (field[0] == 'n') {
            GetFieldData(&value, type, field, 0);
        }

        values.emplace_back(std::make_pair(table->fields[i].first, value));
        field_pos += Constants::TYPE_SIZE[table->fields[i].second.type] + 1;
    }
    readed_data++;
    return values;
}
void Cursor::GetFieldData(std::string* dist, Type type, unsigned char* src, int start_pos) {
    switch (type) {
        case INT: {
            int v;
            std::memcpy(&v, &src[start_pos + 1], Constants::TYPE_SIZE[type]);
            *dist = std::to_string(v);
            break;
        }

        case FLOAT: {
            double v;
            std::memcpy(&v, &src[start_pos + 1], Constants::TYPE_SIZE[type]);
            *dist = std::to_string(v);
            break;
        }
        case CHAR: {
            char dst[Constants::TYPE_SIZE[type]];
            std::memcpy(dst, &src[start_pos + 1], Constants::TYPE_SIZE[type]);
            *dist = std::string(dst);
            break;
        }
    }
}
int Cursor::Next() {
    if (readed_data > table->record_amount - 1) {
        return 1;
    } else {
        pos++;
        return 0;
    }
}
int Cursor::Delete() {
    std::memset(&data[pos * table->record_size], '0', table->record_size);
    table->last_record_pos++;
    return 0;
}
