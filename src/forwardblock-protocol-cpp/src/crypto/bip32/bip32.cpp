//
// Created by pc on 1/4/21.
//

#include "bip32.h"

/**checks if there is any space */
constexpr inline bool IsSpace(char c) noexcept
{
   return c == ' ' || c == '\f' || c == '\n' || c == '\r' || c == '\t' || c == '\v';
}


const signed char p_util_hexdigit[256] = {
   -1,  -1,  -1,  -1, -1, -1,  -1,  -1,  -1,  -1,  -1,  -1, -1, -1, -1, -1, -1, -1,  -1,  -1,
   -1,  -1,  -1,  -1, -1, -1,  -1,  -1,  -1,  -1,  -1,  -1, -1, -1, -1, -1, -1, -1,  -1,  -1,
   -1,  -1,  -1,  -1, -1, -1,  -1,  -1,  0,   1,   2,   3,  4,  5,  6,  7,  8,  9,   -1,  -1,
   -1,  -1,  -1,  -1, -1, 0xa, 0xb, 0xc, 0xd, 0xe, 0xf, -1, -1, -1, -1, -1, -1, -1,  -1,  -1,
   -1,  -1,  -1,  -1, -1, -1,  -1,  -1,  -1,  -1,  -1,  -1, -1, -1, -1, -1, -1, 0xa, 0xb, 0xc,
   0xd, 0xe, 0xf, -1, -1, -1,  -1,  -1,  -1,  -1,  -1,  -1, -1, -1, -1, -1, -1, -1,  -1,  -1,
   -1,  -1,  -1,  -1, -1, -1,  -1,  -1,  -1,  -1,  -1,  -1, -1, -1, -1, -1, -1, -1,  -1,  -1,
   -1,  -1,  -1,  -1, -1, -1,  -1,  -1,  -1,  -1,  -1,  -1, -1, -1, -1, -1, -1, -1,  -1,  -1,
   -1,  -1,  -1,  -1, -1, -1,  -1,  -1,  -1,  -1,  -1,  -1, -1, -1, -1, -1, -1, -1,  -1,  -1,
   -1,  -1,  -1,  -1, -1, -1,  -1,  -1,  -1,  -1,  -1,  -1, -1, -1, -1, -1, -1, -1,  -1,  -1,
   -1,  -1,  -1,  -1, -1, -1,  -1,  -1,  -1,  -1,  -1,  -1, -1, -1, -1, -1, -1, -1,  -1,  -1,
   -1,  -1,  -1,  -1, -1, -1,  -1,  -1,  -1,  -1,  -1,  -1, -1, -1, -1, -1, -1, -1,  -1,  -1,
   -1,  -1,  -1,  -1, -1, -1,  -1,  -1,  -1,  -1,  -1,  -1, -1, -1, -1, -1,
};

/*
template <typename T> std::string BIP32::HexStr(const T itbegin, const T itend)
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
*/

signed char BIP32::HexDigit(char c)
{
   return p_util_hexdigit[(unsigned char)c];
}


std::vector<unsigned char> BIP32::ParseHex(const char* psz)
{
   // convert hex dump to vector
   std::vector<unsigned char> vch;
   while (true) {
      while (IsSpace(*psz))
         psz++;
      signed char c = HexDigit(*psz++);
      if (c == (signed char)-1)
         break;
      unsigned char n = (c << 4);
      c = HexDigit(*psz++);
      if (c == (signed char)-1)
         break;
      n |= c;
      vch.push_back(n);
   }
   return vch;
}

BIP32::BIP32()
{
   //   m_entropy="063679ca1b28b5cfda9c186b367e271e";
   m_wordlist = "english";
   //   obj.Generate(words);
}

BIP32::BIP32(std::string mEntropy, std::string mWordlist, int mWords)
   : m_wordlist(mWordlist), words(mWords)
{
   obj = words;
}

std::string BIP32::generateSeed(std::string& entropy)
{
   m_seed = obj.getSeed(entropy, m_wordlist);
   return m_seed;
}

void BIP32::generatekeys()
{
   std::string seed =
      "fffcf9f6f3f0edeae7e4e1dedbd8d5d2cfccc9c6c3c0bdbab7b4b1aeaba8a5a29f9c999693908d8a8784817e7b78"
      "75726f6c696663605d5a5754514e4b484542";
   static const std::vector<unsigned char> hashkey = {
      'B', 'i', 't', 'c', 'o', 'i', 'n', ' ', 's', 'e', 'e', 'd'};
   auto vseed = ParseHex(seed.c_str());
   // Pass the out seed to hmac512 to master key
   std::vector<uint8_t> outseed(64);
   hmac_sha512(hashkey.data(), hashkey.size(), vseed.data(), vseed.size(), &outseed[0]);
   MasterKey k(outseed);
   auto pk = k.getMaster();
   m_privatekey = pk->getPrivateKey();
   m_publicKey = pk->getPublicKey();
   m_master = *pk;
}
bytes_t BIP32::getPublicKey()
{
   return m_publicKey;
}
bytes_t BIP32::getPrivateKey()
{
   return m_privatekey;
}
ExtendedKey BIP32::getMaster()
{
   return m_master;
}
std::string BIP32::signatureBIP32(const uint8_t* txHash)
{
   m_privatekey = Secp256K1::getInstance()->privateKey();
   std::vector<uint8_t> sign = Secp256K1::getInstance()->Sign(m_privatekey, txHash);
   std::string signature{sign.begin(), sign.end()};
   std::vector<uint8_t> T2 = Secp256K1::getInstance()->recoverableSignature(
      Secp256K1::getInstance()->privateKey(), txHash);
   T2 = Secp256K1::getInstance()->recoverPublicKeyFromSignature(sign, txHash);
   if (Secp256K1::getInstance()->Verify(txHash, sign, Secp256K1::getInstance()->unCompPublicKey()))
      std::cout << "\nsignature verified\n";
   else
      std::cout << "\nsignature not verified\n";
   return signature;
}

void BIP32::uncompressedPublickey()
{
   m_uncompressedPubkey = Secp256K1::getInstance()->uncompress(m_publicKey);
}
bytes_t BIP32::getuncompressedPubkey()
{
   uncompressedPublickey();
   return m_uncompressedPubkey;
}
