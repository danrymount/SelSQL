#include "Headers/Index.h"
Indexes::Indexes(Type t) : val_type_(t), indexes_(IndexComp(t)) {}
void Indexes::Insert(std::string value, int pos) { indexes_.emplace(value, pos); }
std::multimap<std::string, int, cmp> Indexes::GetIndexes() {
    return indexes_;
}
