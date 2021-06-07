//
// Created by Umar.Farooq on 5/21/2021.
//

#include "ledgerentry.h"
#include "../abstracttxreceipt.h"
#include "../abstractprotocolchain.h"
#include <stdexcept>


Protocol::LedgerEntry::LedgerEntry(
   Protocol::AbstractProtocolChain* p,
   Protocol::AbstractTxReceipt* txR,
   Protocol::LedgerFlag& lf,
   const std::string& hash160,
   int amount,
   const std::string& asset)
   : m_flag{lf}, m_hash160{hash160}, m_amount{amount}, m_asset{asset}
{
   if (hash160.length() != 20) {
      spdlog::error("LedgerEntry::{} Hash160 must be 20 raw bytes" ,__FUNCTION__ );
      std::invalid_argument("Hash160 must be 20 raw bytes");
   }
   if (amount < 0 || amount > ProtocolConstants::MAX) {
      spdlog::error("LedgerEntry::{} Invalid TxReceipt.ledgerEntry amount" ,__FUNCTION__ );
      std::invalid_argument("Invalid TxReceipt.ledgerEntry amount");
   }
}

std::tuple<std::tuple<int, std::string, bool>, std::string, int, std::string, bool> Protocol::LedgerEntry::dump()
{
   return std::make_tuple(m_flag.dump(), binToHex(m_hash160), m_amount, m_asset, m_applicable);
}
Protocol::AbstractTxReceipt* Protocol::LedgerEntry::getReceipt()
{
   return m_txR;
}
std::string Protocol::LedgerEntry::account()
{
   return m_hash160;
}
Protocol::LedgerFlag Protocol::LedgerEntry::flag()
{
   return m_flag;
}
int Protocol::LedgerEntry::amount()
{
   return m_amount;
}
std::string Protocol::LedgerEntry::asset()
{
   return m_asset;
}
void Protocol::LedgerEntry::markApplicable()
{
   m_applicable = true;
}
bool Protocol::LedgerEntry::isApplicable()
{
   return m_applicable;
}
std::string Protocol::LedgerEntry::serializeRawBytes()
{
   std::string ser = m_hash160 + Math::Encode_UInt2LE(m_flag.dec()) + Math::Encode_UInt8LE(m_amount);
   std::string padded = std::string((8 - m_asset.size()), '\0').append(m_asset.begin(), m_asset.end());
   ser.append(padded);
   ser.push_back(m_applicable ? '\1' : '\0');
   return ser;
}
