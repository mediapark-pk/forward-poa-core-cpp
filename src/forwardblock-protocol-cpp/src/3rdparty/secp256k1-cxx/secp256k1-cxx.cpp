#include "secp256k1-cxx.hpp"

#include <cassert>
#include <cstring>
#include <iostream>
#include <random>
#include <tuple>
#include <vector>

/**
 * @brief Secp256K1::Secp256K1
 * creates pub/priv key pair
 */
Secp256K1::Secp256K1()
   : ctx(secp256k1_context_create(SECP256K1_CONTEXT_SIGN | SECP256K1_CONTEXT_VERIFY))
{
}

Secp256K1::~Secp256K1()
{
   secp256k1_context_destroy(ctx);
}

bool Secp256K1::createPrivateKey()
{
   // get epoch time
   unsigned seed1 = std::chrono::system_clock::now().time_since_epoch().count();

   // generate random number for priv key
   std::seed_seq seed{seed1};
   std::mt19937_64 eng(seed);
   std::string randString;
   for (int i = 0; i < 10; ++i) {
      randString += eng();
   }

   // generate SHA-256 (our priv key)
   std::vector<uint8_t> out;
   out.resize(32);
   sha256_Raw(reinterpret_cast<const uint8_t*>(randString.c_str()), randString.length(), &out[0]);

   assert(out.size() == 32);

   m_privKey = std::move(out);
   return verifyKey();
}

void Secp256K1::setPrivKey(std::vector<uint8_t> priv)
{
   m_privKey = std::move(priv);
}

void Secp256K1::setPubkey(std::vector<uint8_t> pub)
{
   m_pubKey = std::move(pub);
}

bool Secp256K1::createPublicKeyFromPriv(const std::vector<uint8_t>& privateKey, bool compressed)
{
   m_privKey = privateKey;
   // verify priv key
   if (!verifyKey()) {
      throw Secp256K1Exception("Unable to create and verify key:  ");
   }

   if (!createPublicKey(m_privKey, compressed)) {
      throw Secp256K1Exception("Unable to create publick key");
   }
   return true;
}

bool Secp256K1::privKeyTweakAdd(std::vector<uint8_t>& key, const std::vector<uint8_t>& tweak)
{
   bool ret = secp256k1_ec_privkey_tweak_add(ctx, &key[0], tweak.data());
   return ret;
}

std::vector<uint8_t> Secp256K1::uncompressedPublicKey(std::vector<uint8_t> _pubKey)
{
   if (_pubKey.empty()) {
      _pubKey = m_pubKey;
   }
   secp256k1_pubkey pubkey;
   assert(ctx && "secp256k1_context_verify must be initialized to use CPubKey.");
   if (!secp256k1_ec_pubkey_parse(ctx, &pubkey, &_pubKey[0], _pubKey.size())) {
      throw Secp256K1Exception("Unable to parse public key.");
   }
   std::vector<uint8_t> pub(65);
   size_t publen = 65;
   secp256k1_ec_pubkey_serialize(ctx, &pub[0], &publen, &pubkey, SECP256K1_EC_UNCOMPRESSED);
   m_uncompressedpubKey = pub;
   return pub;
}

std::vector<uint8_t> Secp256K1::uncompress(const std::vector<uint8_t>& pubkey)
{
   if (pubkey.empty())
      throw Secp256K1Exception("Empty public key in Secp256K1::uncompress");
   Secp256K1 s;
   s.setPubkey(pubkey);
   return s.uncompressedPublicKey(pubkey);
}

std::vector<uint8_t> Secp256K1::publicKey() const
{
   return m_pubKey;
}

std::vector<uint8_t> Secp256K1::unCompPublicKey() const
{
   return m_uncompressedpubKey;
}


std::vector<uint8_t> Secp256K1::privateKey() const
{
   return m_privKey;
}

uint32_t Secp256K1::fingerprint() const
{
   std::vector<uint8_t> final(32);
   sha256_Raw(m_pubKey.data(), m_pubKey.size(), &final[0]);
   ripemd160(final.data(), final.size(), &final[0]);
   return ((uint32_t) final[0] << 24) | ((uint32_t) final[1] << 16) | ((uint32_t) final[2] << 8) |
      ((uint32_t) final[3]);
}

