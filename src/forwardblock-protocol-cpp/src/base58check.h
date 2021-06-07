//
// Created by umar on 23/12/20.
//

#ifndef BLOCKCHAIN_BASE58CHECK_H
#define BLOCKCHAIN_BASE58CHECK_H

#include "base58/base58Check.h"
#include <buffer/binary.h>
#include <regex>
#include <string>

namespace protocol
{
class Base58check : public Base58::Base58Check
{
   public:
   static constexpr char* CHARSET = "123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz";
   static const int CHECKSUM_BYTES = 4;

   static Base58check& getInstance();

   private:
   static Base58check instance;

   Base58check();

};

}    // namespace protocol

#endif    // BLOCKCHAIN_BASE58CHECK_H
