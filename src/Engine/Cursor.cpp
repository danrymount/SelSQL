#include "Headers/Cursor.h"
#include <cstring>
#include <mutex>
#include <utility>
std::mutex mutex1;
void convert_INT(char *dist, const std::string &val, Type type);
void convert_FLOAT(char *dist, const std::string &val, Type type);
void convert_CHAR(char *dist, std::string val, Type type);
std::vector<std::function<void(char *dist, std::string val, Type type)>> convert_TYPE = {convert_INT, convert_FLOAT,
                                                                                         convert_CHAR};
void convert_INT(char *dist, const std::string &val, Type type) {
    char null_flag = 'n';
    if (!val.empty() and val != "null") {
        int v = std::stoi(val);
        std::memcpy(&dist[1], &v, C::TYPE_SIZE[type]);
    } else {
        null_flag = 'N';
    }
    dist[0] = null_flag;
}
void convert_FLOAT(char *dist, const std::string &val, Type type) {
    char null_flag = 'n';
    if (!val.empty() and val != "null") {
        double fl = std::stod(val);
        std::memcpy(&dist[1], &fl, C::TYPE_SIZE[type]);
    } else {
        null_flag = 'N';
    }

    dist[0] = null_flag;
}
void convert_CHAR(char *dist, std::string val, Type type) {
    char null_flag = 'n';
    if (!val.empty()) {
        val.reserve(C::TYPE_SIZE[type]);
        if (val.size() > C::TYPE_SIZE[type]) {
        }
        if (val.find('\'') == std::string::npos) {
            if (val.size() + 2 > C::TYPE_SIZE[type]) {
                throw std::exception();
            } else {
                val = '\'' + val + "\'";
            }
        }
        std::memcpy(&dist[1], val.c_str(), C::TYPE_SIZE[type]);
    } else {
        null_flag = 'N';
    }
    dist[0] = null_flag;
}

void Cursor::SaveFieldData(std::string val, Type type, char *dist, int start_pos) {
    char temp_field[C::TYPE_SIZE[type] + 1];
    convert_TYPE[type](temp_field, std::move(val), type);
    std::memcpy(&dist[start_pos], temp_field, C::TYPE_SIZE[type] + 1);
}

int Cursor::Insert(const std::vector<std::string> &cols, const std::vector<std::string> &new_data) {
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
    char record[table_->record_size];
    size_t next_pos = 0;
    for (size_t i = 0; i < values_.size(); ++i) {
        Type type = fields[i].second.type;
        SaveFieldData(values_[i].second, type, record, next_pos);
        next_pos += C::TYPE_SIZE[type] + 1;
    }
    Record new_record(table_->record_size);
    new_record.SetValues(record);
    new_record.tr_s = current_tr_id_;
    new_record.tr_e = 0;
    EmplaceBack(&new_record);
}

std::pair<std::vector<std::pair<std::string, std::string>>, std::pair<int64_t, int64_t>> Cursor::Fetch(int64_t time_s,
                                                                                                       int64_t time_e) {
    Record record(table_->record_size);

    std::vector<std::pair<std::string, std::string>> values;
    if (data_block_ == nullptr) {
        return std::make_pair(values, std::make_pair(0, 0));
    }
    auto block = data_block_;
    data_manager_->IncreaseBlockHeat(BlockPos(table_->name, block_id_));
    char record_buf[record.GetRecordSize()];
    std::memcpy(record_buf, &block->data_[pos_in_block_ * record.GetRecordSize()], record.GetRecordSize());
    record.SetRecord((char *)record_buf);

    int field_pos = 0;
    for (int i = 0; i < table_->fields.size(); ++i) {
        char field[C::TYPE_SIZE[table_->fields[i].second.type] + 1];
        Type type = table_->fields[i].second.type;
        std::string value;
        std::memcpy(field, &record.values_buf[field_pos], C::TYPE_SIZE[table_->fields[i].second.type] + 1);
        if (field[0] != 'n' and field[0] != 'N') {
            return std::make_pair(std::vector<std::pair<std::string, std::string>>(), std::make_pair(0, 0));
        }
        if (field[0] == 'n') {
            GetFieldData(&value, type, field, 0);
        }

        values.emplace_back(std::make_pair(table_->fields[i].first, value));
        field_pos += C::TYPE_SIZE[table_->fields[i].second.type] + 1;
    }
    //    std::cerr << "FETCH" << std::endl;
    //    std::cerr << record.tr_s << " " << record.tr_e << " " << record.commited_tr_s << " " << record.commited_tr_e
    //              << std::endl;
    //    std::cerr << "DATA : ";
    //    for (auto i : values) {
    //        std::cerr << i.second << " ";
    //    }
    //    std::cerr << std::endl;
    auto t_table = transact_manager_->transaction_table;
    if (time_s >= 0 and time_e != 0) {
        if ((record.commited_tr_s == 'c' or record.commited_tr_e == 'c') and
            !(time_s > t_table[record.tr_e].second or time_e < t_table[record.tr_s].second)) {
            auto times = std::make_pair(t_table[record.tr_s].first, record.tr_e != 0 ? t_table[record.tr_e].first : 0);
            return std::make_pair(values, times);
        } else {
            values.clear();
            return std::make_pair(values, std::make_pair(0, 0));
        }
    }

    if ((record.commited_tr_e == 'c' and t_table[record.tr_e].second > t_table[current_tr_id_].first and
         record.tr_s != record.tr_e) or
        (record.commited_tr_e == '0' and record.tr_e != current_tr_id_ and
         t_table[current_tr_id_].first > t_table[record.tr_s].second) or
        (record.commited_tr_s == 'c' and t_table[current_tr_id_].first > t_table[record.tr_s].second and
         record.tr_e == 0) or
        (record.commited_tr_s == '0' and record.tr_s == current_tr_id_ and record.tr_s != record.tr_e)) {
    } else {
        values.clear();
        return std::make_pair(values, std::make_pair(0, 0));
    }
    return std::make_pair(values, std::make_pair(0, 0));
}