std::string Secp256K1::publicKeyHex() const
{
   return base16Encode(reinterpret_cast<const char*>(m_pubKey.data()));
}

std::string Secp256K1::privateKeyHex() const
{
   return base16Encode(reinterpret_cast<const char*>(m_privKey.data()));
}

bool Secp256K1::verifyKey()
{
   return secp256k1_ec_seckey_verify(ctx, m_privKey.data());
}

bool Secp256K1::createPublicKey(std::vector<uint8_t> _key, bool compressed)
{
   // Calculate public key.
   secp256k1_pubkey pubkey;
   int ret = secp256k1_ec_pubkey_create(ctx, &pubkey, _key.data());
   if (ret != 1) {
      return false;
   }

   // Serialize public key.
   size_t outSize = PUBLIC_KEY_SIZE;
   m_pubKey.resize(outSize);
   secp256k1_ec_pubkey_serialize(
      ctx,
      m_pubKey.data(),
      &outSize,
      &pubkey,
      compressed ? SECP256K1_EC_COMPRESSED : SECP256K1_EC_UNCOMPRESSED);
   if (compressed == true)
      m_pubKey.resize(outSize);
   else if (compressed == false)
      m_uncompressedpubKey = m_pubKey;
   // Succeed.
   return true;
}

std::vector<uint8_t> Secp256K1::Sign(std::vector<uint8_t> privKey, const uint8_t* hash) const
{
   // Make signature.
   secp256k1_ecdsa_signature sig;
   std::vector<unsigned char> temp = privKey;

   int ret = secp256k1_ecdsa_sign(
      ctx, &sig, hash, temp.data(), secp256k1_nonce_function_rfc6979, nullptr);

   if (ret != 1) {
      // Failed to sign.
      return {};
   }
   std::vector<uint8_t> sigOut{sig.data, sig.data + 64};
   // For testing
   std::cout << " Simple Signature " << base16Encode(std::string{sigOut.begin(), sigOut.end()})
             << " size " << sigOut.size() << std::endl;

   return sigOut;
}


std::vector<uint8_t> Secp256K1::recoverableSignature(std::vector<uint8_t> privKey, const uint8_t* hash)
{
   secp256k1_context* ctx = secp256k1_context_create(SECP256K1_CONTEXT_SIGN);
   std::vector<uint8_t> vchSig;
   vchSig.resize(65);
   int rec = -1;
   std::vector<unsigned char> secKey = privKey;
   unsigned char extra_entropy[32] = {0};
   std::vector<unsigned char> test_case;
   secp256k1_ecdsa_recoverable_signature sig;
   int ret = secp256k1_ecdsa_sign_recoverable(
      ctx, &sig, hash, secKey.data(), secp256k1_nonce_function_rfc6979, nullptr);
   std::vector<uint8_t> compactSig{sig.data, sig.data + 65};
   std::cout << "Signature recoverable 65 bytes: "
             << base16Encode(std::string{compactSig.begin(), compactSig.end()}) << " size "
             << compactSig.size() << std::endl;
   assert(ret);
   secp256k1_ecdsa_recoverable_signature_serialize_compact(
      ctx, (unsigned char*)&vchSig[0], &rec, &sig);
   std::cout << "Signature serializae : " << base16Encode(std::string{vchSig.begin(), vchSig.end()})
             << " size " << vchSig.size() << std::endl;
   vchSig[64] = rec;
   assert(ret);
   assert(rec != -1);
   return vchSig;
}

