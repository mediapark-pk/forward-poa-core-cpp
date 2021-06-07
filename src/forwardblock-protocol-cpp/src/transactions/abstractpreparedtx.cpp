//
// Created by umar on 5/20/21.
//

#include "abstractpreparedTx.h"


void Protocol::AbstractPreparedTx::decodeTxV1(Buffer::Binary read)
{
   int next = 0;
   // Step 2
   int flag = Math::Decode_UInt2LE(read.value().substr(next, 2));
   next += 2;
   m_flag = flag;

   // Step 3
   auto hasSender = read.value().substr(next, 1);
   next += 1;
   if (hasSender == "\1") {
      m_sender = (read.value().substr(next, 20));
      next += 20;
   } else if (hasSender != "\0") {
      spdlog::error("AbstractPreparedTx::{} Invalid " + hasSender + " flag byte",  __FUNCTION__);
      throw TxDecodeException("Invalid " + hasSender + " flag byte");
   }

   // Step 4
   int nonce = Math::Decode_UInt4LE(read.value().substr(next + 1, 4));
   next += 4;
   if (nonce && m_sender.empty()) {
      spdlog::error("AbstractPreparedTx::{} Transaction has nonce but no sender",  __FUNCTION__);
      throw TxDecodeException("Transaction has nonce but no sender");
      return;
   }
   m_nonce = nonce;

   // Step 5
   auto hasRecipient = read.value().substr(next, 1);
   next += 1;
   if (hasRecipient == "\1") {
      m_recipient = read.value().substr(next, 20);
      next += 20;
   } else if (hasRecipient != "\0") {
      spdlog::error(
         "AbstractPreparedTx::{} Invalid " + hasRecipient + " flag byte",  __FUNCTION__);
      throw TxDecodeException("Invalid " + hasRecipient + " flag byte");
   }

   // Step 6
   int memoLen = Math::Decode_UInt1LE(read.value().substr(next + 1, 1));
   next += 1;
   if (memoLen > 0) {
      if (memoLen > AbstractProtocolChain::MAX_TX_MEMO_LEN) {
         spdlog::error(
            "AbstractPreparedTx::{} Transaction memo of " + std::to_string(memoLen) + " bytes exceeds max size of " +
               std::to_string(AbstractProtocolChain::MAX_TX_MEMO_LEN) + " bytes",
            __FUNCTION__);
         throw TxDecodeException(
            "Transaction memo of " + std::to_string(memoLen) +
            " bytes exceeds max "
            "size of " +
            std::to_string(AbstractProtocolChain::MAX_TX_MEMO_LEN) + " bytes");
         return;
      }
      std::string memoRead = read.value().substr(next, memoLen);
      next += memoLen;
      m_memo = Validator::validatedMemo(memoRead);
      next += memoLen;
      /*try {
          m_memo = Validator::validatedMemo(read.substr(next + 1, memoLen));
          next += memoLen;
      } catch (//Insert exception) {
          //throw new TxDecodeException(
          //    sprintf('Invalid memo (%s) %s', get_class($e), $e->getMessage())
          //);
      }*/
   }

   // Step 7
   auto transfer = Math::Decode_UInt1LE(read.value(next, 1));
   next += 1;
   if (transfer > AbstractProtocolChain::MAX_TRANSFERS_PER_TX) {
      spdlog::error(
         "AbstractPreparedTx::{} ransaction cannot contain more than \" +\n"
         "         std::to_string(AbstractProtocolChain::MAX_TRANSFERS_PER_TX) + \" transfers",
         __FUNCTION__);
      throw TxDecodeException(
         "Transaction cannot contain more than " +
         std::to_string(AbstractProtocolChain::MAX_TRANSFERS_PER_TX) + " transfers");
   }
   if (transfer) {
      if (m_recipient.empty()) {
         spdlog::error(
            "AbstractPreparedTx::{} Transaction has transfer(s) with no recipient",  __FUNCTION__);
         throw TxDecodeException("Transaction has transfer(s) with no recipient");
      }
      for (int count = 0; count < m_transfers.size(); count++) {
         int amount = Math::Decode_UInt8LE(read.value(next, 8));
         next += 8;
         if (amount > ProtocolConstants::MAX) {
            spdlog::error(
               "AbstractPreparedTx::{} Transfer amount overflow at index \" + count",
               __FUNCTION__);
            throw TxDecodeException("Transfer amount overflow at index " + count);
         }
         auto hasAsset = read.value(next, 1);
         next += 1;
         std::string assetId;
         if (hasAsset == "\1") {
            std::string assetRead = read.value(next, 8);
            next += 8;
            Utils::ltrim(assetRead);
            if (Validator::isValidAssetID(assetId)) {
               spdlog::error(
                  "AbstractPreparedTx::{} Invalid transfer asset ID at index \" + count",
                  __FUNCTION__);
               throw TxDecodeException("Invalid transfer asset ID at index " + count);
            }
         } else if (hasAsset != "\0") {
            spdlog::error("AbstractPreparedTx::{} ",  __FUNCTION__);
            throw TxDecodeException("Invalid transfer.hasAsset flag at transferindex " + count);
         }
         m_transfers[(assetId)] = amount;
      }
   }

   // Step 8
   int dataLen = Math::Decode_UInt2LE(read.value().substr(next, 2));
   next += 2;
   if (dataLen > 0) {
      if (dataLen > AbstractProtocolChain::MAX_ARBITRARY_DATA) {
         spdlog::error(
            "AbstractPreparedTx::{} Arbitrary data of " + std::to_string(dataLen) + " bytes exceeds limit of" +
               std::to_string(AbstractProtocolChain::MAX_ARBITRARY_DATA) + " bytes",
            __FUNCTION__);
         throw TxDecodeException(
            "Arbitrary data of " + std::to_string(dataLen) + " bytes exceeds limit of " +
            std::to_string(AbstractProtocolChain::MAX_ARBITRARY_DATA) + " bytes");
      }
      m_data = Buffer::Binary({read.value().substr(next, dataLen)});
      next += dataLen;
   }

   // Step 9
   int signs = Math::Decode_UInt1LE(read.value().substr(next, 1));
   next += 1;
   if (signs > 5) {
      spdlog::error(
         "AbstractPreparedTx::{} Transaction cannot have more than 5 signatures", __FUNCTION__);
      throw TxDecodeException("Transaction cannot have more than 5 signatures");
   }
   if (signs > 0) {
      for (int count = 0; count <= signs; count++) {
         std::string signR = read.value(next, 32);
         signR = binToHex(signR);
         next += 32;
         std::string signS = read.value(next, 32);
         signS = binToHex(signS);
         next += 32;
         int signV = Math::Decode_UInt1LE(read.value().substr(next, 1));
         next += 1;
         Signature sign(signR, signS, signV);
         // TODO:: ADD TRY
         {
            std::string signR = read.value(next, 32);
            signR = binToHex(signR);
            next += 32;
            std::string signS = read.value(next, 32);
            signS = binToHex(signS);
            next += 32;
            int signV = Math::Decode_UInt1LE(read.value(next, 1));
            next += 1;
            Signature sign = Signature(signR, signS, signV);
            m_signs.push_back(sign);
            //            } //catch (/**insert exception**/) {
            //                throw new TxDecodeException(sprintf('Error with signature %d; (%s)
            //                                                    % s
            //                ', $i, get_class($e), $e->getMessage()));
         }
      }
   }

   // Step 10
   m_fee = Math::Decode_UInt8LE(read.value(next, 8));
   next += 8;

   // Step 11
   m_timeStamp = Math::Decode_UInt4LE(read.value(next, 4));
   next += 4;
   if (!Validator::isValidEpoch(m_timeStamp)) {
      spdlog::error("AbstractPreparedTx::{} nvalid timestamp",  __FUNCTION__);
      throw TxDecodeException("Invalid timestamp");
   }

   // Check remaining bytes
   if (!read.value().empty()) {
      spdlog::error(
         "AbstractPreparedTx::{} Transaction byte reader has excess bytes",  __FUNCTION__);
      throw TxDecodeException("Transaction byte reader has excess bytes");
   }
}

