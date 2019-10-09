#include "Headers/Cursor.h"
#include <cstring>

void Cursor::SaveFieldData(std::string val, Type type, unsigned char* dist, int start_pos) {
    unsigned char null_flag = 'n';
    unsigned char temp_field[Constants::TYPE_SIZE[type] + 1];
    auto temp_val = new unsigned char[Constants::TYPE_SIZE[type]];
    switch (type) {
        case INT: {
            if (!val.empty()) {
                int v = std::stoi(val);
                std::memcpy(&temp_field[1], &v, Constants::TYPE_SIZE[type]);
            } else {
                null_flag = 'N';
            }
            break;
        }
        case FLOAT: {
            if (!val.empty()) {
                double fl = std::stod(val);
                std::memcpy(&temp_field[1], &fl, Constants::TYPE_SIZE[type]);
            } else {
                null_flag = 'N';
            }
            break;
        }
        case BOOLEAN:
            break;
        case CHAR: {
            if (!val.empty()) {
                val.reserve(Constants::TYPE_SIZE[type]);
                std::memcpy(&temp_field[1], val.c_str(), Constants::TYPE_SIZE[type]);
            } else {
                null_flag = 'N';
            }
            break;
        }
    }
    temp_field[0] = null_flag;
    std::memcpy(&dist[start_pos], temp_field, Constants::TYPE_SIZE[type] + 1);
}

int Cursor::Insert(std::vector<std::string> cols, std::vector<std::string> new_data) {
    int position = table->record_amount * table->record_size;
    int count = 0;
    for (auto& i : vals) {
        if (cols.empty()) {
            if (count < new_data.size()) {
                i.second = new_data[count++];
                continue;
            }
        }
        for (int j = 0; j < cols.size(); ++j) {
            if (i.first == cols[j]) {
                i.second = new_data[j];
                break;
            }
        }
    }
    auto fields = table->getFields();
    unsigned char record[table->record_size];
    int next_pos = 0;
    for (int i = 0; i < vals.size(); ++i) {
        Type type = fields[i].second.type;
        SaveFieldData(vals[i].second, type, record, next_pos);
        next_pos += Constants::TYPE_SIZE[type] + 1;
    }

    table->record_amount++;
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
        if (field[0] == 'n') {
            GetFieldData(&value, type, field, 0);
        }
        if (field[0] == '0') {
            Next();
            Fetch();
        }
        values.emplace_back(std::make_pair(table->fields[i].first, value));
        field_pos += Constants::TYPE_SIZE[table->fields[i].second.type] + 1;
    }
    for (auto i : values) {
        std::cerr << i.first << "   =   " << i.second << std::endl;
    }
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
        } break;
        case BOOLEAN:
            break;
        case CHAR: {
            char dst[Constants::TYPE_SIZE[type]];
            std::memcpy(dst, &src[start_pos + 1], Constants::TYPE_SIZE[type]);
            *dist = std::string(dst);
            break;
        }
    }
}
int Cursor::Next() {
    if (pos == table->record_amount - 1) {
        return 1;
    } else {
        pos++;
        return 0;
    }
}
