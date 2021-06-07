#include "abstracttx.h"

Protocol::AbstractTx::AbstractTx(Protocol::AbstractProtocolChain* p) : m_p{p} {}


Buffer::Binary Protocol::AbstractTx::hashPreImage(std::string chainId, int forkId)
{
   if (chainId != "") {
      if (!Protocol::Validator::isValidChainID(chainId)) {
         spdlog::error(
            "AbstractTx::{} Cannot generate hashPreImage; Invalid chain identifier",
            __FUNCTION__);

         throw TxEncodeException("Cannot generate hashPreImage; Invalid chain identifier");
      }
   } else {
      chainId = m_p->config()->getMChainId();
   }
   if (forkId < 0 || forkId > 0xff) {
      spdlog::error(
         "AbstractTx::{} Cannot generate hashPreImage; Invalid fork id",  __FUNCTION__);
      throw TxEncodeException("Cannot generate hashPreImage; Invalid fork id");
   } else {
      forkId = m_p->config()->getMForkId();
   }

   Buffer::Binary raw = serialize(false);
   raw.prepend(hexToBin(chainId))->prepend(Math::Encode_UInt1LE(forkId));

   auto ret = m_p->hash256(raw);
   return ret;
}

const Buffer::Binary Protocol::AbstractTx::serialize(bool includeSignatures)
{
   Buffer::Binary ser("");

   /** STEP 1 **/
   ser.append(Math::Encode_UInt1LE(m_version));

   /** STEP 2 **/
   ser.append(Math::Encode_UInt2LE(m_flag));

   /** STEP 3 **/
   if (!m_sender.empty()) {
      ser.setBufferData(ser.value() + '\1');
      ser.append(m_sender);
   } else {
      ser.setBufferData(ser.value() + '\0');
   }
   /** STEP 4 **/
   ser.append(Math::Encode_UInt4LE(m_nonce));

   /** STEP 5 **/
   if (!m_recipient.empty()) {
      ser.setBufferData(ser.value() + '\1');
      ser.append(m_recipient);
   } else {
      ser.setBufferData(ser.value() + '\0');
   }
   /** STEP 6 **/
   if (m_memo.empty()) {
      ser.append(Math::Encode_UInt1LE(m_memo.length()));
      ser.append(m_memo);
   } else {
      ser.setBufferData(ser.value() + '\0');
   }

   /** STEP 7 **/
   if (m_transfers.size() > 10) {
      spdlog::error(
         "AbstractTx::{} Transaction cannot have more than 10 asset transfers",
         __FUNCTION__);
      throw TxEncodeException("Transaction cannot have more than 10 asset transfers");
   }
   ser.append(Math::Encode_UInt1LE(m_transfers.size()));

   if (m_transfers.size() > 0) {
      if (m_recipient.empty()) {
         spdlog::error(
            "AbstractTx::{} Transaction with no recipient cannot have transfers",
            __FUNCTION__);
         throw TxEncodeException("Transaction with no recipient cannot have transfers");
      }
      auto tf = serializeTransfers();
      ser.append(tf.getBufferData());
   }

   /** STEP 8 **/
   if (m_data.sizeInBytes() != 0) {
      if (m_data.sizeInBytes() > AbstractProtocolChain::MAX_ARBITRARY_DATA) {
         spdlog::error(
            "AbstractTx::{} Transaction arbitrary data of size {} bytes exceeds limit of {} bytes",
            __FUNCTION__,
            m_data.sizeInBytes(),
            AbstractProtocolChain::MAX_ARBITRARY_DATA);
         throw TxEncodeException(
            "Transaction arbitrary data of size " + std::to_string(m_data.sizeInBytes()) +
            " bytes exceeds limit of " + std::to_string(AbstractProtocolChain::MAX_ARBITRARY_DATA) +
            " bytes");
      }
      ser.append(Math::Encode_UInt2LE(m_data.sizeInBytes()));
      ser.append(m_data.value(0, m_data.sizeInBytes()));

   } else {
      ser.setBufferData(ser.value() + '\0');    // todo test this
      ser.setBufferData(ser.value() + '\0');
   }
   /** STEP 9 **/
   if (includeSignatures) {
      if (m_signs.size() > 5) {
         spdlog::error(
            "AbstractTx::{} Transaction cannot have more than 5 signatures",  __FUNCTION__);
         throw TxEncodeException("Transaction cannot have more than 5 signatures");
      }
      ser.append(Math::Encode_UInt1LE(m_signs.size()));

      if (m_signs.size() > 0) {
         for (auto tempItr : m_signs) {
            ser.append(hexToBin(tempItr.r().hexits()));
            ser.append(hexToBin(tempItr.s().hexits()));
            ser.append(Math::Encode_UInt1LE(tempItr.v()));
         }
      }
   } else {
      ser.setBufferData(ser.value() + '\0');
   }

   /** Step 10 **/
   ser.append(Math::Encode_UInt8LE(m_fee));

   /** Step 11 **/
   ser.append(Math::Encode_UInt4LE(m_timeStamp));

   /** Step 12  by returning as const it is already read only**/
   return ser;
}

const Buffer::Binary Protocol::AbstractTx::serializeTransfers()
{
   Buffer::Binary ret("");
   for (auto& tf : m_transfers) {
      ret.append(Math::Encode_UInt8LE(tf.second));
      if (!tf.first.empty()) {
         ret.setBufferData(ret.value() + '\1');
         std::string padded =
            std::string((8 - tf.first.size()), '\0').append(tf.first.begin(), tf.first.end());
         ret.append(padded);
      } else {
         ret.setBufferData(ret.value() + '\0');
      }
   }
   return ret;
}
