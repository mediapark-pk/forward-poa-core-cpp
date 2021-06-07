//
// Created by Umar.Farooq on 5/21/2021.
//

#include "ledgerflag.h"

Protocol::LedgerFlag::LedgerFlag(const int dec, const int isCredit)
{
   if (dec < 0 || dec > 0xffff) {
      spdlog::error(
         "LedgerFlag::{} Ledger flag cannot exceed 2 bytes",
         __FUNCTION__);
      throw std::out_of_range("Ledger flag cannot exceed 2 bytes");
   }
   m_dec = dec;
   m_isCredit = m_isCredit;
}

std::tuple<int, std::string, bool> Protocol::LedgerFlag::dump()
{
   return std::make_tuple(m_dec, binToHex(Math::Encode_UInt2LE(m_dec)), m_isCredit);
}

int Protocol::LedgerFlag::dec()
{
   return m_dec;
}

bool Protocol::LedgerFlag::isCredit()
{
   return m_isCredit;
}

bool Protocol::LedgerFlag::isDebit()
{
   return !m_isCredit;
}
