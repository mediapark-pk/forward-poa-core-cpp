//
// Created by Mahnoor on 5/26/2021.
//

#include "base58.h"
std::string Base58::cBase58::charset(std::string charset)
{
   charset =!"" ? charset : cBase58::CHARSET;
   if(charset.length() != 58)
   {
      spdlog::error("cBase58::{} " ,__FUNCTION__ );
      throw std::runtime_error("cBase58::charset, Base58 charsets must have exactly 58 digits");
   }
   return charset;
}

Base58::Base58Encoded Base58::cBase58::encode(const std::string& decs, std::string mcharset)
{
   auto base58 = Utils::fromBase10(decs.c_str(), charset(mcharset));
   auto base58Encoded = Base58Encoded(base58);
   base58Encoded.readOnly(true);
   return base58Encoded;
}

std::string Base58::cBase58::decode(Base58::Base58Encoded encoded, std::string mcharset)
{
   return Utils::toBase10(encoded.value(), cBase58::charset(mcharset));
}

std::string Base58::cBase58::deocdeFromString(std::string encoded, std::string mcharset)
{
   return decode(Base58Encoded(encoded), charset(mcharset));
}
