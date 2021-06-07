#ifndef SECP256K1_CPP_H
#define SECP256K1_CPP_H

#include "../../crypto/ripemd160.h"
#include "../../utils/strencodings.h"
#include "cryptosecp/sha2.hpp"
#include "libsecp256k1/include/secp256k1.h"
#include "libsecp256k1/include/secp256k1_recovery.h"
#include <chrono>
#include <stdexcept>
#include <stdint.h>
#include <vector>
class Secp256K1Exception : public std::runtime_error
{
   public:
   Secp256K1Exception(const char* error) noexcept : std::runtime_error(error) {}

   const char* what() const noexcept
   {
      return std::runtime_error::what();
   }
};

class Secp256K1
{
   public:
   ~Secp256K1();
   bool createPrivateKey();
   void setPrivKey(std::vector<uint8_t> priv);
   void setPubkey(std::vector<uint8_t> pub);
   /**
    * @brief verifies private key and generates corresponding public key
    * @param privateKey - in hexadecimal
    */
   bool createPublicKeyFromPriv(const std::vector<uint8_t>& privateKey, bool compressed = false);

   /**
    * @brief add tweak and module curve order (key + tweak) mod n
    * @param key
    * @param tweak
    * @return true | false
    */
   bool privKeyTweakAdd(std::vector<uint8_t>& key, const std::vector<uint8_t>& tweak);

   /**
    * @brief Recover public key from Signaturte
    * @param sign recoverable signature of 65 bytes
    * @param hash txhash
    * @return PublicKey
    */
   std::vector<uint8_t> recoverPublicKeyFromSignature(std::vector<uint8_t> sign, const uint8_t* hash);


   /**
    * @brief generates uncompressed publicKey from compressed pubkey
    * @param _pubKey compressed pubkey
    * @return uncompressed publicKey
    */
   std::vector<uint8_t> uncompressedPublicKey(std::vector<uint8_t> _pubKey = {});
   std::vector<uint8_t> unCompPublicKey() const;
   std::vector<uint8_t> publicKey() const;
   std::vector<uint8_t> privateKey() const;

   /** Generates fingerprint against pubkey*/
   uint32_t fingerprint() const;
   std::string publicKeyHex() const;
   std::string privateKeyHex() const;
   /**
    * @brief generates signature from prvKey
    * @param privKey
    * @param hash txHash
    * @return signature 64 bytes
    */
   std::vector<uint8_t> Sign(std::vector<uint8_t> privKey, const uint8_t* hash) const;

   /**
    * @brief generates Recoverable signature
    * @param privKey 32 bytes
    * @param hash txHash
    * @return Recoverable signature 65 bytes
    */
   std::vector<uint8_t> recoverableSignature(
      std::vector<uint8_t> privKey, const uint8_t* hash);

   /**
    * @brief Secp256K1::Verify
    * @param msgHash being verified
    * @param sign input recoverable signature (65 bytes)
    * @param pubKey uncompressed pubKey being used to verify the msg (65 bytes)
    * @return true if success
    */
   bool Verify(
      const uint8_t* msgHash, const std::vector<uint8_t>& sig, const std::vector<uint8_t>& pubKey);

   static std::vector<uint8_t> uncompress(const std::vector<uint8_t>& pubkey);
   static Secp256K1* getInstance();
   void setPrivKey(const std::vector<uint8_t>& privKey);
   std::vector<uint8_t> m_pubKey;
   std::vector<uint8_t> m_uncompressedpubKey;

   private:
   secp256k1_context* ctx = NULL;
   std::vector<uint8_t> m_privKey;
   static constexpr size_t PUBLIC_KEY_SIZE = 65;
   static Secp256K1* instance;

   /** PRIVATE METHODS **/
   Secp256K1();
   bool verifyKey();
   bool createPublicKey(std::vector<uint8_t> _key = {}, bool compressed = true);
};

#endif