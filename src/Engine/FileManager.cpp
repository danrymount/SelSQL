#include "Headers/FileManager.h"
#include <memory>

#include "../Utils/Structures/Data/DataBlock.h"

void FileManager::WriteTableMetaData(const std::shared_ptr<Table>& table) {
    if (files_.find(table->name) == files_.end() or !files_[table->name].isOpen()) {
        std::cerr << __func__ << "\t File isn't opened" << std::endl;
        throw FileNotOpened();
    }

    std::cerr << table->record_amount << std::endl;
    auto meta_file = files_[table->name].meta_file;
    meta_file->seekp(0, std::ios::beg);
    buffer_data buffer = GetTableBuffer(table.get());
    meta_file->write(buffer.first, buffer.second);
    meta_file->flush();
    delete[] buffer.first;
}

void FileManager::ReadTableMetaData(const std::string& table_name) {
    if (files_.find(table_name) == files_.end() or !files_[table_name].isOpen()) {
        std::cerr << __func__ << "\t File isn't opened" << std::endl;
        throw FileNotOpened();
    }
    auto meta_file = files_[table_name].meta_file;
    int size = GetFileSize(meta_file);
    char buffer[size];
    meta_file->read(buffer, size);
    table_data[table_name] = ReadTableFromBuffer(buffer);
}
int FileManager::OpenFile(const std::string& table_name) {
    const std::string& directory = table_name;
    std::string file_name = table_name;
    if (!fs::exists(directory)) {
        return 1;
    }
    auto dbFile = DB_FILE(new std::fstream(table_name + DIR_SEPARATOR + table_name + Constants::META_FILE_TYPE,
                                           std::ios::in | std::ios::out | std::ios::binary),
                          new std::fstream(table_name + DIR_SEPARATOR + table_name + Constants::DATA_FILE_TYPE,
                                           std::ios::in | std::ios::out | std::ios::binary));
    if (!dbFile.isOpen()) {
        dbFile.close();
        return 1;
    }
    files_[table_name] = dbFile;
    ReadTableMetaData(table_name);
    return 0;
}
int FileManager::CreateFile(const std::shared_ptr<Table>& table) {
    std::string table_name = table->name;
    this->CloseAllFiles();
    if (!fs::create_directory(table->name)) {
        return 1;
    }

    auto dbFile = DB_FILE(new std::fstream(table_name + DIR_SEPARATOR + table_name + Constants::META_FILE_TYPE,
                                           std::ios::in | std::ios::out | std::ios::binary | std::ios::trunc),
                          new std::fstream(table_name + DIR_SEPARATOR + table_name + Constants::DATA_FILE_TYPE,
                                           std::ios::in | std::ios::out | std::ios::binary | std::ios::trunc));
    files_[table_name] = dbFile;
    WriteTableMetaData(table);
    CloseAllFiles();
    return 0;
}
std::shared_ptr<Table> FileManager::GetTable(const std::string& table_name) { return table_data[table_name]; }

int FileManager::DeleteFile(const std::string& table_name) {
    this->CloseAllFiles();
    return !fs::remove_all(table_name);
}

int FileManager::UpdateFile(const std::shared_ptr<Table>& table, const std::vector<std::shared_ptr<DataBlock>>& data) {
    if (files_.find(table->name) == files_.end() or !files_[table->name].isOpen()) {
        std::cerr << __func__ << "\t File isn't opened" << std::endl;
        throw FileNotOpened();
    }

    this->WriteTableMetaData(table);
    if (data.empty()) {
        return 0;
    }

    this->WriteDataBlocks(std::string(table->name), data);
    CloseAllFiles();
    return 0;
}

std::vector<std::shared_ptr<DataBlock>> FileManager::ReadDataBlocks(const std::string& table_name) {
    if (files_.find(table_name) == files_.end() or !files_[table_name].isOpen()) {
        std::cerr << __func__ << "\t File isn't opened" << std::endl;
        throw FileNotOpened();
    }
    std::vector<std::shared_ptr<DataBlock>> data;
    int readed_data = 0;
    int offset = 0;
    auto data_file = files_[table_name].data_file;

    if (!GetFileSize(data_file)) {
        return data;
    }
    data_file->seekg(std::ios::beg);
    int v = ReadIntFromFile(data_file);
    offset += 4;
    auto table = table_data[table_name];
    table->record_amount = v;
    std::cerr << table->record_amount << std::endl;
    if (table->record_amount == 0) {
        auto dataBlock = std::make_shared<DataBlock>();
        dataBlock->record_size = table->record_size;
        dataBlock->max_deleted_amount = Constants::DATA_SIZE / table->record_size;
        dataBlock->setDeletedPos(new char[dataBlock->max_deleted_amount * sizeof(short int)]);
        data.emplace_back(dataBlock);
        return data;
    }
    while (readed_data < table->record_amount) {
        data_file->seekg(offset, std::ios::beg);
        char data_buffer[CalcDataBlockSize(table->record_size)];
        data_file->read(data_buffer, CalcDataBlockSize(table->record_size));
        data.emplace_back(ReadDataBlockFromBuffer(data_buffer, table->record_size));
        readed_data += data[data.size() - 1]->record_amount;
        offset += CalcDataBlockSize(table->record_size);
    }

    return data;
}
void FileManager::WriteDataBlocks(const std::string& table_name, const std::vector<std::shared_ptr<DataBlock>>& data) {
    if (files_.find(table_name) == files_.end() or !files_[table_name].isOpen()) {
        std::cerr << __func__ << "\t File isn't opened" << std::endl;
        throw FileNotOpened();
    }
    auto data_file = files_[table_name].data_file;
    int offset = 0;
    data_file->seekp(offset, std::ios::beg);
    WriteIntToFile(data_file, table_data[table_name]->record_amount);
    offset += sizeof(int);
    for (const auto& block : data) {
        if (block->record_amount == 0) {
            continue;
        }
        buffer_data buffer = GetDataBlockBuffer(block.get());
        data_file->write(buffer.first, buffer.second);
        offset += buffer.second;
        delete[] buffer.first;
    }

    data_file->close();
}

void FileManager::CloseAllFiles() {
    for (auto& file : files_) {
        file.second.close();
    }
    files_.clear();
}