Protocol::AbstractPreparedTx* Protocol::AbstractPreparedTx::Decode(
   Protocol::AbstractPreparedTx* self, Protocol::AbstractProtocolChain* p, Buffer::Binary encoded)
{
}


Protocol::AbstractPreparedTx::AbstractPreparedTx(Protocol::AbstractProtocolChain* p, Buffer::Binary bytes)
   : m_hash(bytes)
{
}

Buffer::Binary Protocol::AbstractPreparedTx::hash()
{
   return m_hash;
}

Buffer::Binary Protocol::AbstractPreparedTx::raw()
{
   return m_raw;
}

int Protocol::AbstractPreparedTx::version()
{
   return m_version;
}

int Protocol::AbstractPreparedTx::flag()
{
   return m_flag;
}

std::string Protocol::AbstractPreparedTx::sender()
{
   return m_sender;
}

int Protocol::AbstractPreparedTx::nonce()
{
   return m_nonce;
}

std::string Protocol::AbstractPreparedTx::recipient()
{
   m_recipient;
}

std::string Protocol::AbstractPreparedTx::memo()
{
   m_memo;
}

std::map<std::string, int> Protocol::AbstractPreparedTx::transfers()
{
   return m_transfers;
}

std::string Protocol::AbstractPreparedTx::data()
{
   return m_data.raw();
}

std::vector<Protocol::Signature> Protocol::AbstractPreparedTx::signatures()
{
   return m_signs;
}

int Protocol::AbstractPreparedTx::fee()
{
   return m_fee;
}

int Protocol::AbstractPreparedTx::timeStamp()
{
   return m_timeStamp;
}
