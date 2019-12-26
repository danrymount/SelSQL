//
// Created by quiks on 25.12.2019.
//

#ifndef SELSQL_INDEX_H
#define SELSQL_INDEX_H
#include <functional>
#include "../../Utils/Headers/CommonUtils.h"
using cmp = std::function<bool(const std::string&, const std::string&)>;
struct IndexComp {
    Type type;
    explicit IndexComp(Type t) : type(t){};
    bool operator()(const std::string& lhs, const std::string& rhs);
};
class Indexes {
    Type val_type_;
    std::multimap<std::string, int, cmp> indexes_;

   public:
    explicit Indexes(Type t);
    void Insert(std::string value, int pos);
    std::multimap<std::string, int, cmp> GetIndexes();
};
#endif  // SELSQL_INDEX_H
