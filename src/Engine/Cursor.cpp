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
                if (val.size() > Constants::TYPE_SIZE[type]) {
                }
                if (val.find('\'') == std::string::npos) {
                    if (val.size() + 2 > Constants::TYPE_SIZE[type]) {
                        throw std::exception();
                    } else {
                        val = '\'' + val + "\'";
                    }
                }
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
    convert(temp_field, std::move(val), type);
    std::memcpy(&dist[start_pos], temp_field, Constants::TYPE_SIZE[type] + 1);
}

int Cursor::Insert(std::vector<std::string> cols, std::vector<std::string> new_data, int transact_id) {
    size_t pos_in_block = 0;
    int no_place = 1;
    char empty[data_block_->record_size];
    std::memset(&empty, '0', data_block_->record_size);
    do {
        if (std::memcmp(&data_block_->data_[pos_in_block * data_block_->record_size], &empty,
                        data_block_->record_size) == 0) {
            no_place = 0;
            break;
        }
    } while (!NextDataBlock());

    if (no_place) {
        read_block_id++;
        write_block_id++;
        Allocate();
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
    if (transact_manager_->SetUsed(table_->name, Position(write_block_id, current_pos), transact_id)) {
        return ErrorConstants::ERR_TRANSACT_CONFLICT;
    }
    //    table_->record_amount++;
    data_block_->was_changed = 1;
    std::memcpy(&data_block_->data_[pos_in_block], record, table_->record_size);
    changed = 1;
    return 0;
}

int Cursor::UpdateDataBlock() {
    if (data_block_ != nullptr) {
        //        if (data_block_->record_amount == 0) {
        //            --write_block_id;
        //            return 0;
        //        }
        if (data_block_->was_changed) {
            //            table_->record_amount -= current_session_deleted_;
            file_manager_->WriteDataBlock(table_, data_block_, write_block_id, data_file_);
        }
    }
    return 0;
}

std::vector<std::pair<std::string, std::string>> Cursor::Fetch() {
    unsigned char record[table_->record_size];
    std::vector<std::pair<std::string, std::string>> values;
    if (data_block_ == nullptr) {
        return values;
    }
    auto block = data_block_;
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

int Cursor::NextRecord() {
    if (data_block_ != nullptr and Constants::DATA_SIZE / data_block_->record_size > current_pos + 1) {
        current_pos++;
        return 0;
    } else {
        return NextDataBlock();
    }
}

int Cursor::Delete(int transact_id) {
    if (transact_manager_->SetUsed(table_->name, Position(write_block_id, current_pos), transact_id)) {
        return ErrorConstants::ERR_TRANSACT_CONFLICT;
    }
    std::memset(&data_block_->data_[current_pos * table_->record_size], '0', table_->record_size);
    data_block_->was_changed = 1;
    changed = 1;
    return 0;
}

int Cursor::Update(std::vector<std::string> cols, std::vector<std::string> new_data, int transact_id) {
    unsigned char record[table_->record_size];
    std::memcpy(record, &data_block_->data_[current_pos * table_->record_size], table_->record_size);
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
    if (transact_manager_->SetUsed(table_->name, Position(write_block_id, current_pos), transact_id)) {
        return ErrorConstants::ERR_TRANSACT_CONFLICT;
    }
    std::memcpy(&data_block_->data_[current_pos * table_->record_size], record, table_->record_size);
    data_block_->was_changed = 1;
    changed = 1;
    return 0;
}

int Cursor::Reset() {
    current_pos = 0;
    readed_data = 0;
    read_block_id = 0;
    write_block_id = 0;
    data_block_ = file_manager_->ReadDataBlock(table_->name, read_block_id, data_file_, table_->record_size);
    if (data_block_ == nullptr) {
        Allocate();
    }
    return 0;
}

Cursor::Cursor() { table_ = std::make_shared<Table>(); }

Cursor::Cursor(const std::shared_ptr<Table> &table, const std::shared_ptr<FileManager> &file_manager,
               const std::shared_ptr<TransactManager> &transact_manager, std::shared_ptr<std::fstream> data_file)
                                                                                                    : table_(table),
                                                                                                      file_manager_(file_manager),
                                                                                                      transact_manager_(transact_manager),
                                                                                                      data_file_(data_file) {
    data_block_ = file_manager_->ReadDataBlock(table->name, 0, data_file_, table->record_size);
    if (data_block_ == nullptr) {
        Allocate();
    }
    for (const auto &i : table_->fields) {
        values_.emplace_back(std::make_pair(i.first, ""));
    }
}

void Cursor::Allocate() {
    data_block_ = std::make_shared<DataBlock>();
    data_block_->record_size = table_->record_size;
    char *n_data = new char[Constants::DATA_SIZE];
    memset(n_data, 0, Constants::DATA_SIZE);
    data_block_->setData(n_data);
    current_pos = 0;
    readed_data = 0;
    current_session_deleted_ = 0;
}

int Cursor::NextDataBlock() {
    UpdateDataBlock();
    data_block_ = file_manager_->ReadDataBlock(table_->name, ++read_block_id, data_file_, table_->record_size);
    ++write_block_id;

    if (data_block_ == nullptr) {
        --read_block_id;
        --write_block_id;

        return 1;
    }
    //    std::cerr<<"NEXT BLOCK READED "<<read_block_id<<std::endl;
    readed_data = 0;
    current_pos = 0;
    return 0;
}

void Cursor::Commit() {
    if (!table_->name.empty() and changed) {
        UpdateDataBlock();
        file_manager_->UpdateFile(table_->name, data_file_);
    }
}

Cursor::~Cursor() {
    if (file_manager_ != nullptr) {
    }
}
