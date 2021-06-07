//
// Created by umar on 23/12/20.
//

#include "base58check.h"
protocol::Base58check protocol::Base58check::instance;

protocol::Base58check& protocol::Base58check::getInstance()
{
   protocol::Base58check::instance.charset(Base58check::CHARSET);
   protocol::Base58check::instance.checksum(Base58check::CHECKSUM_BYTES);
   return instance;
}


protocol::Base58check::Base58check() {}
