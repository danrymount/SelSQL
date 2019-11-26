
#include "Headers/TransactManager.h"

TransactManager::TransactManager() = default;
size_t TransactManager::GetTransactionId() { return new_transaction_id++; }