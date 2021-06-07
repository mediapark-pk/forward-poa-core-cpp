#include "extendedkey.h"
#include "iostream"

ExtendedKey::ExtendedKey(
   const bytes_t& key, bytes_t chainCode, uint32_t childNum, uint32_t parentFP, unsigned char depth)
   : m_key{key}
   , m_chainCode{std::move(chainCode)}
   , m_parentFingerprint{parentFP}
   , m_childNum{childNum}
   , m_depth{depth}
{
   if (m_chainCode.size() != 32) {
      throw std::runtime_error("Invalid chain code, len: " + std::to_string(m_chainCode.size()));
   }

   std::cout << "private Key : ";
   for (const auto& itr : base16Encode({key.begin(), key.end()})) {
      std::cout << itr;
   }
   std::cout << "\n";

   std::cout << "chaincode : ";
   for (const auto& itr : base16Encode({m_chainCode.begin(), m_chainCode.end()})) {
      std::cout << itr;
   }
   std::cout << "\n";


   // key is private -- 32
   if (key.size() == 32) {
      // TODO: check for size AND zeroness here
      m_key.insert(m_key.begin(), 0x00);
   }

   // key is public -- 33
   if (key.size() == 33) {
      try {
      } catch (const std::runtime_error& e) {
         throw e;
      }
   }
   if (isPrivate()) {
      Secp256K1::getInstance()->createPublicKeyFromPriv(key);
      // compressed form
      m_publicKey = Secp256K1::getInstance()->publicKey();
   } else {
      m_publicKey = m_key;
   }
   std::cout << "public key : ";
   for (const auto& itr : base16Encode({m_publicKey.begin(), m_publicKey.end()})) {
      std::cout << itr;
   }
   std::cout << "\n";

   m_valid = true;
}

unsigned char ExtendedKey::depth() const
{
   return m_depth;
}

bytes_t ExtendedKey::getPrivateKey() const
{
   return m_key;
}

bytes_t ExtendedKey::chainCode() const
{
   return m_chainCode;
}

bytes_t ExtendedKey::getPublicKey() const
{
   return m_publicKey;
}

uint32_t ExtendedKey::fp() const
{
   std::vector<uint8_t> final(32);
   sha256_Raw(m_publicKey.data(), m_publicKey.size(), &final[0]);
   ripemd160(final.data(), final.size(), &final[0]);
   return ((uint32_t) final[0] << 24) | ((uint32_t) final[1] << 16) | ((uint32_t) final[2] << 8) |
      ((uint32_t) final[3]);
}

bytes_t ExtendedKey::serializedKey(uint32_t version) const
{
   bytes_t extkey;
   extkey.reserve(78);

   // version
   extkey.push_back((uint32_t)version >> 24);
   extkey.push_back(((uint32_t)version >> 16) & 0xff);
   extkey.push_back(((uint32_t)version >> 8) & 0xff);
   extkey.push_back((uint32_t)version & 0xff);

   // depth
   extkey.push_back(m_depth);

   // fingerprint
   auto fp = m_depth == 0 ? 0 : m_parentFingerprint;
   extkey.push_back((uint32_t)fp >> 24);
   extkey.push_back(((uint32_t)fp >> 16) & 0xff);
   extkey.push_back(((uint32_t)fp >> 8) & 0xff);
   extkey.push_back((uint32_t)fp & 0xff);

   // childnum
   extkey.push_back((uint32_t)m_childNum >> 24);
   extkey.push_back(((uint32_t)m_childNum >> 16) & 0xff);
   extkey.push_back(((uint32_t)m_childNum >> 8) & 0xff);
   extkey.push_back((uint32_t)m_childNum & 0xff);

   // chaincode
   extkey.insert(extkey.end(), m_chainCode.begin(), m_chainCode.end());

   // key
   uint32_t val = XPRV;
   if (version == val) {
      if (m_key.size() == 32)
         extkey.emplace_back(0x00);

      extkey.insert(extkey.end(), m_key.begin(), m_key.end());
   }

   val = XPUB;

   if (version == val) {
      extkey.insert(extkey.end(), m_publicKey.begin(), m_publicKey.end());
   }

   assert(extkey.size() == 78);

   return extkey;
}

