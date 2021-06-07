//
// Created by umar on 5/20/21.
//

#include "abstracttxconstructor.h"
#include "abstracttx.h"
#include "abstracttxflag.h"


Protocol::AbstractTxConstructor::AbstractTxConstructor(Protocol::AbstractProtocolChain *p, int ver,
                                                       Protocol::AbstractTxFlag *flag, int epoch) :
        AbstractTx(p) {
    m_version = ver;
    m_txFlag = flag;
    m_flag = flag->id();
    timeStamp(epoch);
}


void Protocol::AbstractTxConstructor::sender(Protocol::PublicKey sender, int nonce)
{
   if (m_nonce < 0 || m_nonce > 0xffffffff) {
      spdlog::error("AbstractTxConstructor::{} Sender nonce is out of range",  __FUNCTION__);
      throw TxConstructException("Sender nonce is out of range");
   }
   m_senderPubKey = sender;
   m_nonce = nonce;
}

void Protocol::AbstractTxConstructor::memo(std::string memo)
{
   try {
      memo = Validator::validatedMemo(memo);
   } catch (Exception& e) {
      spdlog::error(
         "AbstractTxConstructor::{} {}",
         __FUNCTION__,
         e.what());
      throw TxConstructException(e.what());
   }
   m_memo = memo;
}

void Protocol::AbstractTxConstructor::timeStamp(int ts)
{
   if (!Validator::isValidEpoch(ts)) {
      spdlog::error(
         "AbstractTxConstructor::{} nvalid timestamp/epoch :: timestamp = {}",
         __FUNCTION__,
         m_timeStamp);
      throw TxConstructException(
         "Invalid timestamp/epoch :: timestamp = " + std::to_string(m_timeStamp));
   }
   m_timeStamp = ts;
}

void Protocol::AbstractTxConstructor::addSignature(std::vector<Signature> sign)
{
   if (sign.size() >= 5) {
      spdlog::error("AbstractTxConstructor::{} Cannot add more than 5 signatures", __FUNCTION__);
      throw TxConstructException("Cannot add more than 5 signatures ");
   }
   m_signs.insert(m_signs.end(), sign.begin(), sign.end());
}

void Protocol::AbstractTxConstructor::signTransaction(
   Protocol::PrivateKey pK, std::string chainId, int forkId)
{
   // TODO:: ONCE SIGNTRANSACTION is implemented in pvtkey
}

Buffer::Binary Protocol::AbstractTxConstructor::serialize(bool includeSignatures) {
    if (!m_senderPubKey.getPubKey().empty()) {
        m_sender = hexToBin({m_senderPubKey.getHash160().begin(),m_senderPubKey.getHash160().end()});
    }
    if (!m_recipientPubKey.getPubKey().empty()) {
        m_sender = hexToBin({m_recipientPubKey.getHash160().begin(),m_recipientPubKey.getHash160().end()});
    }
    return AbstractTx::serialize(includeSignatures);
}
