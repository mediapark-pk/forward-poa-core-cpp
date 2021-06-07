//
// Created by Mahnoor on 1/28/2021.
//

#ifndef FORWARDBLOCK_CPP_LEDGERENTRIES_H
#define FORWARDBLOCK_CPP_LEDGERENTRIES_H

#include <tuple>
#include <vector>

#include "../../protocolconstants.h"
#include "spdlog/spdlog.h"

namespace Protocol
{
class LedgerEntry;
class LedgerEntries
{
   std::vector<std::vector<LedgerEntry>> m_batches;
   int m_leCount;

   public:
   void purge();
   void addBatch(const std::vector<LedgerEntry>& entries);
   std::tuple<int, int, std::vector<std::vector<LedgerEntries>>> dump();
   std::vector<std::vector<Protocol::LedgerEntry>> batches();
   int batchCount() const;
   int entriesCount() const;
   std::string serializeBatches();
};
}    // namespace Protocol


#endif    // FORWARDBLOCK_CPP_LEDGERENTRIES_H
