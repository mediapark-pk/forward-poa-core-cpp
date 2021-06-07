//
// Created by umar on 23/12/20.
//

#include "base58Check.h"

Base58::Base58Check Base58::Base58Check::charset(std::string charset)
{
   if (charset.length() != 58) {
      spdlog::error(
         "Base58Check::{} Base58 charsets must have exactly 58 digits",  __FUNCTION__);
      throw std::length_error("Base58 charsets must have exactly 58 digits");
   }
   m_charset = charset;
   return *this;
}

Base58::Base58Check Base58::Base58Check::checksum(
   int bytes, Buffer::Binary (*checksumCalculateFunc)(std::string))
{
   if (bytes < 0) {
      spdlog::error(
         "Base58Check::{} Checksum bytes must be positive integer",  __FUNCTION__);
      throw std::invalid_argument("Checksum bytes must be positive integer");
   }
   m_checksumBytes = bytes;
   m_checksumCalculateFunc = checksumCalculateFunc;
   return *this;
}


Buffer::Base16 Base58::Base58Check::decode(std::string encoded, bool convertLeadingOnes)
{
   Base58Encoded bs58Ec(encoded);
   encoded = bs58Ec.value();
   int leadingones = 0;
   if (convertLeadingOnes) {
      leadingones = encoded.length() - encoded.substr(encoded.find_first_not_of("1")).length();
   }

   auto base58Charset = !m_charset.empty() ? m_charset : Base58::cBase58::CHARSET;
   auto base58Decode = Utils::toBase10(encoded, base58Charset);
   auto data = Utils::fromBase10(base58Decode.c_str(), Utils::CHARSET_BASE16);

   auto checksumLen = m_checksumBytes != 0 ? m_checksumBytes : Base58Check::CHECKSUM_BYTES;
   std::string checksum = "";
   if (checksumLen > 0) {
      auto checksumHexits = checksumLen * 2;
      checksum = data.substr(data.length() - checksumHexits);
      data = data.substr(0, data.length() - checksumHexits);
   }
   if (leadingones > 0) {
      data = std::string(leadingones, '0') + std::string(leadingones, '0') + data;
   }

   Buffer::Base16 dataB16 = data;
   Buffer::Binary validateChecksum("");
   if (!checksum.empty()) {
      if (m_checksumCalculateFunc != nullptr) {
         validateChecksum = m_checksumCalculateFunc(data);
      } else {
         validateChecksum = dataB16.binary()->hash().digest("sha256", 2, checksumLen);
      }
      if (checksum != validateChecksum.base16().hexits()) {
         spdlog::error(
            "Base58Check::{} Base58check decoded checksum does not match",  __FUNCTION__);
         throw std::invalid_argument("Base58check decoded checksum does not match");
      }
   }
   return dataB16;
}

Base58::Base58Encoded Base58::Base58Check::encode(std::string hexits)
{
   if (
      !std::regex_match(hexits, std::regex("^(0x)?[A-F0-9]+$")) &&
      !std::regex_match(hexits, std::regex("^(0x)?[a-f0-9]+$"))) {
      spdlog::error(
         "Base58Check::{} Only hexadecimal numbers can be decoded",  __FUNCTION__);
      throw std::invalid_argument("Only hexadecimal numbers can be decoded");
   }
   if (hexits.substr(0, 2) == "0x") {
      hexits = hexits.substr(2);
   }

   Buffer::Base16 buffer = hexits;
   auto checksumBytes = m_checksumBytes != 0 ? m_checksumBytes : Base58Check::CHECKSUM_BYTES;

   Buffer::Binary checksum("");
   if (m_checksumCalculateFunc != nullptr) {
      checksum = m_checksumCalculateFunc(buffer.getBufferData());
   } else {
      checksum = buffer.binary()->hash().digest("sha256", 2, m_checksumBytes);
   }

   // verify checksum length in bytes
   if (checksum.sizeInBytes() != checksumBytes) {
      spdlog::error(
         "Base58Check::{} Base58Check checksum must be precisely {}",
         __FUNCTION__, std::to_string(CHECKSUM_BYTES));
      throw std::invalid_argument(
         "Base58Check checksum must be precisely " + std::to_string(CHECKSUM_BYTES) +
         " bytes long, got" + std::to_string(checksum.sizeInBytes()) + " bytes \n");
   }
   buffer.append(checksum.base16().value());

   int leadingZeros = 0;
   leadingZeros = hexits.find_first_not_of("0");
   leadingZeros = leadingZeros / 2;
   std::string hex2dec = Utils::toBase10(buffer.hexits(false), Utils::CHARSET_BASE16, false);
   auto base58charet = m_charset != "" ? m_charset : cBase58::CHARSET;
   auto base58Encoded = Utils::fromBase10(hex2dec.c_str(), base58charet);
   if (leadingZeros > 0) {
      base58Encoded = std::string(leadingZeros, '1') + base58Encoded;
   }
   auto base58Ec = Base58Encoded(base58Encoded);
   base58Ec.readOnly(true);
   return base58Ec;
}

Base58::Base58Check::Base58Check() {}
