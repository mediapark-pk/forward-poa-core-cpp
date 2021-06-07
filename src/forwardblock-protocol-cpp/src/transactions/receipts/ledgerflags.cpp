//
// Created by umar on 5/20/21.
//

#include "ledgerflags.h"

int Protocol::LedgerFlags::count() const
{
   return m_flags.size();
}

void Protocol::LedgerFlags::append(const int dec, bool isCredit)
{
   m_flags.insert({dec, Protocol::LedgerFlag(dec, isCredit)});
}

bool Protocol::LedgerFlags::has(const int dec)
{
   return m_flags.find(dec) != m_flags.end();
}

Protocol::LedgerFlag Protocol::LedgerFlags::get(int dec)
{
   if (has(dec)) {
      spdlog::error(
         "LedgerFlags::{} Invalid ledger flag", __FUNCTION__);

      throw std::out_of_range("Invalid ledger flag");
   }
   return m_flags.at(dec);
}
