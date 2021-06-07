//
// Created by umar on 5/20/21.
//

#include "txflags.h"
#include "../abstractprotocolchain.h"

Protocol::TxFlags::TxFlags(Protocol::AbstractProtocolChain* p) : m_p{p}, m_count{0} {}

Protocol::LedgerFlags Protocol::TxFlags::ledgerFlags()
{
   return m_ledgerFlags;
}

Protocol::TxFlags Protocol::TxFlags::append(Protocol::AbstractTxFlag* flag)
{
   if (flag->id()) {
      spdlog::error(
         "TxFlags::{} Tx Flag {} is already registered",
         __FUNCTION__,
         Math::Encode_UInt1LE(flag->id()));
      throw std::invalid_argument(
         "Tx Flag " + Math::Encode_UInt1LE(flag->id()) + " is already registered");
   }
   if (!flag->name().empty()) {
      spdlog::error(
         "TxFlags::{} Tx Flag {} is already registered",  __FUNCTION__, flag->name());
      throw std::invalid_argument("Tx Flag " + flag->name() + " is already registered");
   }
   m_flags[flag->id()] = flag;
   m_namesMap[flag->name()] = flag;
   ++m_count;
   return *this;
}

bool Protocol::TxFlags::hasName(const std::string& name)
{
   if (!Validator::isValidTxFlagName(name)) {
      spdlog::error("TxFlags::{} Invalid tx flag name to search",  __FUNCTION__);
      throw std::invalid_argument("Invalid tx flag name to search");
   }
   return m_namesMap.find(name) != m_namesMap.end();
}

Protocol::AbstractTxFlag* Protocol::TxFlags::getFlag(int id)
{
   /**TODO ::**/
   //  if (!$this->has($id)) {
   //            throw new TxFlagException("Cannot find TxFlag with ID "+std::to_string(id)+"
   //            "+Math::Encode_UInt1LE(id));
   //        }

   return m_flags[id];
}

Protocol::AbstractTxFlag* Protocol::TxFlags::getWithName(const std::string& name)
{
   if (hasName(name)) {
      spdlog::error("TxFlags::{} Txflags::append, Cannot find {} TxFlag",  __FUNCTION__, name);
      throw std::invalid_argument("Txflags::append, Cannot find " + name + " TxFlag");
   }
   return m_namesMap[name];
}
