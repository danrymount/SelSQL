#include "Headers/FileManager.h"
#include <memory>

#include "../Utils/Structures/Data/DataBlock.h"

void FileManager::WriteTableMetaData(const std::shared_ptr<Table>& table) {
    //    if (meta_files_.find(table->name) == meta_files_.end() or !meta_files_[table->name].isOpen()) {
    //        std::cerr << __func__ << "\t File isn't opened" << std::endl;
    //        throw FileNotOpened();
    //    }

    auto meta_file = meta_files_[table->name];
    meta_file->seekp(0, std::ios::beg);
    buffer_data buffer = GetTableBuffer(table.get());
    meta_file->write(buffer.first, buffer.second);
    meta_file->flush();
    delete[] buffer.first;
}

void FileManager::ReadTableMetaData(const std::string& table_name) {
    //    if (meta_files_.find(table_name) == meta_files_.end() or !meta_files_[table_name].isOpen()) {
    //        std::cerr << __func__ << "\t File isn't opened" << std::endl;
    //        throw FileNotOpened();
    //    }
    auto meta_file = meta_files_[table_name];
    int size = GetFileSize(meta_file);
    char buffer[size];
    meta_file->read(buffer, size);
    table_data[table_name] = ReadTableFromBuffer(buffer);
}
files FileManager::OpenFile(const std::string& table_name, size_t transaction_id) {
    const std::string& directory = table_name;
    std::string file_name = table_name;
    if (!fs::exists(directory)) {
        return files();
    }
    std::string dist_file_name;
    std::string src_file_name = table_name + DIR_SEPARATOR + table_name + Constants::DATA_FILE_TYPE;
    if (transaction_id != 9999) {
        dist_file_name = FindTempFile(table_name, transaction_id);
    }
    if (dist_file_name.empty() and transaction_id != 9999) {
        dist_file_name = ConstructFileName(table_name, transaction_id);
    } else {
        src_file_name = dist_file_name;
    }
    std::cerr << dist_file_name << std::endl;
    auto meta_file = new std::fstream(table_name + DIR_SEPARATOR + table_name + Constants::META_FILE_TYPE,
                                      std::ios::in | std::ios::out | std::ios::binary);
    if (!meta_file->is_open()) {
        return files();
    }
    meta_files_[table_name] = meta_file;
    ReadTableMetaData(table_name);
    int flag = 0;
    if (src_file_name == dist_file_name and transaction_id != 9999) {
        flag = 1;
        try {
            fs::copy_file(table_name + DIR_SEPARATOR + table_name + Constants::DATA_FILE_TYPE, src_file_name);
        } catch (std::exception) {
        }
    }
    if (flag) {
        return files(meta_file,
                     std::make_shared<std::fstream>(src_file_name, std::ios::in | std::ios::out | std::ios::binary),
                     std::make_shared<std::fstream>(dist_file_name, std::ios::in | std::ios::out | std::ios::binary));
    } else {
        return files(meta_file,
                     std::make_shared<std::fstream>(src_file_name, std::ios::in | std::ios::out | std::ios::binary),
                     std::make_shared<std::fstream>(dist_file_name,
                                                    std::ios::in | std::ios::out | std::ios::binary | std::ios::trunc));
    }
}
int FileManager::CreateFile(const std::shared_ptr<Table>& table) {
    std::string table_name = table->name;
    //    this->CloseAllFiles();
    if (!fs::create_directory(table->name)) {
        return 1;
    }

    auto dbFile = DB_FILE(new std::fstream(table_name + DIR_SEPARATOR + table_name + Constants::META_FILE_TYPE,
                                           std::ios::in | std::ios::out | std::ios::binary | std::ios::trunc),
                          new std::fstream(table_name + DIR_SEPARATOR + table_name + Constants::DATA_FILE_TYPE,
                                           std::ios::in | std::ios::out | std::ios::binary | std::ios::trunc));
    meta_files_[table_name] = dbFile.meta_file;
    WriteTableMetaData(table);
    //    CloseAllFiles();
    return 0;
}
std::shared_ptr<Table> FileManager::GetTable(const std::string& table_name) { return table_data[table_name]; }

int FileManager::DeleteFile(const std::string& table_name) {
    //    this->CloseAllFiles();
    return !fs::remove_all(table_name);
}

