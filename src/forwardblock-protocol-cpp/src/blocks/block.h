#ifndef BLOCKCHAIN_BLOCK_H
#define BLOCKCHAIN_BLOCK_H

#include <vector>
#include "../abstractprotocolchain.h"
#include "abstractblock.h"
#include "../accounts/accountproto.h"
#include "blocktxreceipts.h"
#include "blocktxs.h"
#include "../keypair/privatekey/signature.h"
#include "validator.h"
#include "spdlog/spdlog.h"
#include "../exceptions/blockdecodeexception.h"
#include "../transactions/abstractpreparedTx.h"
#include "../transactions/transaction.h"
#include "../transactions/txflags.h"


namespace Protocol
{
class Block : public Protocol::AbstractBlock
{
   protected:
   Block(AbstractProtocolChain* p, Buffer::Binary bytes, int heightContext);

   private:
   Buffer::Binary m_hash;
   Buffer::Binary m_raw;
   std::vector<std::string> m_rawTxs = {};
   std::vector<std::string> m_rawTxReceipts = {};
   void decodeBlockV1(Buffer::Binary read, int heightContext);
   int pos;

   public:
   const Buffer::Binary& getMHash() const;
   const Buffer::Binary& getMRaw() const;
   const std::vector<std::string>& getMRawTxs() const;
   const std::vector<std::string>& getMRawTxReceipts() const;
   static Block Decode(AbstractProtocolChain* p, Buffer::Binary encoded, int heightContext);
};
}    // namespace Protocol


#endif    // BLOCKCHAIN_BLOCK_H
