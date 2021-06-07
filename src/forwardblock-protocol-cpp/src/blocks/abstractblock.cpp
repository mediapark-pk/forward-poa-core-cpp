//
// Created by umar on 20/5/21.
//


#include "abstractblock.h"


Protocol::AbstractBlock::AbstractBlock(Protocol::AbstractProtocolChain* p) : m_p{p} {}


Buffer::Binary Protocol::AbstractBlock::hashPreImage(std::string chainId = "", int forkId = 0)
{
   if (chainId != "") {
      if (!Validator::isValidChainID(chainId)) {
         spdlog::error("AbstractBlock::{} Cannot generate hashPreImage; Invalid chain identifier",  __FUNCTION__);
         throw BlockEncodeException("Cannot generate hashPreImage; Invalid chain identifier");
      }
   } else {
      chainId = m_p->config()->getMChainId();
   }

   if (forkId < 0 || forkId > 0xff) {
      spdlog::error("AbstractBlock::{} Cannot generate hashPreImage; Invalid fork id",  __FUNCTION__);
      throw BlockEncodeException("Cannot generate hashPreImage; Invalid fork id");
   } else {
      forkId = this->m_p->config()->getMForkId();
   }

   Buffer::Binary raw = ser(false);
   raw.prepend(hexToBin(chainId))->prepend(Math::Encode_UInt1LE(forkId));

   auto ret = m_p->hash256(raw);
   return ret;
}
Buffer::Binary Protocol::AbstractBlock::ser(bool includeSignatures)
{
   Buffer::Binary ser("");

   /** Version Byte **/
   ser.append(Math::Encode_UInt1LE(m_version));

   /** STEP 2 **/
   ser.append(Math::Encode_UInt4LE(m_timeStamp));

   /** STEP 3 **/
   ser.append(m_prevBlockId);

   /** STEP 4 **/
   ser.append(Math::Encode_UInt2LE(m_txCount));


   /** STEP 5 **/
   ser.append(Math::Encode_UInt8LE(m_totalIn));

   /** STEP 6 **/
   ser.append(Math::Encode_UInt8LE(m_totalOut));

   /** STEP 7 **/
   ser.append(Math::Encode_UInt8LE(m_totalFee));


   /** STEP 8 **/
   ser.append(m_forger);

   /** STEP 9 **/
   if (includeSignatures) {
      auto signsCount = m_signs.size();
      if (m_signs.size() > 5)
      {
         spdlog::error("AbstractBlock::{} Forger account not specified",  __FUNCTION__);
         throw std::runtime_error("Forger account not specified");
      }


      ser.append(Math::Encode_UInt1LE(m_signs.size()));
      for (auto itr : m_signs) {
         ser.append(hexToBin(itr.r().hexits(false)));
         ser.append(hexToBin(itr.s().hexits(false)));
         ser.append(Math::Encode_UInt1LE(itr.v()));
      }
   } else {
      std::string str = "";
      str.push_back('\0');
      ser.append(str);
   }

   /** STEP 10 **/
   ser.append(Math::Encode_UInt8LE(m_reward));

   /** STEP 11 **/
   ser.append(m_merkleTx);

   /** STEP 12 **/
   ser.append(m_merkleTxReceipts);

   /** Work on body **/
   auto bodyBuffer = new Buffer::Binary;
   auto txsCount = m_txs.getMPreparedTxCount();
   auto txRCount = m_txsReceipts.getMTxReceiptTxCount();
   if (txsCount != txRCount) {
      spdlog::error("AbstractBlock::{} Transactions and receipts count does not match",  __FUNCTION__);
      throw BlockEncodeException("Transactions and receipts count does not match");
   }

   Buffer::Binary serTxR("");
   if (txsCount > 0) {
      // Step15
      for (int i = 0; i < txsCount; i++) {
         Buffer::Binary serTx = m_txs.index(i)->raw();
         try {
            serTxR = m_txsReceipts.index(i)->serialize();
         } catch (Exception e) {
            spdlog::error("AbstractBlock::{} TxReceipt {} {}",  __FUNCTION__, i, e.what());
            throw BlockEncodeException("TxReceipt "+ std::to_string(i) +" " +e.what() );
         }


         // Step 15.1
         Buffer::Binary bodyBuffer("");
         bodyBuffer.append(Math::Encode_UInt2LE(serTx.sizeInBytes()));

         // Step 15.2
         bodyBuffer.append(serTx.raw());

         // Step 15.3
         bodyBuffer.append(Math::Encode_UInt2LE(serTxR.sizeInBytes()));

         // Step 15.4
         bodyBuffer.append(serTxR.raw());
      }
   }

   // Step 13
   ser.append(Math::Encode_UInt4LE(bodyBuffer->sizeInBytes()));

   // Step 14
   std::string str = "";
   str.push_back('\0');
   ser.append(str);

   // Step 15
   if (bodyBuffer->sizeInBytes() > 0) {
      ser.append(bodyBuffer->raw());
   }
   return ser;
}
