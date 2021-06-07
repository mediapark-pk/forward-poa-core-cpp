#ifndef EXTENDEDKEY_H
#define EXTENDEDKEY_H

#include "../../3rdparty/secp256k1-cxx/cryptosecp/sha2.hpp"
#include "../../3rdparty/secp256k1-cxx/secp256k1-cxx.hpp"
#include "../../crypto/base58.h"
#include "../../utils/strencodings.h"
#include "../hmac.h"
#include "../ripemd160.h"
#include <array>
#include <assert.h>
#include <iostream>
#include <stdexcept>
#include <stdint.h>
#include <string>
#include <vector>


#define XPRV 0x0488ade4;
#define XPUB 0x0488b21e;
#include <regex>

typedef std::vector<uint8_t> bytes_t;

class ExtendedKey
{
   public:
   ExtendedKey() = default;

   ExtendedKey(
      const bytes_t& key,
      bytes_t chainCode,
      uint32_t childNum = 0,
      uint32_t parentFP = 0,
      unsigned char depth = 0);

   static constexpr int HARDENED_INDEX_BEGIN = 0x80000000;
   static constexpr int BITWISE_SEED_LENGTH = 512;

   unsigned char depth() const;

   bytes_t getPrivateKey() const;

   bytes_t chainCode() const;

   bytes_t getPublicKey() const;

   /** Generates fingerprint */
   uint32_t fp() const;

   /** Serialization of key
    * @param version version for extended key
    */
   bytes_t serializedKey(uint32_t version) const;

   /** Derive is used to generate public keys from private keys,
    * derives XPrv & XPub keys
    */
   ExtendedKey derive(uint32_t i);

   /** Derives the path of the key*/
   ExtendedKey derivePath(const std::string& path);

   /**Generates wallet Import Format from private key
    * @param extkey privatekey of typ bytes_t
    * @return base58 encoded value as string
    * */
   std::string wif(bytes_t extkey);

   /** Generates private Key from Wallet import format
    * @param wif base58 encoded string
    * @return base16encoded private key as string
    * */
   std::string wifTokey(std::string wif);

   /** Generates mainAddress to send and receive money
    * @return base58 encoded string
    * */
   std::string mainAddr();

   /** Generates account ID
    * @return string
    * */
   std::string accountID(bytes_t pubkey = {});


   private:
   bytes_t m_key;          // 33 bytes, first is 0x00
   bytes_t m_chainCode;    // 32 bytes
   bytes_t m_publicKey;
   uint32_t m_parentFingerprint;
   uint32_t m_childNum;
   bool m_validateChildKeyCurveN;
   unsigned char m_depth;
   bool m_valid;
   std::string account_ID;
   inline bool isPrivate() const
   {
      return (m_key.size() == 33 && m_key[0] == 0x00);
   }
};
#endif    // EXTENDEDKEY_H
