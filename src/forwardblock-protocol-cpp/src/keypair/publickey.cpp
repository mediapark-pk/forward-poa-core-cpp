#include "publickey.h"
#include "privatekey.h"

Protocol::PublicKey::PublicKey(AbstractProtocolChain* _protocol,
   std::vector<uint8_t> pub, std::vector<uint8_t> priv, bool pubKeyArgIsCompressed)
{
   m_protocol=_protocol;
       if (m_pubKey.empty() && m_privKey.empty()) {
          spdlog::error("PublicKey::{} public and private key empty",  __FUNCTION__);
          throw std::runtime_error("public and private key empty");
       }
   if (!priv.empty()) {
      try {
         // Derive public key from private key
         Secp256K1::getInstance()->createPublicKeyFromPriv(priv);
         m_privKey = std::move(priv);
         // this is compressed pub key
         m_pubKey = Secp256K1::getInstance()->publicKey();
      } catch (...) {
         throw;
      }
   } else if (!pub.empty()) {
      if (pubKeyArgIsCompressed == false) {
         // Argument is a full (uncompressed) public key
         Secp256K1::getInstance()->createPublicKeyFromPriv(priv, true);
         m_pubKey = Secp256K1::getInstance()->publicKey();
      } else if (pubKeyArgIsCompressed == true) {
         // Argument is a compressed public key
         Secp256K1::getInstance()->uncompress(pub);
         m_uncompPubKey = Secp256K1::getInstance()->unCompPublicKey();
      }
   }
}

/*
Protocol::PublicKey Protocol::PublicKey::operator=(Protocol::PublicKey& obj)
{
   m_protocol=obj.m_protocol;
   m_privKey=obj.m_privKey;
   m_pubKey=obj.m_pubKey;
   m_uncompPubKey=obj.m_uncompPubKey;
   return *this;
}
*/


Protocol::PublicKey Protocol::PublicKey::fromPrivateKey(Protocol::AbstractProtocolChain* _protocol, const std::vector<uint8_t>& privKey)
{
    return PublicKey(_protocol, {}, privKey);
}

Protocol::PublicKey Protocol::PublicKey::fromPublicKey(AbstractProtocolChain* p, Buffer::Binary pubKey)
{
   if (pubKey.getBufferData().empty()) {
      spdlog::error("PublicKey::{} publicKey is empty",  __FUNCTION__);
      throw std::runtime_error("pubkey is empty");
   }
   std::string prefix = pubKey.value(0, 1);
   std::string arr[3]={"\x02", "\x03", "\x04"};
   if (std::find(std::begin(arr), std::end(arr), prefix)) {
      spdlog::error("PublicKey::{} Invalid public key prefix",  __FUNCTION__);
      throw std::runtime_error("Invalid public key prefix");
   }
   bool isCompressed = prefix=="\x04" ? false:true;
   std::vector<uint8_t> pub{pubKey.getBufferData().begin(), pubKey.getBufferData().end()};
   return PublicKey(p,  pub, {}, isCompressed);
}

const std::string Protocol::PublicKey::getHash160()
{
   if (m_hash160.empty()) {
      std::vector<uint8_t> ripmd(20);
      ripmd = ripemd160(sha256(m_pubKey));
      std::string accountID{ripmd.begin(), ripmd.end()};
      m_hash160 = accountID;
   }
   return m_hash160;
}
std::vector<uint8_t> Protocol::PublicKey::uncompressed() const
{
   return Secp256K1::uncompress(m_pubKey);
}
std::string Protocol::PublicKey::getAddress()
{
   if (!m_address.empty())
      return m_address;
   m_address = mainAddr();
   return m_address;
}

std::string Protocol::PublicKey::mainAddr()
{
   std::string ripmdFromPubkey = getHash160();
   if (!std::regex_match(ripmdFromPubkey, std::regex("/^[a-f0-9]{40}$/i"))) {
      std::runtime_error("Invalid hash160 value");
   }
   // Add version byte in front of RIPEMD-160 hash (0x00 for Main Network)
   bytes_t ripemd;
   ripemd.insert(ripemd.end(), 0x00);

   ripemd.insert(ripemd.end(), ripmdFromPubkey.begin(), ripmdFromPubkey.end());

   // base58 encoding
   std::vector<char> key(64);
   base58_encode_check(ripemd.data(), ripemd.size(), &key[0], key.size());
   std::cout << "\npublic Key to main address: ";
   std::string mainaddr{key.begin(), key.end()};
   std::cout << mainaddr << "\n";
   return mainaddr;
}


uint32_t Protocol::PublicKey::fingerPrint()
{
   if (m_fingerprint)
      return m_fingerprint;
   std::vector<uint8_t> final(32);
   sha256_Raw(m_pubKey.data(), m_pubKey.size(), &final[0]);
   ripemd160(final.data(), final.size(), &final[0]);
   m_fingerprint = ((uint32_t) final[0] << 24) | ((uint32_t) final[1] << 16) |
      ((uint32_t) final[2] << 8) | ((uint32_t) final[3]);
   return m_fingerprint;
}
const std::vector<uint8_t>& Protocol::PublicKey::getPubKey() const
{
   return m_pubKey;
}
const std::string& Protocol::PublicKey::getHash160() const
{
   return m_hash160;
}
const std::vector<uint8_t>& Protocol::PublicKey::getPrivKey() const
{
   return m_privKey;
}
