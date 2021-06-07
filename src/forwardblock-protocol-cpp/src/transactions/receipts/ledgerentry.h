//
// Created by Umar.Farooq on 5/21/2021.
//

#ifndef FORWARDBLOCK_LEDGERENTRY_H
#define FORWARDBLOCK_LEDGERENTRY_H

#include <cstdint>
#include <limits>
#include <tuple>
#include <vector>
#ifdef __WIN32
#   include <stdint.h>
#endif

#include "ledgerflag.h"
#include "spdlog/spdlog.h"

namespace Protocol
{
class AbstractProtocolChain;
class AbstractTxReceipt;

class LedgerEntry
{
   protected:
   AbstractProtocolChain* m_p;
   AbstractTxReceipt* m_txR;
   std::string m_hash160;
   LedgerFlag m_flag;
   int m_amount;
   std::string m_asset;
   bool m_applicable;

   public:
   LedgerEntry(
      Protocol::AbstractProtocolChain* p,
      Protocol::AbstractTxReceipt* txR,
      Protocol::LedgerFlag& lf,
      const std::string &hash160,
      int amount,
      const std::string& asset);
   std::tuple<std::tuple<int, std::string, bool>, std::string, int, std::string, bool> dump();
   AbstractTxReceipt* getReceipt();
   std::string account();
   LedgerFlag flag();
   int amount();
   std::string asset();
   void markApplicable();
   bool isApplicable();
   std::string serializeRawBytes();
};

}    // namespace Protocol


#endif    // FORWARDBLOCK_LEDGERENTRY_H
