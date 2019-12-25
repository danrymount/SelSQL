//
// Created by quiks on 25.12.2019.
//

#ifndef SELSQL_INDEX_H
#define SELSQL_INDEX_H
#include "../../Utils/Headers/CommonUtils.h"
class Indexes {
    Type val_type_;
    std::multimap<std::string, int> indexes_;

   public:
    explicit Indexes(Type t);
    void Insert(std::string value, int pos);
    std::multimap<std::string, int> GetIndexes();
};
#endif  // SELSQL_INDEX_H
