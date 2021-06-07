#ifndef BIP32_CXX_H
#define BIP32_CXX_H


#include "../../3rdparty/secp256k1-cxx/secp256k1-cxx.hpp"
#include "../bip39/bip39.h"
#include "iostream"
#include "masterkey.h"

typedef std::vector<uint8_t> bytes_t;

class BIP32
{
   public:
   BIP32();
   BIP32(std::string mEntropy, std::string mWordlist = "English", int mWords = 12);    // parameterized
                                                                                       // constructor
   int words = 12;
   BIP39 obj;

   /**generate seed from given entropy
    * @param entropy as string
    * @return seed as hexadecimal string
    */
   std::string generateSeed(std::string& entropy);

   /** Generates Keys against the Master key obtained after hmac_sha512 processing on seed and
    * hashkey
    */
   void generatekeys();

   /** Hex parsing */
   std::vector<unsigned char> ParseHex(const char* psz);

   /** Generates signature against Txhash
    * @param txHash as string
    * @return signature as hexadecimal string*/
   std::string signatureBIP32(const uint8_t* txHash);

   /** Generates uncompressed public key from compressed pubkey*/
   void uncompressedPublickey();

   bytes_t getPublicKey();
   bytes_t getuncompressedPubkey();
   bytes_t getPrivateKey();
   ExtendedKey getMaster();
   signed char HexDigit(char c);
   template <typename T> std::string HexStr(const T itbegin, const T itend)
   {
      std::string rv;
      static const char hexmap[16] = {
         '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
      rv.reserve(std::distance(itbegin, itend) * 2);
      for (T it = itbegin; it < itend; ++it) {
         unsigned char val = (unsigned char)(*it);
         rv.push_back(hexmap[val >> 4]);
         rv.push_back(hexmap[val & 15]);
      }
      return rv;
   }

   private:
   std::string m_entropy;
   std::string m_wordlist;    // mnemonics wordlist
   std::string m_seed;        // seed generated from BIP39
   bytes_t m_privatekey;      // 33 bytes, first is 0x00
   bytes_t m_chainCode;       // 32 bytes
   bytes_t m_publicKey;
   bytes_t m_uncompressedPubkey;
   ExtendedKey m_master;
};

#endif
