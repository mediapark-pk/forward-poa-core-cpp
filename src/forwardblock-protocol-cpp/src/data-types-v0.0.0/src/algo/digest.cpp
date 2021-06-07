
#include "digest.h"
#include "../buffer/base16.h"
#include "../../../crypto/hmac.h"
#include "../../../crypto/md5.h"
#include "../../../crypto/pbkdf2.h"
#include "../../../crypto/ripemd160.h"
#include "../../../crypto/sha1.h"

CAlgo::Digest::Digest(Buffer::Binary& mBuffer) : m_buffer{mBuffer} {}

Buffer::Binary CAlgo::Digest::result(const std::string& bytes)
{
   return Buffer::Binary(bytes);
}

Buffer::Binary CAlgo::Digest::digest(const std::string& algo, const int iterations, const int bytes)
{
   std::string hashed = this->m_buffer.raw();
   if (algo == "md5") {
      for (int i = 0; i < iterations; i++) {
         hashed = ::md5(hashed);
      }
   } else if (algo == "sha1") {
      SHA1 checkSum;
      for (int i = 0; i < iterations; i++) {
         checkSum.update(hashed);
         hashed = checkSum.final();
      }
   } else if (algo == "sha256") {
      for (int i = 0; i < iterations; i++) {
         auto hash = ::sha256(hashed);
         hashed = {hash.begin(), hash.end()};
      }
   } else if (algo == "ripemd160") {
      for (int i = 0; i < iterations; i++) {
         std::vector<unsigned char> hash({hashed.begin(), hashed.end()});
         hash = ::ripemd160(hash);
         hashed = {hash.begin(), hash.end()};
      }
   } else {
      throw std::runtime_error("Encoding algorithm is incorrect");
   }
   if (bytes) {
      hashed = hashed.substr(0, bytes);
   }
   return this->result(hashed);
}

Buffer::Binary CAlgo::Digest::md5(int bytes)
{
   return this->digest("md5", 1, bytes);
}

Buffer::Binary CAlgo::Digest::sha1(int bytes)
{
   return this->digest("sha1", 1, bytes);
}

Buffer::Binary CAlgo::Digest::sha256(int bytes)
{
   return this->digest("sha256", 1, bytes);
}

Buffer::Binary CAlgo::Digest::ripeMd160(int bytes)
{
   return this->digest("ripemd160", 1, bytes);
}

Buffer::Binary CAlgo::Digest::pbkdf2(
   const std::string& algo, Buffer::Binary salt, int iterations, int length)
{
   std::string d_salt = salt.raw();
   if (algo == "sha256") {
      std::vector<unsigned char> value = ::hashPbkdf2(
         SHA::SHA256, m_buffer.raw(), d_salt, iterations, length);
      return result({value.begin(), value.end()});
   } else if (algo == "sha512") {
      std::vector<unsigned char> value = ::hashPbkdf2(
         SHA::SHA512, m_buffer.raw(), d_salt, iterations, length);
      return result({value.begin(), value.end()});
   }
   else
   {
      throw;
   }
}

Buffer::Binary CAlgo::Digest::hmac(const std::string& algo, Buffer::Base16 &key)
{
   std::string rawKey = key.binary()->raw();
   if (algo == "Sha256" || algo == "sha256" || algo == "SHA256") {
      std::vector<unsigned char> hash = hashHmac(
         static_cast<HMAC_ALGO>(SHA::SHA256), m_buffer.raw(), rawKey);
      return std::string{hash.begin(), hash.end()};
   } else if (algo == "sha512" || algo == "Sha512" || algo == "SHA512") {
      std::vector<unsigned char> hash = hashHmac(
         static_cast<HMAC_ALGO>(SHA::SHA512), m_buffer.raw(), rawKey);
      return std::string{hash.begin(), hash.end()};
   }
   else
      throw;
}

Buffer::Binary CAlgo::Digest::hmac(const std::string& algo, Buffer::Binary key)
{
   std::string rawKey = key.raw();
   if (algo == "Sha256" || algo == "sha256" || algo == "SHA256") {
      std::vector<unsigned char> hash = hashHmac(
         static_cast<HMAC_ALGO>(SHA::SHA256), m_buffer.raw(), rawKey);
      return std::string{hash.begin(), hash.end()};
   } else if (algo == "sha512" || algo == "Sha512" || algo == "SHA512") {
      std::vector<unsigned char> hash = hashHmac(
         static_cast<HMAC_ALGO>(SHA::SHA512), m_buffer.raw(), rawKey);
      return std::string{hash.begin(), hash.end()};
   }
   else
      throw;
}
