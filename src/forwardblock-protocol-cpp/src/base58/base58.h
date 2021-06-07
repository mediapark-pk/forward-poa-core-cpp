//
// Created by Mahnoor on 5/26/2021.
//

#ifndef FORWARDBLOCK_BASE58_H
#define FORWARDBLOCK_BASE58_H

#include "base58encoded.h"
#include <string>
namespace Base58
{
class cBase58
{
   public:
   static constexpr char* CHARSET = "123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz";
   static Base58Encoded encode(const std::string& decs, std::string mcharset = "");
   static std::string decode(Base58Encoded encoded, std::string mcharset = "");
   static std::string deocdeFromString(std::string encoded, std::string charset = "");

   private:
   static std::string charset(std::string charset = "");
};

}    // namespace Base58


#endif    // FORWARDBLOCK_BASE58_H
