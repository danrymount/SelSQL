//
// Created by quiks on 25.12.2019.
//

#ifndef SELSQL_CACHEDBLOCKS_H
#define SELSQL_CACHEDBLOCKS_H
#include <algorithm>
#include <memory>
#include "../../Utils/Structures/Data/DataBlock.h"
const int MAX_SIZE = 10;
typedef std::pair<std::string, int> BlockPos;
class CachedBlocks {
    std::vector<std::tuple<std::shared_ptr<DataBlock>, BlockPos, int>> blocks_;

   public:
    void InsertBlock(const std::shared_ptr<DataBlock>& data, const BlockPos& pos);
    std::shared_ptr<DataBlock> GetBlock(const BlockPos& pos);
    void ClearTable(const std::string& table_name);
};
#endif  // SELSQL_CACHEDBLOCKS_H