std::vector<uint8_t> Secp256K1::recoverPublicKeyFromSignature(
   std::vector<uint8_t> sign, const uint8_t* hash)
{
   secp256k1_context* ctx = secp256k1_context_create(SECP256K1_CONTEXT_VERIFY);
   std::string str{sign.begin() + 64, sign.end()};
   int v = std::stoi(base16Encode(str));
   if (v > 3)
      return {};

   secp256k1_ecdsa_recoverable_signature rawSig;
   if (!secp256k1_ecdsa_recoverable_signature_parse_compact(ctx, &rawSig, sign.data(), v))
      return {};

   secp256k1_pubkey rawPubkey;
   if (!secp256k1_ecdsa_recover(ctx, &rawPubkey, &rawSig, hash))
      return {};
   std::vector<uint8_t> rawpubkey{rawPubkey.data, rawPubkey.data + 65};
   std::cout << "recoverpublicKey : " << base16Encode(std::string{rawpubkey.begin(), rawpubkey.end()})
             << " size " << rawpubkey.size() << std::endl;
   std::vector<unsigned char> serializedPubkey(65);
   size_t serializedPubkeySize = serializedPubkey.size();
   secp256k1_ec_pubkey_serialize(
      ctx, serializedPubkey.data(), &serializedPubkeySize, &rawPubkey, SECP256K1_EC_UNCOMPRESSED);
   assert(serializedPubkeySize == serializedPubkey.size());

   // Expect single byte header of value 0x04 -- uncompressed public key.
   assert(serializedPubkey[0] == 0x04);

   std::cout << "recoverpublicKey serializae : "
             << base16Encode(std::string{serializedPubkey.begin(), serializedPubkey.end()})
             << " size " << serializedPubkey.size() << std::endl;
   if (Secp256K1::getInstance()->m_uncompressedpubKey == serializedPubkey) {
      std::cout << "\n * * * Succeded * * *\n";
   } else
      std::cout << "\n * * * Try again * * *\n";

   return serializedPubkey;
}

bool Secp256K1::Verify(
   const uint8_t* msgHash, const std::vector<uint8_t>& sign, const std::vector<uint8_t>& pubKey)
{
   if (pubKey.size() != PUBLIC_KEY_SIZE) {
      throw Secp256K1Exception("Invalid public key size");
      return false;
   }
   secp256k1_context* ctx = secp256k1_context_create(SECP256K1_CONTEXT_VERIFY);
   std::string str{sign.begin() + 64, sign.end()};
   int v = std::stoi(base16Encode(str));
   if (v > 3)
      return {};

   secp256k1_ecdsa_recoverable_signature rawSig;
   if (!secp256k1_ecdsa_recoverable_signature_parse_compact(ctx, &rawSig, sign.data(), v))
      return false;

   secp256k1_pubkey rawPubkey;
   if (!secp256k1_ecdsa_recover(ctx, &rawPubkey, &rawSig, msgHash))
      return false;
   std::vector<uint8_t> rawpubkey{rawPubkey.data, rawPubkey.data + 65};
   std::cout << "recoverpublicKey : " << base16Encode(std::string{rawpubkey.begin(), rawpubkey.end()})
             << " size " << rawpubkey.size() << std::endl;

   std::vector<unsigned char> serializedPubkey(65);
   size_t serializedPubkeySize = serializedPubkey.size();
   secp256k1_ec_pubkey_serialize(
      ctx, serializedPubkey.data(), &serializedPubkeySize, &rawPubkey, SECP256K1_EC_UNCOMPRESSED);
   assert(serializedPubkeySize == serializedPubkey.size());

   // Expect single byte header of value 0x04 -- uncompressed public key.
   assert(serializedPubkey[0] == 0x04);

   // For Testing
   std::cout << "recoverpublicKey serializae : "
             << base16Encode(std::string{serializedPubkey.begin(), serializedPubkey.end()})
             << " size " << serializedPubkey.size() << std::endl;
   std::cout << "pubkey uncompressed : " << base16Encode(std::string{pubKey.begin(), pubKey.end()})
             << " size " << pubKey.size() << std::endl;
   if (pubKey == serializedPubkey) {
      std::cout << "\n * * * pubkey match * * *\n";
   } else
      std::cout << "\n * * * pubkey notmatch * * *\n";

   secp256k1_ecdsa_signature Sig;
   secp256k1_ecdsa_recoverable_signature_convert(ctx, &Sig, &rawSig);
   secp256k1_pubkey parspubkey;
   secp256k1_ec_pubkey_parse(ctx, &parspubkey, serializedPubkey.data(), 65);
   int ret = secp256k1_ecdsa_verify(ctx, &Sig, msgHash, &parspubkey);
   return ret;
}

Secp256K1* Secp256K1::instance = nullptr;
Secp256K1* Secp256K1::getInstance()
{
   if (instance == nullptr) {
      instance = new Secp256K1;
   }
   return instance;
}

void Secp256K1::setPrivKey(const std::vector<uint8_t>& privKey)
{
   Secp256K1::m_privKey = privKey;
}
