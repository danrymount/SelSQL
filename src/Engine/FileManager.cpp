#include "Headers/FileManager.h"
#include <algorithm>
#include <memory>

#include "../Utils/Structures/Data/DataBlock.h"
#include "../Utils/Structures/Data/Record.h"

void FileManager::WriteTableMetaData(const std::shared_ptr<Table>& table) {
    auto meta_file = meta_files_[table->name];
    meta_file->seekp(0, std::ios::beg);
    buffer_data buffer = GetTableBuffer(table.get());
    meta_file->write(buffer.first, buffer.second);
    meta_file->flush();
    delete[] buffer.first;
}

void FileManager::ReadTableMetaData(const std::string& table_name) {
    auto meta_file = meta_files_[table_name];
    int size = GetFileSize(meta_file.get());
    char buffer[size];
    meta_file->read(buffer, size);
    table_data[table_name] = ReadTableFromBuffer(buffer);
}
files FileManager::OpenFile(const std::string& table_name) {
    const std::string& file_name = table_name;
    if (!fs::exists(table_name)) {
        return files();
    }
    std::string meta_file_name = table_name + DIR_SEPARATOR + table_name + C::META_FILE_TYPE;
    std::string data_file_name = table_name + DIR_SEPARATOR + table_name + C::DATA_FILE_TYPE;
    std::string log_file_name = table_name + DIR_SEPARATOR + table_name + C::LOG_FILE_TYPE;
    std::shared_ptr<std::fstream> meta_file = std::make_shared<std::fstream>(meta_file_name,
                                                                             std::ios::in | std::ios::out | std::ios::binary);
    std::shared_ptr<std::fstream> data_file = std::make_shared<std::fstream>(data_file_name,
                                                                             std::ios::in | std::ios::out | std::ios::binary);

    if (!meta_file->is_open()) {
        return files();
    }
    meta_files_[table_name] = meta_file;
    ReadTableMetaData(table_name);
    if (!data_file->is_open() or GetFileSize(data_file.get()) == 0) {
        data_file = std::make_shared<std::fstream>(data_file_name,
                                                   std::ios::in | std::ios::out | std::ios::binary | std::ios::trunc);
        int zero = 0;
        data_file->write(reinterpret_cast<char*>(&zero), sizeof(zero));
        data_file->flush();
    }
    return files(meta_file, data_file);
}
int FileManager::CreateFile(const std::shared_ptr<Table>& table) {
    std::string table_name = table->name;
    if (!fs::create_directory(table->name)) {
        return 1;
    }

    auto m_file = std::make_shared<std::fstream>(table_name + DIR_SEPARATOR + table_name + C::META_FILE_TYPE,
                                                 std::ios::in | std::ios::out | std::ios::binary | std::ios::trunc);
    auto d_file = std::make_shared<std::fstream>(table_name + DIR_SEPARATOR + table_name + C::DATA_FILE_TYPE,
                                                 std::ios::in | std::ios::out | std::ios::binary | std::ios::trunc);

    meta_files_[table_name] = m_file;
    WriteTableMetaData(table);
    return 0;
}
std::shared_ptr<Table> FileManager::GetTable(const std::string& table_name) { return table_data[table_name]; }

int FileManager::DeleteFile(const std::string& table_name) { return !fs::remove_all(table_name); }

int FileManager::WriteDataBlock(const std::shared_ptr<Table>& table, std::shared_ptr<DataBlock> data, int block_id,
                                std::shared_ptr<std::fstream> dist) {
    int offset = 4 + block_id * C::DATA_BLOCK_SIZE;
    buffer_data buffer = GetDataBlockBuffer(data.get());
    dist->seekp(offset);
    dist->write(buffer.first, buffer.second);
    dist->flush();
    delete[] buffer.first;
    return 0;
}

std::shared_ptr<DataBlock> FileManager::ReadDataBlock(const std::string& table_name, int block_id,
                                                      const std::shared_ptr<std::fstream>& src) {
    auto data_file = src.get();

    if (data_file == nullptr or !data_file->is_open()) {
        return nullptr;
    }

    if (!GetFileSize(data_file)) {
        return nullptr;
    }
    if (GetFileSize(data_file) <= 4 + C::DATA_BLOCK_SIZE * block_id) {
        return nullptr;
    }

    data_file->seekg(sizeof(int), std::ios::beg);

    if (GetFileSize(data_file) < C::DATA_BLOCK_SIZE) {
        return nullptr;
    }
    data_file->seekg(sizeof(int) + C::DATA_BLOCK_SIZE * block_id, std::ios::beg);
    char data_buffer[C::DATA_BLOCK_SIZE];
    data_file->read(data_buffer, C::DATA_BLOCK_SIZE);
    return ReadDataBlockFromBuffer(data_buffer);
}

int FileManager::UpdateFile(const std::string& table_name, const std::shared_ptr<std::fstream>& src) {
    auto flag = std::fstream(C::FLAG_FILE, std::ios::in | std::ios::out);

    if (!flag.is_open() and !table_name.empty()) {
        //        flag = std::fstream(C::FLAG_FILE, std::ios::in | std::ios::out | std::ios::trunc);
        //        flag << table_name;
        //        flag.close();
        //        auto data_file = files_[table_name].data_file;
        //        RestoreFromTemp(temp.get(), data_file, table_data[table_name]->record_size);
        //        std::remove(C::FLAG_FILE.c_str());
        //        temp = std::make_shared<std::fstream>(C::TEMP_FILE,
        //                                              std::ios::binary | std::ios::out | std::ios::trunc |
        //                                              std::ios::in);

    } else {
        //        if (!flag.is_open()) {
        //            return 0;
        //        }
        //        std::string t_name;
        //        flag >> t_name;
        //        auto res = std::fstream(t_name + DIR_SEPARATOR + t_name + C::DATA_FILE_TYPE,
        //                                std::ios::binary | std::ios::in);
        //        if (res.is_open()) {
        //            RestoreFromTemp(temp.get(), &res, table_data[table_name]->record_size);
        //        }
        //        flag.close();
        //        std::remove(C::FLAG_FILE.c_str());
        //        temp = std::make_shared<std::fstream>(C::TEMP_FILE,
        //                                              std::ios::binary | std::ios::out | std::ios::trunc |
        //                                              std::ios::in);
    }
}

FileManager::FileManager(std::shared_ptr<TransactManager> manager) : transact_manager_(manager) {
    //    temp = std::make_shared<std::fstream>(C::TEMP_FILE,
    //                                          std::ios::binary | std::ios::out | std::ios::trunc | std::ios::in);
    //    UpdateFile("");
}
FileManager::~FileManager() = default;
void FileManager::Clear(size_t transaction_id) {
    fs::create_directories(C::TEMP_DIR);
    for (const auto& file : fs::directory_iterator(C::TEMP_DIR)) {
        size_t file_id = 0;
        std::string file_name = file.path().string();
        file_name.erase(file_name.begin(), file_name.begin() + C::TEMP_DIR.size() + 1);
        std::string file_id_str = file_name.substr(0, file_name.find('_'));
        file_id = std::stoi(file_id_str);
        if (file_id == transaction_id) {
            fs::remove(file.path());
        }
    }
}
