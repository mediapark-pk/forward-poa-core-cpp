#ifndef BLOCKCHAIN_PUBLICKEY_H
#define BLOCKCHAIN_PUBLICKEY_H

#include "../3rdparty/secp256k1-cxx/secp256k1-cxx.hpp"
#include "../abstractprotocolchain.h"
#include "algorithm"
#include "../crypto/ripemd160.h"
#include "../crypto/sha256.h"
#include <array>
#include <istream>
#include <vector>
#include "spdlog/spdlog.h"
namespace Protocol
{
class privateKey;
class PublicKey
{
   public:
   PublicKey() = default;
   PublicKey(
      AbstractProtocolChain* Protocol,
      std::vector<uint8_t> pub,
      std::vector<uint8_t> priv = {},
      bool pubKeyArgIsCompressed = false);

   //   PublicKey operator=(PublicKey& obj);
   /**
    * Generates PublicKey from Private Key
    * @param AbstractProtocolChain protocol
    * @param PrivateKey privKey
    * @return PublicKey
    */
   static PublicKey fromPrivateKey(
      Protocol::AbstractProtocolChain* _protocol, const std::vector<uint8_t>& privKey);

   /**
    * Generates PublicKey from publicKey Key
    * @param AbstractProtocolChain protocol
    * @param PrivateKey privKey
    * @return PublicKey
    */
   static PublicKey fromPublicKey(AbstractProtocolChain* p, Buffer::Binary pubKey);

   /**
    * Generates AccountID
    *@return string
    */
   const std::string getHash160();

   /** Generates uncompressed public key from compressed pubkey*/
   std::vector<uint8_t> uncompressed() const;

   /** Generates fingerprint against pubkey*/
   uint32_t fingerPrint();

   /** Generates mainAddr*/
   std::string getAddress();

   /** Generates mainAddress to send and receive money
    * @return base58 encoded string
    * */
   std::string mainAddr();

   const std::string& getHash160() const;
   const std::vector<uint8_t>& getPrivKey() const;
   const std::vector<uint8_t>& getPubKey() const;


   protected:
   Protocol::AbstractProtocolChain* m_protocol;
   std::vector<uint8_t> m_privKey;
   std::vector<uint8_t> m_pubKey;

   private:
   std::vector<uint8_t> m_uncompPubKey;
   uint32_t m_fingerprint;
   std::string m_address;
   std::string m_hash160;
};
}    // namespace Protocol

#endif    // BLOCKCHAIN_PUBLICKEY_H
