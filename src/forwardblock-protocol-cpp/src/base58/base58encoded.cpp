//
// Created by Mahnoor on 5/26/2021.
//

#include "base58encoded.h"

std::string Base58::Base58Encoded::validatedDataTypeValue(const std::string& data)
{
   if (data.empty()) {
      spdlog::error("Base58Encoded::{} Base58 objects cannot be constructed without data" ,__FUNCTION__ );
      throw std::runtime_error(
         "Base58 objects cannot be constructed without data");
   }

   if (!m_charset.empty()) {
      if (!std::regex_match(data, std::regex("[" + m_charset + "]+"))) {
         spdlog::error("Base58Encoded::{} Encoded string does not match given Base58 charset" ,__FUNCTION__ );
         throw std::invalid_argument(
            "Encoded string does not match given Base58 charset");
      }
   }
   return data;
}

Base58::Base58Encoded::Base58Encoded(std::string encoded, std::string charset)
   : Buffer::AbstractBuffer{encoded}, m_charset{charset}
{
}
Base58::Base58Encoded::Base58Encoded(Base58::Base58Encoded& copyObj)
   : AbstractBuffer(copyObj.getBufferData()), m_charset{copyObj.m_charset}
{
}