ExtendedKey ExtendedKey::derive(uint32_t i)
{
   if (!m_valid) {
      throw std::runtime_error("Invalid extended key, cannot derive");
   }
   bool privDerivation = 0x80000000 & i;
   if (!isPrivate() && privDerivation) {
      throw std::runtime_error("Cannot do private key derivation on public key.");
   }

   ExtendedKey child;
   child.m_valid = false;

   bytes_t data;
   data = privDerivation ? m_key : m_publicKey;

   data.push_back(i >> 24);
   data.push_back((i >> 16) & 0xff);
   data.push_back((i >> 8) & 0xff);
   data.push_back(i & 0xff);

   bytes_t hash;
   hash.resize(64);
   hmac_sha512(m_chainCode.data(), m_chainCode.size(), data.data(), data.size(), &hash[0]);
   assert(hash.size() == 64);

   bytes_t childKey(m_key.begin() + 1, m_key.end());
   bytes_t childChainCode(hash.begin() + 32, hash.end());
   bytes_t lL(hash.begin(), hash.begin() + 32);

   assert(childKey.size() == 32);
   assert(childChainCode.size() == 32);

   bool ret = Secp256K1::getInstance()->privKeyTweakAdd(childKey, lL);
   if (ret) {
      m_parentFingerprint = Secp256K1::getInstance()->fingerprint();
      //        std::cout<<"\nParent : "<<m_parentFingerprint<<"\n";
      child = ExtendedKey(childKey, childChainCode, i, m_parentFingerprint, m_depth + 1);
      //        std::cout<<"\nDepth level : "<<std::to_string(m_depth)<<"\n";
   }

   return child;
}

ExtendedKey ExtendedKey::derivePath(const std::string& path)
{
   if (path.empty())
      throw std::runtime_error("Invalid derive path: empty");

   std::vector<uint32_t> path_vector;

   size_t i = 0;
   uint64_t n = 0;
   while (i < path.size()) {
      char c = path[i];
      if (c == 'm') {
         i++;
         continue;
      }
      if (c >= '0' && c <= '9') {
         n *= 10;
         n += (uint32_t)(c - '0');
         if (n >= 0x80000000)
            throw std::runtime_error("Invalid size in derive path");
         i++;
         if (i >= path.size()) {
            path_vector.push_back((uint32_t)n);
         }
      } else if (c == '\'') {
         if (i + 1 < path.size()) {
            if ((i + 2 >= path.size()) || (path[i + 1] != '/') || (path[i + 2] < '0') || (path[i + 2] > '9'))
               throw std::runtime_error("Invalid size in derive path");
         }
         n |= 0x80000000;
         path_vector.push_back((uint32_t)n);
         n = 0;
         i += 2;
      } else if (c == '/') {
         if (i + 1 >= path.size() || path[i + 1] < '0' || path[i + 1] > '9')
            throw std::runtime_error("invalid derive path");
         path_vector.push_back((uint32_t)n);
         n = 0;
         i++;
      } else {
         throw std::runtime_error("invalid derive path");
      }
   }

   ExtendedKey child(*this);
   for (auto n : path_vector) {
      child = child.derive(n);
   }
   return child;
}

std::string ExtendedKey::wif(bytes_t extkey)
{
   bytes_t wifkey;
   /*  Add a 0x80 byte in front of it for mainnet addresses*/
   wifkey.insert(wifkey.end(), 0x80);
   wifkey.insert(wifkey.end(), extkey.begin() + 1, extkey.end());

   /*  Add a 0x01 byte at the end if the private key will correspond to a compressed public key */
   if (m_publicKey.size() == 33) {
      wifkey.insert(wifkey.end(), 0x01);
   }

   std::vector<char> key(64);
   base58_encode_check(wifkey.data(), wifkey.size(), &key[0], key.size());

   std::cout << "Private Key to WIF: ";
   std::string pubb58{key.begin(), key.end()};
   std::cout << pubb58 << "\n";
   return pubb58;
}

std::string ExtendedKey::wifTokey(std::string wif)
{
   unsigned char key[34];
   uint8_t* ptr = reinterpret_cast<uint8_t*>(key);
   base58_decode_check(wif.c_str(), ptr, wif.length());
   std::string decodedkey;

   for (const auto& itr : base16Encode({std::begin(key), std::end(key)})) {
      decodedkey += itr;
   }
   if (m_publicKey.size() == 33) {
      decodedkey = decodedkey.substr(2, decodedkey.length() - 4);
   } else {
      decodedkey = decodedkey.substr(2, decodedkey.length() - 2);
   }
   std::cout << "\nWIF to Private key : " << decodedkey << "\n";
   return decodedkey;
}

std::string ExtendedKey::mainAddr()
{
   std::string ripmdFromPubkey = accountID();
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
   std::string mainaddr1{key.begin(), key.end()};
   std::cout << mainaddr1 << "\n";
   return mainaddr1;
}

std::string ExtendedKey::accountID(bytes_t pubkey)
{
   if (pubkey.empty()) {
      pubkey = m_publicKey;
   }
   // Sha256 applied on publickey
   std::vector<uint8_t> final(32);
   sha256_Raw(pubkey.data(), pubkey.size(), &final[0]);

   // ripemd applied on resulting value of sha256
   std::vector<uint8_t> ripmd(20);
   ripemd160(final.data(), final.size(), &ripmd[0]);
   std::string accountID{ripmd.begin(), ripmd.end()};
   account_ID = accountID;
   //   std::cout<<"\nAccount ID : "<<BIP39_Utils::base16Encode(accountID);
   //   std::cout<<"\nAccount ID : "<<(accountID);

   return accountID;
}
