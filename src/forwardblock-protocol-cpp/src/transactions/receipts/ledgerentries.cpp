//
// Created by Mahnoor on 1/28/2021.
//

#include "ledgerentries.h"
#include "ledgerentry.h"


void Protocol::LedgerEntries::purge() {}
void Protocol::LedgerEntries::addBatch(const std::vector<LedgerEntry>& entries)
{
   if (m_leCount + entries.size() > ProtocolConstants::MAX_LEDGER_ENTRIES) {
      spdlog::error(
         "LedgerEntries::{} Tx receipt cannot contain more than "
          +std::to_string(ProtocolConstants::MAX_LEDGER_ENTRIES)+" ledger entries",
         __FUNCTION__);
      std::domain_error(
         "Tx receipt cannot contain more than " +
         std::to_string(ProtocolConstants::MAX_LEDGER_ENTRIES) + " ledger entries");
   }
   m_batches.push_back(entries);
   m_leCount += entries.size();
}
std::tuple<int, int, std::vector<std::vector<Protocol::LedgerEntries>>> Protocol::LedgerEntries::dump()
{
   std::vector<Protocol::LedgerEntries> batches;
   for (const auto& batch : m_batches) {
   }
}
std::vector<std::vector<Protocol::LedgerEntry>> Protocol::LedgerEntries::batches()
{
   return m_batches;
}
int Protocol::LedgerEntries::batchCount() const
{
   return m_batches.size();
}
int Protocol::LedgerEntries::entriesCount() const
{
   return m_leCount;
}
std::string Protocol::LedgerEntries::serializeBatches()
{
   std::string ser = Math::Encode_UInt1LE(m_batches.size());
   for (auto& batch : m_batches) {
      ser.append(Math::Encode_UInt1LE(batch.size()));
      for (auto& entry : batch) {
         ser.append(entry.serializeRawBytes());
      }
   }
   return ser;
}
