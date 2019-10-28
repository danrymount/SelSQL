#include "Headers/Cursor.h"
#include <cstring>
#include <utility>

void convert(unsigned char *dist, std::string val, Type type) {
    unsigned char null_flag = 'n';
    switch (type) {
        case TYPE_INT: {
            if (!val.empty() and val != "null") {
                int v = std::stoi(val);
                std::memcpy(&dist[1], &v, Constants::TYPE_SIZE[type]);
            } else {
                null_flag = 'N';
            }
            break;
        }
        case TYPE_FLOAT: {
            if (!val.empty() and val != "null") {
                double fl = std::stod(val);
                std::memcpy(&dist[1], &fl, Constants::TYPE_SIZE[type]);
            } else {
                null_flag = 'N';
            }
            break;
        }
        case TYPE_CHAR: {
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

void Cursor::SaveFieldData(std::string val, Type type, unsigned char *dist, int start_pos) {
    unsigned char temp_field[Constants::TYPE_SIZE[type] + 1];
    auto temp_val = new unsigned char[Constants::TYPE_SIZE[type]];
    convert(temp_field, std::move(val), type);
    std::memcpy(&dist[start_pos], temp_field, Constants::TYPE_SIZE[type] + 1);
}

int Cursor::Insert(std::vector<std::string> cols, std::vector<std::string> new_data) {
    size_t pos_in_block = 0;
    std::shared_ptr<DataBlock> block;
    int no_place = 1;
    for (const auto &i : data_blocks_) {
        if (i->deleted) {
            block = i;
            pos_in_block = i->deleted_pos_[--i->deleted] * table_->record_size;
            no_place = 0;
            break;
        } else if (i->last_record_pos <= Constants::DATA_SIZE / table_->record_size - 1) {
            block = i;
            pos_in_block = i->last_record_pos++ * table_->record_size;
            no_place = 0;
            break;
        }
    }
    if (no_place) {
        block = std::make_shared<DataBlock>();
        pos_in_block = block->last_record_pos++;
        block->record_size = table_->record_size;
        block->max_deleted_amount = Constants::DATA_SIZE / table_->record_size;
        block->setDeletedPos(new char[block->max_deleted_amount * sizeof(short int)]);
        data_blocks_.emplace_back(block);
    }

    int count = 0;
    for (auto &i : values_) {
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
    auto fields = table_->getFields();
    unsigned char record[table_->record_size];
    size_t next_pos = 0;
    for (size_t i = 0; i < values_.size(); ++i) {
        Type type = fields[i].second.type;
        SaveFieldData(values_[i].second, type, record, next_pos);
        next_pos += Constants::TYPE_SIZE[type] + 1;
    }

    table_->record_amount++;
    block->record_amount++;
    std::memcpy(&block->data_[pos_in_block], record, table_->record_size);

    return 0;
}

int Cursor::Commit() {
    file_manager_->UpdateFile(table_, data_blocks_);
    return 0;
}

std::vector<std::pair<std::string, std::string>> Cursor::Fetch() {
    auto block = data_blocks_[current_block_];
    unsigned char record[table_->record_size];
    std::vector<std::pair<std::string, std::string>> values;

    std::memcpy(record, &block->data_[current_pos * table_->record_size], table_->record_size);
    int field_pos = 0;
    for (int i = 0; i < table_->fields.size(); ++i) {
        unsigned char field[Constants::TYPE_SIZE[table_->fields[i].second.type] + 1];
        Type type = table_->fields[i].second.type;
        std::string value;
        std::memcpy(field, &block->data_[field_pos + current_pos * table_->record_size],
                    Constants::TYPE_SIZE[table_->fields[i].second.type] + 1);
        if (field[0] != 'n' and field[0] != 'N') {
            return std::vector<std::pair<std::string, std::string>>();
        }
        if (field[0] == 'n') {
            GetFieldData(&value, type, field, 0);
        }

        values.emplace_back(std::make_pair(table_->fields[i].first, value));
        field_pos += Constants::TYPE_SIZE[table_->fields[i].second.type] + 1;
    }
    readed_data++;
    return values;
}

void Cursor::GetFieldData(std::string *dist, Type type, unsigned char *src, int start_pos) {
    switch (type) {
        case TYPE_INT: {
            int v;
            std::memcpy(&v, &src[start_pos + 1], Constants::TYPE_SIZE[type]);
            *dist = std::to_string(v);
            break;
        }

        case TYPE_FLOAT: {
            double v;
            std::memcpy(&v, &src[start_pos + 1], Constants::TYPE_SIZE[type]);
            *dist = std::to_string(v);
            break;
        }
        case TYPE_CHAR: {
            char dst[Constants::TYPE_SIZE[type]];
            std::memcpy(dst, &src[start_pos + 1], Constants::TYPE_SIZE[type]);
            *dist = std::string(dst);
            break;
        }
    }
}

int Cursor::Next() {
    if (data_blocks_[current_block_]->record_amount > readed_data) {
        current_pos++;
        return 0;
    } else {
        if (data_blocks_.size() - 1 == current_block_) {
            return 1;
        }
        current_block_++;
        readed_data = 0;
        current_pos = 0;
        return 0;
    }
}

int Cursor::Delete() {
    auto block = data_blocks_[current_block_];
    std::memset(&block->data_[current_pos * table_->record_size], '0', table_->record_size);
    block->deleted_pos_[block->deleted++] = current_pos;
    block->last_record_pos++;
    deleted_[block]++;
    return 0;
}

int Cursor::Update(std::vector<std::string> cols, std::vector<std::string> new_data) {
    auto block = data_blocks_[current_block_];
    unsigned char record[table_->record_size];
    std::memcpy(record, &block->data_[current_pos * table_->record_size], table_->record_size);
    auto fields = table_->getFields();
    int next_pos = 0;
    for (size_t i = 0; i < values_.size(); ++i) {
        Type type = fields[i].second.type;

        for (size_t j = 0; j < cols.size(); j++) {
            if (values_[i].first == cols[j]) {
                SaveFieldData(new_data[j], type, record, next_pos);
            }
        }
        next_pos += Constants::TYPE_SIZE[type] + 1;
    }

    std::memcpy(&block->data_[current_pos * table_->record_size], record, table_->record_size);

    return 0;
}

int Cursor::Reset() {
    current_pos = 0;
    readed_data = 0;
    current_block_ = 0;
    return 0;
}

Cursor::~Cursor() {
    if (!table_->name.empty()) {
        int del = 0;
        for (const auto &i : deleted_) {
            i.first->record_amount -= i.second;
            del += i.second;
        }
        table_->record_amount -= del;
        Commit();
        file_manager_->CloseAllFiles();
    }
}

Cursor::Cursor() { table_ = std::make_shared<Table>(); }

Cursor::Cursor(const std::shared_ptr<Table> &table, const std::shared_ptr<FileManager> &file_manager)
                                                                                                    : table_(table),
                                                                                                      file_manager_(file_manager) {
    data_blocks_ = file_manager_->ReadDataBlocks(table->name);
    for (const auto &i : table_->fields) {
        values_.emplace_back(std::make_pair(i.first, ""));
    }
}
