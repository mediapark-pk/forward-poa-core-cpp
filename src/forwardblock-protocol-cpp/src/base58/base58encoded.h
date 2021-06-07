//
// Created by Mahnoor on 5/26/2021.
//

#ifndef FORWARDBLOCK_BASE58ENCODED_H
#define FORWARDBLOCK_BASE58ENCODED_H
#include "../data-types-v0.0.0/src/buffer/abstractbuffer.h"
#include "spdlog/spdlog.h"

namespace Base58
{
class Base58Encoded : public Buffer::AbstractBuffer
{
   public:
   Base58Encoded(std::string encoded, std::string charset = "");

   Base58Encoded(Base58::Base58Encoded& copyObj);
   std::string validatedDataTypeValue(const std::string& data) override;

   private:
   std::string m_charset;
};
}    // namespace Base58


#endif    // FORWARDBLOCK_BASE58ENCODED_H
