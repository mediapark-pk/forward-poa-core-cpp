//
// Created by umar on 20/5/21.
//

#ifndef FORWARDBLOCK_ABSTRACTBLOCK_H
#define FORWARDBLOCK_ABSTRACTBLOCK_H

#include "../abstractprotocolchain.h"
#include "../accounts/accountproto.h"
#include "blocktxreceipts.h"
#include "blocktxs.h"
#include "../exceptions/blockencodeexception.h"
#include "../exceptions/txencodeexception.h"
#include "../keypair/privatekey/signature.h"
#include "validator.h"
#include "../utils/mathencodings.h"
#include "spdlog/spdlog.h"

namespace Protocol
{


class AbstractBlock
{
   protected:
   AbstractProtocolChain* m_p;
   int m_version;
   int m_timeStamp;
   std::string m_prevBlockId;
   int m_txCount = 0;
   int m_totalIn = 0;
   int m_totalOut = 0;
   int m_totalFee = 0;
   std::string m_forger;
   std::vector<Signature> m_signs;
   int m_reward = 0;
   std::string m_merkleTx = "";
   std::string m_merkleTxReceipts = "";
   int m_bodySize;

   BlockTxs m_txs;
   BlockTxReceipts m_txsReceipts;

   public:
   AbstractBlock() = default;
   AbstractBlock(AbstractProtocolChain* m_p);
   Buffer::Binary hashPreImage(std::string chainId, int forkId);
   Buffer::Binary ser(bool includeSignatures);
};
}    // namespace Protocol


#endif    // FORWARDBLOCK_ABSTRACTBLOCK_H