int FileManager::WriteDataBlock(const std::shared_ptr<Table>& table, std::shared_ptr<DataBlock> data, int block_id,
                                std::shared_ptr<std::fstream> dist) {
    //    if (meta_files_.find(table->name) == meta_files_.end() or !meta_files_[table->name].isOpen()) {
    //        std::cerr << __func__ << "\t File isn't opened" << std::endl;
    //        throw FileNotOpened();
    //    }

    //    if (data != nullptr) {
    //        this->WriteDataBlockToTemp(std::string(table->name), data, block_id, dist);
    //    }
    int offset = GetFileSize(dist.get());
    offset = block_id * GetDataBlockSize(data.get());
    buffer_data buffer = GetDataBlockBuffer(data.get());
    dist->seekp(offset);
    //    dist->write(reinterpret_cast<char*>(&block_id), sizeof(block_id));
    dist->write(buffer.first, buffer.second);
    delete[] buffer.first;
    return 0;
}

std::shared_ptr<DataBlock> FileManager::ReadDataBlock(const std::string& table_name, int block_id,
                                                      const std::shared_ptr<std::fstream>& src, int record_size) {
    auto data_file = src.get();
    if (data_file == nullptr or !data_file->is_open()) {
        return nullptr;
    }

    if (!GetFileSize(data_file)) {
        return nullptr;
    }
    if (GetFileSize(data_file) <= GetDataBlockSize(record_size) * block_id) {
        return nullptr;
    }

    data_file->seekg(0, std::ios::beg);

    if (GetFileSize(data_file) < GetDataBlockSize(record_size)) {
        return nullptr;
    }
    data_file->seekg(GetDataBlockSize(record_size) * block_id, std::ios::beg);
    char data_buffer[GetDataBlockSize(record_size)];
    data_file->read(data_buffer, GetDataBlockSize(record_size));
    return ReadDataBlockFromBuffer(data_buffer, record_size);
}

int FileManager::UpdateFile(size_t transact_id) {
    auto flag = std::fstream(Constants::FLAG_FILE, std::ios::in | std::ios::out);

    if (!flag.is_open() and transact_id != 9999) {
        fs::create_directory(Constants::TEMP_DIR);
        for (auto file : fs::directory_iterator(Constants::TEMP_DIR)) {
            std::string t_name = FindTempFile("", transact_id);
            if (!t_name.empty()) {
                auto temp_file = new std::fstream(t_name, std::ios::in | std::ios::out | std::ios::binary);
                std::string table_name = t_name.substr(t_name.find('_') + 1, t_name.find('.') - t_name.find('_') - 1);

                if (GetFileSize(temp_file) == 0) {
                    return 0;
                }
                auto data_file = new std::fstream(table_name + DIR_SEPARATOR + table_name + Constants::DATA_FILE_TYPE,
                                                  std::ios::in | std::ios::out | std::ios::binary | std::ios::trunc);
                RestoreFromTemp(temp_file, data_file, table_data[table_name]->record_size);
                temp_file->close();
                std::remove(t_name.c_str());
            }
        }
        //        flag = std::fstream(Constants::FLAG_FILE, std::ios::in | std::ios::out | std::ios::trunc);
        //        flag << table_name;
        //        flag.close();

        //        std::remove(Constants::FLAG_FILE.c_str());
        //        temp = std::make_shared<std::fstream>(Constants::TEMP_FILE,
        //                                              std::ios::binary | std::ios::out | std::ios::trunc |
        //                                              std::ios::in);

    } else {
        if (!flag.is_open()) {
            return 0;
        }
        std::string t_name;
        flag >> t_name;
        auto res = std::fstream(t_name + DIR_SEPARATOR + t_name + Constants::DATA_FILE_TYPE,
                                std::ios::binary | std::ios::in);
        if (res.is_open()) {
            //            OpenFile(table_name,-1);
            //            RestoreFromTemp(temp.get(), &res, table_data[table_name]->record_size);
        }
        flag.close();
        std::remove(Constants::FLAG_FILE.c_str());
        //        temp = std::make_shared<std::fstream>(Constants::TEMP_FILE,
        //                                              std::ios::binary | std::ios::out | std::ios::trunc |
        //                                              std::ios::in);
    }
}

FileManager::FileManager() {
    //    temp = std::make_shared<std::fstream>(Constants::TEMP_FILE,
    //                                          std::ios::binary | std::ios::out | std::ios::trunc | std::ios::in);
    //    UpdateFile("");
}
FileManager::~FileManager() {}
void FileManager::Clear(size_t transaction_id) {
    fs::create_directories(Constants::TEMP_DIR);
    for (const auto& file : fs::directory_iterator(Constants::TEMP_DIR)) {
        size_t file_id = 0;
        std::string file_name = file.path().string();
        file_name.erase(file_name.begin(), file_name.begin() + Constants::TEMP_DIR.size() + 1);
        std::string file_id_str = file_name.substr(0, file_name.find('_'));
        file_id = std::stoi(file_id_str);
        if (file_id == transaction_id) {
            fs::remove(file.path());
        }
    }
}
