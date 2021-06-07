#include "keypairfactory.h"
#include <stdexcept>

Protocol::KeyPairFactory::KeyPairFactory(AbstractProtocolChain* Protocol) : m_protocol(Protocol) {}

Protocol::PrivateKey Protocol::KeyPairFactory::privateKeyFromEntropy(Buffer::Base16 priv)
{
   if (priv.len() != 32) {
      spdlog::error(
         "KeyPairFactory::{} Private key must be 32 bytes in length",  __FUNCTION__);
      throw std::runtime_error("Private key must be 32 bytes in length");
   }
   // Todo change according to constrructor
   //    return PrivateKey(std::move(priv));
}
Protocol::PrivateKey Protocol::KeyPairFactory::privateKeyFromMnemonic(
   const Mnemonic& m, const std::string seedPassphrase)
{
   Buffer::Base16 b16Entropy(m.entropy);
   if (seedPassphrase.empty()) {
      Buffer::Binary entropy(
         std::string(m.generateSeed(seedPassphrase).begin(), m.generateSeed(seedPassphrase).end()));
//      b16Entropy = entropy.base16();
   }
   return privateKeyFromEntropy(b16Entropy);
}

Protocol::PublicKey Protocol::KeyPairFactory::publicKeyfromEntropy(Buffer::Base16 pub)
{
   try{
      Protocol::Validator::checkPublicKey(const_cast<std::string&>(pub.getBufferData()));
      std::vector<uint8_t> pubkey{pub.getBufferData().begin(), pub.getBufferData().end()};
      return Protocol::PublicKey(m_protocol, pubkey, {}, false);
   }
   catch(Exception e){
//      Todo Error type
      throw std::runtime_error("");
   }
}
Protocol::PrivateKey Protocol::KeyPairFactory::privateKeyFromWIF(const std::string wif, bool isCompressed)
{
   Buffer::Base16 entropy(wif);
   try{
      //Todo ask umar about WIFPrefix
      Buffer::Base16 entropy1 = WIF::Decode(
         m_protocol->config()->getMWifPrvPrefix(), wif, isCompressed);
      entropy=entropy1;
   }
   catch(Exception e){
      throw std::runtime_error("Failed to Decode WIF");
   }
   return privateKeyFromEntropy(entropy);
}
