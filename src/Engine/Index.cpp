#include "Headers/Index.h"

bool comp_INT(const std::string& lhs, const std::string& rhs) { return std::stoi(lhs) < std::stoi(rhs); };
bool comp_FLOAT(const std::string& lhs, const std::string& rhs) { return std::stod(lhs) < std::stod(rhs); };
bool comp_CHAR(const std::string& lhs, const std::string& rhs) { return lhs < rhs; };
std::vector<std::function<bool(const std::string& lhs, const std::string& rhs)>> comparator = {comp_INT, comp_FLOAT,
                                                                                               comp_CHAR};

Indexes::Indexes(Type t) : val_type_(t), indexes_(IndexComp(t)) {}
void Indexes::Insert(std::string value, int pos) { indexes_.emplace(value, pos); }
std::multimap<std::string, int, cmp> Indexes::GetIndexes() { return indexes_; }
bool IndexComp::operator()(const std::string& lhs, const std::string& rhs) { return comparator[type](lhs, rhs); }
