#include "Headers/Cursor.h"
#include <cstring>
#include <utility>

void convert(char *dist, std::string val, Type type) {
    unsigned char null_flag = 'n';
    switch (type) {
        case TYPE_INT: {
            if (!val.empty() and val != "null") {
                int v = std::stoi(val);
                std::memcpy(&dist[1], &v, C::TYPE_SIZE[type]);
            } else {
                null_flag = 'N';
            }
            break;
        }
        case TYPE_FLOAT: {
            if (!val.empty() and val != "null") {
                double fl = std::stod(val);
                std::memcpy(&dist[1], &fl, C::TYPE_SIZE[type]);
            } else {
                null_flag = 'N';
            }
            break;
        }
        case TYPE_CHAR: {
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
            break;
        }
    }
    dist[0] = null_flag;
}

void Cursor::SaveFieldData(std::string val, Type type, char *dist, int start_pos) {
    char temp_field[C::TYPE_SIZE[type] + 1];
    convert(temp_field, std::move(val), type);
    std::memcpy(&dist[start_pos], temp_field, C::TYPE_SIZE[type] + 1);
}

int Cursor::Insert(const std::vector<std::string> &cols, const std::vector<std::string> &new_data, long transact_sp) {
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
    EmplaceBack(record, transact_sp, 0);
    data_block_->was_changed = 1;
    //    if (transact_manager_->SetUsed(table_->name, Position(write_block_id, current_pos), transact_id)) {
    //        return ErrorC::ERR_TRANSACT_CONFLICT;
    //    }
    //    //    table_->record_amount++;
    //    data_block_->was_changed = 1;

    //    changed = 1;
    //    return 0;
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

std::vector<std::pair<std::string, std::string>> Cursor::Fetch(long tr_p) {
    Record record(table_->record_size);
    char *record_buf = new char[record.GetRecordSize()];
    std::vector<std::pair<std::string, std::string>> values;
    if (data_block_ == nullptr) {
        return values;
    }
    auto block = data_block_;
    std::cerr << "FETCH " << current_pos << std::endl;
    std::memcpy(record_buf, &block->data_[current_pos * record.GetRecordSize()], record.GetRecordSize());
    record.SetRecord((char *)record_buf);
    if (!(record.tr_s <= tr_p and record.tr_e >= tr_p)) {
        return values;
    }
    int field_pos = 0;
    for (int i = 0; i < table_->fields.size(); ++i) {
        char field[C::TYPE_SIZE[table_->fields[i].second.type] + 1];
        Type type = table_->fields[i].second.type;
        std::string value;
        std::memcpy(field, &record.values_buf[field_pos], C::TYPE_SIZE[table_->fields[i].second.type] + 1);
        if (field[0] != 'n' and field[0] != 'N') {
            return std::vector<std::pair<std::string, std::string>>();
        }
        if (field[0] == 'n') {
            GetFieldData(&value, type, field, 0);
        }

        values.emplace_back(std::make_pair(table_->fields[i].first, value));
        field_pos += C::TYPE_SIZE[table_->fields[i].second.type] + 1;
    }
    readed_data++;
    return values;
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
    int last_pos = 0;
    data_file_->seekg(std::ios::beg);
    data_file_->read(reinterpret_cast<char *>(&last_pos), sizeof(last_pos));
    std::cerr << "LAST_POS IN NEXT REC " << last_pos << std::endl;
    std::cerr << "DELETE ON "
              << (C::DATA_BLOCK_SIZE * (read_block_id + 1) / Record(table_->record_size).GetRecordSize()) << std::endl;
    if (data_block_ != nullptr and
        last_pos >= current_pos + (read_block_id * C::DATA_BLOCK_SIZE) / Record(table_->record_size).GetRecordSize()) {
        current_pos++;
        return 0;
    } else {
        return NextDataBlock();
    }
}

int Cursor::Delete(long transact_sp) {
    //    if (transact_manager_->SetUsed(table_->name, Position(write_block_id, current_pos), transact_sp)) {
    //        return ErrorC::ERR_TRANSACT_CONFLICT;
    //    }

    Record record(table_->record_size);
    char *buf = new char[record.GetRecordSize()];
    std::memcpy(buf, &data_block_->data_[current_pos * record.GetRecordSize()], record.GetRecordSize());

    record.tr_e = transact_sp;

    std::memcpy(&data_block_->data_[current_pos * record.GetRecordSize()], record.GetRecordBuf(),
                record.GetRecordSize());
    memset(record.values_buf, '0', table_->record_size);
    EmplaceBack(record.values_buf, transact_sp, 0);
    data_block_->was_changed = 1;
    changed = 1;
    return 0;
}

int Cursor::Update(std::vector<std::string> cols, std::vector<std::string> new_data, long transact_sp) {
    Record record(table_->record_size);
    char full_record_buf[record.GetRecordSize()];
    std::memcpy(full_record_buf, &data_block_->data_[current_pos * record.GetRecordSize()], record.GetRecordSize());

    record.SetRecord(full_record_buf);
    auto fields = table_->getFields();
    int next_pos = 0;
    for (size_t i = 0; i < values_.size(); ++i) {
        Type type = fields[i].second.type;

        for (size_t j = 0; j < cols.size(); j++) {
            if (values_[i].first == cols[j]) {
                SaveFieldData(new_data[j], type, record.values_buf, next_pos);
            }
        }
        next_pos += C::TYPE_SIZE[type] + 1;
    }

    data_block_->was_changed = 1;
    char full_record_buf1[record.GetRecordSize()];
    record.tr_e = transact_sp;

    std::memcpy(&data_block_->data_[current_pos * record.GetRecordSize()], record.GetRecordBuf(),
                record.GetRecordSize());
    //    UpdateDataBlock();
    EmplaceBack(record.values_buf, transact_sp, 0);
    return 0;
}

int Cursor::Reset() {
    current_pos = 0;
    readed_data = 0;
    read_block_id = 0;
    write_block_id = 0;
    Record record(table_->record_size);
    data_block_ = file_manager_->ReadDataBlock(table_->name, read_block_id, data_file_, record.GetRecordSize());
    if (data_block_ == nullptr) {
        data_block_ = Allocate();
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
    Record record(table->record_size);
    data_block_ = file_manager_->ReadDataBlock(table->name, 0, data_file_, record.GetRecordSize());
    if (data_block_ == nullptr) {
        data_block_ = Allocate();
        transact_manager_->SetDataBlock(table->name, 0, data_block_);
    }
    for (const auto &i : table_->fields) {
        values_.emplace_back(std::make_pair(i.first, ""));
    }
}

std::shared_ptr<DataBlock> Cursor::Allocate() {
    auto block = std::make_shared<DataBlock>();
    Record record(table_->record_size);
    block->record_size = record.GetRecordSize();
    char *n_data = new char[C::DATA_BLOCK_SIZE];
    memset(n_data, 0, C::DATA_BLOCK_SIZE);
    block->setData(n_data);
    return block;
}

int Cursor::NextDataBlock() {
    UpdateDataBlock();
    Record record(table_->record_size);

    data_block_ = file_manager_->ReadDataBlock(table_->name, ++read_block_id, data_file_, record.GetRecordSize());
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

void Cursor::Commit(long tr) {
    if (!table_->name.empty()) {
        transact_manager_->Clear(table_->name, tr);
        UpdateDataBlock();
        file_manager_->UpdateFile(table_->name, data_file_);
    }
}

Cursor::~Cursor() {
    if (file_manager_ != nullptr) {
    }
}
int Cursor::EmplaceBack(char *record_buf, long tr_s, long tr_e) {
    int flag = 1;
    int last_pos = 0;
    data_file_->seekg(std::ios::beg);
    data_file_->read(reinterpret_cast<char *>(&last_pos), sizeof(last_pos));
    std::cerr << "EMPLACE_BACK " << last_pos << std::endl;
    int block_id = (last_pos + 1) / (C::DATA_BLOCK_SIZE / Record(table_->record_size).GetRecordSize());
    auto last_block = file_manager_->ReadDataBlock(table_->name, block_id, data_file_, table_->record_size);
    if (last_block == nullptr) {
        flag = 1;
        std::cerr << "EMPLACE ALLOC" << std::endl;
        last_block = Allocate();
        transact_manager_->SetDataBlock(table_->name, block_id, last_block);
    }

    Record new_record(table_->record_size);

    new_record.tr_s = tr_s;
    new_record.tr_e = INT64_MAX;
    new_record.SetValues(record_buf);

    if (flag) {
        std::memcpy(&last_block->data_[last_pos * new_record.GetRecordSize()], new_record.GetRecordBuf(),
                    new_record.GetRecordSize());
        file_manager_->WriteDataBlock(table_, last_block, block_id, data_file_);
    } else {
        data_file_->seekp(4 + last_pos * new_record.GetRecordSize(), std::ios::beg);
        data_file_->write(new_record.GetRecordBuf(), new_record.GetRecordSize());
        std::cerr << "LAST _ POS " << last_pos << std::endl;
        data_file_->flush();
    }
    data_file_->seekp(std::ios::beg);
    data_file_->write(reinterpret_cast<char *>(&(++last_pos)), sizeof(last_pos));
    transact_manager_->SetNewPos(table_->name, last_pos - 1, tr_s);
    return 0;
}