void Cursor::GetFieldData(std::string *dist, Type type, char *src, int start_pos) {
    switch (type) {
        case TYPE_INT: {
            int v;
            std::memcpy(&v, &src[start_pos + 1], C::TYPE_SIZE[type]);
            *dist = std::to_string(v);
            break;
        }

        case TYPE_FLOAT: {
            double v;
            std::memcpy(&v, &src[start_pos + 1], C::TYPE_SIZE[type]);
            *dist = std::to_string(v);
            break;
        }
        case TYPE_CHAR: {
            char dst[C::TYPE_SIZE[type]];
            std::memcpy(dst, &src[start_pos + 1], C::TYPE_SIZE[type]);
            *dist = std::string(dst);
            break;
        }
    }
}

int Cursor::NextRecord() {
    if (data_block_ != nullptr and
        max_pos - 1 > pos_in_block_ + (block_id_ * C::DATA_BLOCK_SIZE) / Record(table_->record_size).GetRecordSize()) {
        pos_in_block_++;
        return 0;
    } else {
        return NextDataBlock();
    }
}

int Cursor::Delete() {
    if (transact_manager_->SetUsed(table_->name, Position(block_id_, pos_in_block_), current_tr_id_, 0)) {
        return ErrorConstants::ERR_TRANSACT_CONFLICT;
    }
    Record cur_record(table_->record_size);
    char *buf = new char[cur_record.GetRecordSize()];
    std::memcpy(buf, &data_block_->data_[pos_in_block_ * cur_record.GetRecordSize()], cur_record.GetRecordSize());
    cur_record.SetRecord(buf);
    delete[] buf;
    cur_record.tr_e = current_tr_id_;
    cur_record.commited_tr_e = '0';

    data_block_->was_changed = 1;
    std::memcpy(&data_block_->data_[pos_in_block_ * cur_record.GetRecordSize()], cur_record.GetRecordBuf(),
                cur_record.GetRecordSize());
    return 0;
}

int Cursor::Update(std::vector<std::string> cols, std::vector<std::string> new_data) {
    if (transact_manager_->SetUsed(table_->name, Position(block_id_, pos_in_block_), current_tr_id_, 0)) {
        return ErrorConstants::ERR_TRANSACT_CONFLICT;
    }
    Record record(table_->record_size);
    Record new_record(table_->record_size);
    char full_record_buf[record.GetRecordSize()];
    std::memcpy(full_record_buf, &data_block_->data_[pos_in_block_ * record.GetRecordSize()], record.GetRecordSize());
    record.SetRecord(full_record_buf);
    new_record.SetRecord(full_record_buf);
    auto fields = table_->getFields();
    int next_pos = 0;
    for (size_t i = 0; i < values_.size(); ++i) {
        Type type = fields[i].second.type;

        for (size_t j = 0; j < cols.size(); j++) {
            if (values_[i].first == cols[j]) {
                SaveFieldData(new_data[j], type, new_record.values_buf, next_pos);
            }
        }
        next_pos += C::TYPE_SIZE[type] + 1;
    }

    record.tr_e = current_tr_id_;
    new_record.tr_s = current_tr_id_;
    new_record.tr_e = 0;
    new_record.commited_tr_s = '0';
    new_record.commited_tr_e = '0';
    record.commited_tr_e = '0';
    std::memcpy(&data_block_->data_[pos_in_block_ * record.GetRecordSize()], record.GetRecordBuf(),
                record.GetRecordSize());

    EmplaceBack(&new_record);
    return 0;
}

