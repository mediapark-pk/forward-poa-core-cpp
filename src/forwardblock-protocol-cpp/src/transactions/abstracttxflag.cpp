//
// Created by umar on 5/20/21.
//
#include "abstracttxflag.h"
#include "../abstractprotocolchain.h"

Protocol::AbstractTxFlag::AbstractTxFlag(
   Protocol::AbstractProtocolChain& p, const int id, const std::string& name)
   : m_p{&p}, m_id{id}, m_name{name}
{
   if (id < 0 || id > 0xffff) {
      spdlog::error("AbstractTxFlag::{} Invalid flag ID/decimal", __FUNCTION__);
      throw std::out_of_range("Invalid flag ID/decimal");
   }
   if (!Validator::isValidTxFlagName(name)) {
      spdlog::error("AbstractTxFlag::{} Invalid flag name",  __FUNCTION__);
      throw std::invalid_argument("Invalid flag name");
   }
}

int Protocol::AbstractTxFlag::id()
{
   return m_id;
}

std::string Protocol::AbstractTxFlag::name()
{
   return m_name;
}
