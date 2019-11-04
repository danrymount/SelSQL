#include "Headers/FileManager.h"
#include <memory>

#include "../Utils/Structures/Data/DataBlock.h"

void FileManager::WriteTableMetaData(const std::shared_ptr<Table>& table) {
    auto meta_file = files_[table->name].meta_file;
    meta_file->seekp(0, std::ios::beg);
    buffer_data buffer = GetTableBuffer(table.get());
    meta_file->write(buffer.first, buffer.second);
    meta_file->flush();
    delete[] buffer.first;
}

void FileManager::ReadTableMetaData(const std::string& table_name) {

    auto meta_file = files_[table_name].meta_file;
    char buffer[Constants::MD_SIZE];
    meta_file->read(buffer,Constants::MD_SIZE);
    table_data[table_name] = ReadTableFromBuffer(buffer);
}
int FileManager::OpenFile(const std::string& table_name) {
    const std::string& directory = table_name;
    std::string file_name = table_name;
    if (!fs::exists(directory)) {
        return 1;
    }
    //    std::shared_ptr<DB_FILE> dbFile = FindLastVersion(table_name, 100);
    auto dbFile = DB_FILE(new std::fstream(table_name + DIR_SEPARATOR + table_name + Constants::META_FILE_TYPE,
                                           std::ios::in | std::ios::out | std::ios::binary),
                          new std::fstream(table_name + DIR_SEPARATOR + table_name + Constants::DATA_FILE_TYPE,
                                           std::ios::in | std::ios::out | std::ios::binary));
    if (!dbFile.isOpen()) {
        dbFile.close();
        return 1;
    }
    //    std::cout<<dbFile->meta_file->is_open();
    //    std::cout<<dbFile->data_file->is_open();
    files_[table_name] = dbFile;
    ReadTableMetaData(table_name);
    //    if (!CheckFileHashSum(dbFile)) {
    //        std::cout << "ОТКАТ" << std::endl;
    //        dbFile = FindLastVersion(table_name, dbFile->version - 1);
    //        files_[table_name] = dbFile;
    //        ReadTableMetaData(table_name);
    //    }

    return 0;
}
int FileManager::CreateFile(const std::shared_ptr<Table>& table) {
    std::string table_name = table->name;
    this->CloseAllFiles();
    int def_version = 1;
    if (!fs::create_directory(table->name)) {
        //        for (const auto& file : fs::directory_iterator(table->name)) {
        //            std::string name = file.path().string();
        //            name.erase(name.begin(), name.begin() + table->name.size() + 1);
        //            int version = std::stoi(name.substr(name.find(Constants::VERSION) + Constants::VERSION.size(),
        //                                                name.find('_')));
        //        }
        return 1;
    }
    //    std::string file_name = table->name + '/' + Constants::VERSION + std::to_string(def_version) + "_" +
    //    table->name;
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
    this->WriteTableMetaData(table);
    if (data.empty()) {
        return 0;
    }

    this->WriteDataBlocks(std::string(table->name), data);
    CloseAllFiles();
    return 0;
}

std::vector<std::shared_ptr<DataBlock>> FileManager::ReadDataBlocks(const std::string& table_name) {
    std::vector<std::shared_ptr<DataBlock>> data;
    int readed_data = 0;
    int offset = 0;
    // assert opened
    auto data_file = files_[table_name].data_file;
    data_file->seekg(std::ios::beg);
    auto table = table_data[table_name];
    if (table->record_amount == 0) {
        auto dataBlock = std::make_shared<DataBlock>();
        dataBlock->record_size = table->record_size;
        dataBlock->max_deleted_amount = Constants::DATA_SIZE / table->record_size;
        dataBlock->setDeletedPos(new char[dataBlock->max_deleted_amount * sizeof(short int)]);
        data.emplace_back(dataBlock);
        return data;
    }
    while (readed_data < table->record_amount) {
        auto new_data = new char[Constants::DATA_SIZE];
        auto dataBlock = std::make_shared<DataBlock>();
        data_file->seekg(offset, std::ios::beg);
        dataBlock->record_size = table->record_size;
        dataBlock->record_amount = ReadIntoFromFile(data_file);
        readed_data += dataBlock->record_amount;
        dataBlock->last_record_pos = ReadIntoFromFile(data_file);
        dataBlock->deleted = ReadIntoFromFile(data_file);
        dataBlock->max_deleted_amount = Constants::DATA_SIZE / table->record_size;
        char* deleted = new char[dataBlock->max_deleted_amount * sizeof(short int)];
        data_file->read(deleted, dataBlock->max_deleted_amount * sizeof(short int));
        dataBlock->setDeletedPos(deleted);
        offset += Constants::DATA_BLOCK_RECORD_AMOUNT + Constants::DATA_BLOCK_DELETED_AMOUNT +
                  Constants::DATA_BLOCK_RECORD_AMOUNT + Constants::DATA_BLOCK_RECORD_LAST_POS +
                  dataBlock->max_deleted_amount * sizeof(short int);
        //        std::cerr<<offset<<std::endl;
        data_file->seekg(offset, std::ios::beg);
        data_file->read(new_data, Constants::DATA_SIZE);
        offset += Constants::DATA_SIZE;
        dataBlock->setData(new_data);
        data.emplace_back(dataBlock);
    }

    return data;
}
void FileManager::WriteDataBlocks(const std::string& table_name, const std::vector<std::shared_ptr<DataBlock>>& data) {
    auto data_file = files_[table_name].data_file;

    int offset = 0;
    for (const auto& block : data) {
        if (block->record_amount == 0) {
            continue;
        }
        data_file->seekp(offset, std::ios::beg);
        WriteIntToFile(data_file, block->record_amount);
        WriteIntToFile(data_file, block->last_record_pos);
        WriteIntToFile(data_file, block->deleted);
        data_file->write(block->getDeletedPos(), block->max_deleted_amount * sizeof(short int));
        offset += Constants::DATA_BLOCK_RECORD_AMOUNT + Constants::DATA_BLOCK_DELETED_AMOUNT +
                  Constants::DATA_BLOCK_RECORD_AMOUNT + Constants::DATA_BLOCK_RECORD_LAST_POS +
                  block->max_deleted_amount * sizeof(short int);
        //                std::cerr<<offset<<std::endl;
        data_file->seekp(offset, std::ios::beg);
        data_file->write(block->data_, Constants::DATA_SIZE);

        offset += Constants::DATA_SIZE;
    }

    //    std::cout<<offset<<std::endl;
    data_file->close();
}

void FileManager::CloseAllFiles() {
    for (auto& file : files_) {
        file.second.close();
    }
    files_.clear();
}
