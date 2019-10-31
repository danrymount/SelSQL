//
// Created by quiks on 31.10.2019.
//

#include "Headers/EngineUtils.h"
void WriteIntToFile(const std::shared_ptr<std::fstream>& file, int value) {
    file->write(reinterpret_cast<char*>(&value), sizeof(int));
}
int ReadIntoFromFile(const std::shared_ptr<std::fstream>& file) {
    int res;
    file->read(reinterpret_cast<char*>(&res), sizeof(int));
    return res;
}
int GetVersion(const std::string& file_name) {
    return std::stoi(file_name.substr(Constants::VERSION.size(), file_name.find('_')));
}

int CheckFileHashSum(const std::shared_ptr<DB_FILE>& file) {
    double saved_hash_sum = 0;
    file->meta_file->seekp(-Constants::MD_HASH_SUM, std::ios::end);
    file->meta_file->read(reinterpret_cast<char*>(&saved_hash_sum), Constants::MD_HASH_SUM);
    if (saved_hash_sum == CalcHashSum(file->data_file)) {
        return 1;
    }
    return 0;
}
double CalcHashSum(const std::shared_ptr<std::fstream>& file) {
    double res = 0;
    char c;
    file->seekg(std::ios::beg);
    while (file->get(c)) {
        res += (std::fmod(pow(res, 3), (double)7.0) + 131 * c) / 3;
    }
    file->clear();
    return res;
}
DB_FILE::DB_FILE(std::shared_ptr<std::fstream> m_file, std::shared_ptr<std::fstream> d_file, int ver)
                                                                                                    : meta_file(std::move(m_file)),
                                                                                                      data_file(std::move(d_file)),
                                                                                                      version(ver) {}
void DB_FILE::close() {
    meta_file->close();
    data_file->close();
}
int DB_FILE::isOpen() { return meta_file->is_open() and data_file->is_open(); }

std::shared_ptr<DB_FILE> FindLastVersion(const std::string& table_name, int max_v) {
    int max_ver = max_v;
    std::map<std::string, int> version_files = {};
    for (const auto& m_file : fs::directory_iterator(table_name)) {
        std::string name_without_type;
        std::string name = m_file.path();
        std::string type = name.substr(name.size() - Constants::DATA_FILE_TYPE.size(), name.size() - 1);
        if (type != Constants::META_FILE_TYPE) {
            continue;
        }

        name_without_type = name;
        name_without_type.erase(name_without_type.end() - Constants::DATA_FILE_TYPE.size(), name_without_type.end());
        name_without_type.erase(name_without_type.begin(), name_without_type.begin() + name_without_type.find('/') + 1);
        if (GetVersion(name_without_type) > max_ver) {
            fs::remove_all(m_file.path());
        }
        //        std::cerr << name_without_type << std::endl;
        version_files[name_without_type] = 0;
    }
    for (const auto& d_file : fs::directory_iterator(table_name)) {
        std::string name_without_type;
        std::string name = d_file.path();
        std::string type = name.substr(name.size() - Constants::DATA_FILE_TYPE.size(), name.size() - 1);
        if (type != Constants::DATA_FILE_TYPE) {
            continue;
        }
        name_without_type = name;
        name_without_type.erase(name_without_type.end() - Constants::DATA_FILE_TYPE.size(), name_without_type.end());
        name_without_type.erase(name_without_type.begin(), name_without_type.begin() + name_without_type.find('/') + 1);
        if (GetVersion(name_without_type) > max_ver) {
            fs::remove_all(d_file.path());
        }
        //        std::cerr << name_without_type << std::endl;
        if (version_files.find(name_without_type) != version_files.end()) {
            version_files[name_without_type] = 1;
        }
    }

    std::string last_ver_file;
    int last_ver = 0;
    for (const auto& i : version_files) {
        if (i.second) {
            if (last_ver < GetVersion(i.first)) {
                last_ver = GetVersion(i.first);
                last_ver_file = i.first;
            }
        }
    }

    std::string meta_file = table_name + '/' + last_ver_file + Constants::META_FILE_TYPE;
    std::string data_file = table_name + '/' + last_ver_file + Constants::DATA_FILE_TYPE;
    return std::make_shared<DB_FILE>(std::make_shared<std::fstream>(meta_file,
                                                                    std::ios::binary | std::ios::out | std::ios::in),
                                     std::make_shared<std::fstream>(data_file,
                                                                    std::ios::binary | std::ios::out | std::ios::in),
                                     last_ver);
}
