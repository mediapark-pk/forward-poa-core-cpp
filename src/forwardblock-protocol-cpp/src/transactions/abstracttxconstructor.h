//
// Created by umar on 5/20/21.
//

#ifndef FORWARDBLOCK_ABSTRACTTXCONSTRUCTOR_H
#define FORWARDBLOCK_ABSTRACTTXCONSTRUCTOR_H

#include "abstracttx.h"
#include "../keypair/privatekey/signature.h"
#include "../keypair/publickey.h"
#include <spdlog/spdlog.h>
#include "../exceptions/txconstructexception.h"
#include "../keypair/privatekey.h"
#include <vector>

namespace Protocol
{
class AbstractProtocolChain;
class AbstractTxFlag;
class AbstractTxConstructor : public AbstractTx
{
   protected:
   PublicKey m_senderPubKey;
   PublicKey m_recipientPubKey;
   Protocol::AbstractTxFlag* m_txFlag;
   AbstractTxConstructor(AbstractProtocolChain* p, int ver, AbstractTxFlag* flag, int epoch);
   virtual void beforeSerialize() = 0;

   public:
   void sender(PublicKey sender, int nonce);
   void memo(std::string memo);
   void timeStamp(int ts);
   void addSignature(std::vector<Signature> sign);
   void signTransaction(PrivateKey pK, std::string chainId = nullptr, int forkId = 0);
   Buffer::Binary serialize(bool includeSignatures);
};
}    // namespace Protocol


#endif    // FORWARDBLOCK_ABSTRACTTXCONSTRUCTOR_H