int Cursor::Reset() {
    pos_in_block_ = 0;
    block_id_ = 0;
    data_block_ = data_manager_->GetDataBlock(table_->name, 0, false);
    transact_manager_->SetBlockUsage(current_tr_id_, table_->name, 0);
    return 0;
}

Cursor::Cursor() { table_ = std::make_shared<Table>(); }

Cursor::Cursor(const std::shared_ptr<Table> &table, const std::shared_ptr<DataManager> &data_manager,
               const std::shared_ptr<TransactManager> &transact_manager, std::shared_ptr<std::fstream> data_file,
               int64_t tr_id)
                                                                                                    : table_(table),
                                                                                                      data_manager_(data_manager),
                                                                                                      transact_manager_(transact_manager),
                                                                                                      data_file_(data_file),
                                                                                                      current_tr_id_(tr_id) {
    Record record(table->record_size);
    data_block_ = data_manager_->GetDataBlock(table_->name, 0, true);
    transact_manager_->SetBlockUsage(current_tr_id_, table_->name, 0);

    for (const auto &i : table_->fields) {
        values_.emplace_back(std::make_pair(i.first, ""));
    }
    //    std::cerr << "MAX POS = " << max_pos << std::endl;
    data_file_->seekg(std::ios::beg);
    data_file_->read(reinterpret_cast<char *>(&max_pos), sizeof(max_pos));
    //    std::cerr << "MAX POS = " << max_pos << std::endl;
}

int Cursor::NextDataBlock() {
    Record record(table_->record_size);

    data_block_ = data_manager_->GetDataBlock(table_->name, ++block_id_, false);

    if (data_block_ == nullptr) {
        --block_id_;
        return 1;
    }
    transact_manager_->SetBlockUsage(current_tr_id_, table_->name, block_id_);
    pos_in_block_ = 0;
    return 0;
}

int Cursor::EmplaceBack(Record *record) {
    std::lock_guard l(mutex1);
    int last_pos = FileManager::GetLastPos(data_file_);

    int block_id = (last_pos) / (C::DATA_BLOCK_SIZE / Record(table_->record_size).GetRecordSize());
    int pos_in_last_block = last_pos % (C::DATA_BLOCK_SIZE / Record(table_->record_size).GetRecordSize());
    auto last_block = data_manager_->GetDataBlock(table_->name, block_id, true);
    transact_manager_->SetBlockUsage(current_tr_id_, table_->name, block_id);
    data_manager_->IncreaseBlockHeat(BlockPos(table_->name, block_id));
    char *record_buf = record->GetRecordBuf();
    if (last_block == nullptr) {
        return 0;
    }
    std::memcpy(&last_block->data_[pos_in_last_block * record->GetRecordSize()], record_buf, record->GetRecordSize());
    delete[] record_buf;
    last_inserted = last_pos;
    last_block->was_changed = 1;
    FileManager::UpdateLastPos(data_file_, last_pos);
    transact_manager_->SetUsed(table_->name, std::make_pair(block_id, pos_in_last_block), current_tr_id_, 1);
    return 0;
}

void Cursor::MakeCommited(const std::shared_ptr<DataBlock> &block, int64_t tr_id,
                          const std::vector<std::pair<int, int>> &need_commit, int record_size) {
    Record record(record_size);
    char c = 'c';
    for (auto i : need_commit) {
        std::memcpy(&block->data_[(i.first + 1) * record.GetRecordSize() - i.second - 1], &c, sizeof(c));
    }
}

Cursor::~Cursor() {
    if (data_file_ != nullptr) {
        data_file_->close();
    }
}

int Cursor::GetLastInsertedPos() { return last_inserted; }

std::shared_ptr<DataManager> Cursor::GetDataManager() { return data_manager_; }
int Cursor::GetCurrentPos() { return pos_in_block_; }
int Cursor::SetPos(int pos) { pos_in_block_ = pos; }
