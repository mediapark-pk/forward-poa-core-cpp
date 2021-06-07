//
// Created by umar on 28/5/21.
//
#include "transactionconstructor.h"


Protocol::TransactionConstructor::TransactionConstructor(
   Protocol::AbstractProtocolChain* p, int ver, Protocol::AbstractTxFlag* flag, int epoch)
   : AbstractTxConstructor(p, ver, flag, epoch)
{
}

void Protocol::TransactionConstructor::beforeSerialize() {}
Protocol::TransactionConstructor* Protocol::TransactionConstructor::sendToRecipient(
   Protocol::PublicKey recipient)
{
   // TODO verify when maria is done 28/5
   m_recipientPubKey = recipient;
   auto v = recipient.getHash160();
   m_recipient = hexToBin({v.begin(), v.end()});
   return this;
}

Protocol::TransactionConstructor* Protocol::TransactionConstructor::setData(Buffer::Binary data)
{
   m_data = data;
   return this;
}

Protocol::TransactionConstructor* Protocol::TransactionConstructor::addTransfer(
   int value, std::string assetId)
{
   if (assetId != "" && !Validator::isValidAssetID(assetId)) {
      spdlog::error("TransactionConstructor::{} Invalid asset identifier",  __FUNCTION__);
      throw TxConstructException("Invalid asset identifier");
   }

   if (value < 0 || value > ProtocolConstants::MAX) {
      spdlog::error("TransactionConstructor::{} Invalid transfer amount",  __FUNCTION__);
      throw TxConstructException("Invalid transfer amount");
   }

   if (m_transfers.size() >= AbstractProtocolChain::MAX_TRANSACTION_SIZE) {
      spdlog::error(
         "TransactionConstructor::{} Transaction cannot have more then 10 asset transfers",
         __FUNCTION__);
      throw TxConstructException("Transaction cannot have more then 10 asset transfers");
   }

   m_transfers[assetId] = value;
   return this;
};
