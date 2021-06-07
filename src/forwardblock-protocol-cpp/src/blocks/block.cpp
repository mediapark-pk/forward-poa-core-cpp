#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>

#include "block.h"


Protocol::Block Protocol::Block::Decode(AbstractProtocolChain* p, Buffer::Binary encoded, int heightContext)
{
   return Protocol::Block(p, encoded, heightContext);
}

Protocol::Block::Block(Protocol::AbstractProtocolChain* p, Buffer::Binary bytes, int heightContext)
   : Protocol::AbstractBlock{p}, m_raw{bytes}, m_hash{p->hash256(bytes)}
{
   if (bytes.sizeInBytes() > AbstractProtocolChain::MAX_BLOCK_SIZE) {
      spdlog::error(
         "Block::{} Encoded block of {} bytes exceeds limit of {} bytes",
         __FUNCTION__,
         bytes.sizeInBytes(),
         AbstractProtocolChain::MAX_BLOCK_SIZE);
      throw BlockDecodeException(
         "Encoded block of " + std::to_string(AbstractProtocolChain::MAX_BLOCK_SIZE) +
         " bytes exceeds limit of " + std::to_string(bytes.sizeInBytes()) + " bytes");
   }
   Buffer::Binary read = bytes;
   try {
      // Step 1
      pos = 0;
      m_version = Math::Decode_UInt1LE(read.getBufferData().substr(pos, 1));
      pos += 1;
      switch (m_version) {
      case 1:
         decodeBlockV1(read, heightContext);
         break;
      default:
         spdlog::error("Block::{} Unsupported block version", __FUNCTION__);
         throw BlockDecodeException("Unsupported block version");
      }
   } catch (BlockDecodeException e) {
      spdlog::error("Block::{} {}", __FUNCTION__, e.what());
      throw BlockDecodeException(e.what());
   }
}
void Protocol::Block::decodeBlockV1(Buffer::Binary read, int heightContext)
{
   // step 2

   auto timeStamp = Math::Decode_UInt4LE(read.getBufferData().substr(pos, 4));
   pos += 4;
   if (!Validator::isValidEpoch(timeStamp)) {
      spdlog::error("Block::{} Invalid TimeStamp",  __FUNCTION__);
      throw std::runtime_error("Invalid TimeStamp");
   }
   m_timeStamp = timeStamp;

   // Step 3
   m_prevBlockId = read.getBufferData().substr(pos, 32);
   pos += 32;

   // Step 4
   m_txCount = Math::Decode_UInt2LE(read.getBufferData().substr(pos, 4));
   pos += 4;

   // Step 6
   m_totalOut = Math::Decode_UInt8LE(read.getBufferData().substr(pos, 8));
   pos += 8;

   // Step 7
   m_totalFee = Math::Decode_UInt8LE(read.getBufferData().substr(pos, 8));
   pos += 8;

   // Step 8
   m_forger = read.getBufferData().substr(pos, 20);
   pos += 20;

   // Step 9
   auto signs = Math::Decode_UInt1LE(read.getBufferData().substr(pos, 1));
   pos += 1;
   if (signs > 5) {
      spdlog::error("Block::{} Blocks cannot have more than 5 signatures",  __FUNCTION__);
      throw BlockDecodeException("Blocks cannot have more than 5 signatures");
   }

   int v;
   Signature sign = Signature(Buffer::Base16(""), Buffer::Base16(""), (int&)v);
   if (signs > 0) {
      for (int i = 1; i <= signs; i++) {    // Step 9.1
         try {
            Buffer::Base16 signR = Buffer::Base16(binToHex(read.getBufferData().substr(pos, 32)));
            pos += 32;
            Buffer::Base16 signS = Buffer::Base16(binToHex(read.getBufferData().substr(pos, 32)));
            pos += 32;
            int signV = Math::Decode_UInt1LE(binToHex(read.getBufferData().substr(pos, 1)));
            pos += 1;
            auto s = Signature(signR, signS, signV);
            sign = s;

         } catch (Exception e) {
            spdlog::error("Block::{} Error with signature {} {} {}",  __FUNCTION__);
            throw BlockDecodeException("Error with signature"+std::to_string(i)+e.what());
         }
         m_signs.push_back(sign);
      }
   }

   // Step 10
   m_reward = Math::Decode_UInt8LE(read.getBufferData().substr(pos, 8));
   pos += 8;

   // Step 11
   m_merkleTx = read.getBufferData().substr(pos, 32);
   pos += 32;

   // Step 12
   m_merkleTxReceipts = read.getBufferData().substr(pos, 32);
   pos += 32;

   // Verify Step 11 and 12
   if (m_txCount) {
      std::string nullMerkleTree(32, '\0');
      if (m_merkleTx != nullMerkleTree) {
         spdlog::error("Block::{} Block merkle tx root must be all NULL bytes with txCount 0",  __FUNCTION__);
         throw BlockDecodeException("Block merkle tx root must be all NULL bytes with txCount 0");
      }
      if (m_merkleTxReceipts != nullMerkleTree) {
         spdlog::error("Block::{} Block merkle tx receipts root must be all NULL bytes with txCount 0",  __FUNCTION__);
         throw BlockDecodeException("Block merkle tx receipts root must be all NULL bytes with txCount 0");
      }
   }

   // Step 13
   m_bodySize = Math::Decode_UInt4LE(read.getBufferData().substr(pos, 4));
   pos += 1;

   // Step 14
   if (read.getBufferData().substr(pos, 1) != "\0") {
      spdlog::error("Block::{} Invalid block headers separator",  __FUNCTION__);
      throw BlockDecodeException("Invalid block headers separator");
   }

   // Step 15
   if (m_txCount > 0) {
      for (int i = 0; i < m_txCount; i++) {
         // step 15.1
         auto serTxLen = Math::Decode_UInt2LE(read.getBufferData().substr(pos, 2));
         pos += 2;

         // step15.2
         auto serTx = read.getBufferData().substr(serTxLen);
         AbstractPreparedTx* blockTx;
         try {
            m_rawTxs.push_back(serTx);
            // TODO DecodeAs function in transaction class
            blockTx = Protocol::Transaction::DecodeAs(AbstractBlock::m_p, Buffer::Binary(serTx));
            // TODO Append function in blocktxs class
             m_txs.append(blockTx);
         } catch (Exception e) {
            spdlog::error("Block::{} Failed to decode transaction at index {}",  __FUNCTION__, i);
            throw std::runtime_error("Failed to decode transaction at index %d"+std::to_string(i));
         }

         // Step 15.3
         auto serTxRLen = Math::Decode_UInt2LE(read.getBufferData().substr(pos, 2));
         pos += 2;

         // Step 15.4
         auto serTxR = read.getBufferData().substr(serTxRLen);
         try {
            m_rawTxReceipts.push_back(serTxR);
            // TODO DecodeReciept function in transaction class
            auto blockTxR = m_p->txFlags().get()->getFlag(blockTx->flag())->decodeReceipt(blockTx,Buffer::Binary(serTxR) );
            // Buffer::Binary(serTxR), heightContext);
            // TODO append function in BlockTxReceipt class
             m_txsReceipts.append(blockTxR);

         } catch (Exception e) {

         }
         spdlog::error("Block::{} Failed to decode tx receipt at index {}",  __FUNCTION__, i);
         throw BlockDecodeException("Failed to decode tx receipt at index "+std::to_string(i));
      }
   }
   // Check remaining bytes?
   if (read.getBufferData().at(pos) != '\0') {
      spdlog::error("Block::{} Block byte reader has excess bytes",  __FUNCTION__);
      throw BlockDecodeException("Block byte reader has excess bytes");
   }
}

// std::vector<std::string> Protocol::Block::array(bool getRawTx)
//{
//    std::vector<std::string> partialBlock;
//    partialBlock.at("hash") = m_hash.base16().hexits(false);
//
//    return std::vector<std::string>();
// }


const Buffer::Binary& Protocol::Block::getMHash() const
{
   return m_hash;
}
const Buffer::Binary& Protocol::Block::getMRaw() const
{
   return m_raw;
}
const std::vector<std::string>& Protocol::Block::getMRawTxs() const
{
   return m_rawTxs;
}
const std::vector<std::string>& Protocol::Block::getMRawTxReceipts() const
{
   return m_rawTxReceipts;
}
