#ifndef ABSTRACT_PROTOCOL_CHAIN_H
#define ABSTRACT_PROTOCOL_CHAIN_H

#include "config.h"
// TODO::
#include "data-types-v0.0.0/src/buffer/binary.h"
#include "keypair/privatekey/signature.h"
#include "protocolconstants.h"
#include <memory>
#include <string>
#include <vector>
#include "3rdparty/secp256k1-cxx/secp256k1-cxx.hpp"

namespace Protocol
{
class AbstractTx;
class TxFlags;
class AccountsProto;
class KeyPairFactory;

class AbstractProtocolChain : public ProtocolConstants
{
   int m_versionId;

   public:
   AbstractProtocolChain() = default;
   explicit AbstractProtocolChain(const std::string& configPath);
   /** TODO **/
   std::string chainName();
   std::string version();
   int versionId();
   std::shared_ptr<Protocol::TxFlags> txFlags();
   Secp256K1* secp256K1();
   std::shared_ptr<Protocol::Config> config();
   std::shared_ptr<Protocol::KeyPairFactory> keyPair();
   std::shared_ptr<AccountsProto> accounts();
   Buffer::Binary hash256(Buffer::Binary& bin);
   Buffer::Binary serializeSignatures(std::vector<Signature> signature);


   virtual bool isEnabledTxFlag(const AbstractTx& f, int blockHeightContext) = 0;
   virtual int getForkId(int blockHeightContext) = 0;

   virtual ~AbstractProtocolChain();


   protected:
   std::shared_ptr<Config> m_config;
   std::shared_ptr<KeyPairFactory> m_kpF;
   std::shared_ptr<TxFlags> m_txFlags;
   std::shared_ptr<AccountsProto> m_ap;

   virtual void registerTxFlags(std::shared_ptr<Protocol::TxFlags> flags) = 0;
};
}    // namespace Protocol
#endif