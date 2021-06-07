//
// Created by Umar.Farooq on 5/21/2021.
//

#ifndef FORWARDBLOCK_LEDGERFLAG_H
#define FORWARDBLOCK_LEDGERFLAG_H

#include "../utils/strencodings.h"
#include "../utils/mathencodings.h"
#include <tuple>
#include "spdlog/spdlog.h"

namespace Protocol
{
class LedgerFlag
{
   int m_dec;
   bool m_isCredit;

   public:
   LedgerFlag(const int dec, const int isCredit);
   LedgerFlag(const LedgerFlag&) = default;
   std::tuple<int, std::string, bool> dump();
   int dec();
   bool isCredit();
   bool isDebit();
};
}    // namespace Protocol

#endif    // FORWARDBLOCK_LEDGERFLAG_H
