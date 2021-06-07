//
// Created by umar on 5/18/21.
//

#ifndef DATA_TYPES_V0_0_0_VALIDATOR_H
#define DATA_TYPES_V0_0_0_VALIDATOR_H

#include <string>
#include <regex>
namespace Validator
{
static bool isBase16(const std::string& value)
{
   if (
      value.c_str() &&
      (std::regex_match(value, std::regex("^(0x)?[a-f0-9]+$")) ||
       std::regex_match(value, std::regex("^(0x)?[A-F0-9]+$"))))
      return true;
   else
      return false;
}

static bool isHex(std::string val)
{
   return isBase16(val);
}

static bool isBase64(std::string val)
{
   if (val.c_str() && std::regex_match(val, std::regex("[a-zA-Z0-9+\\/]+={0,2}")))
      return true;
   else
      return false;
}

static bool isUtf8(std::string val)
{
   return val.length() != val.size();
}

static bool isBitwise(std::string val)
{
   if (val.c_str() && std::regex_match(val, std::regex("^[01]+$")))
      return true;
   else
      return false;
}

}    // namespace Validator


#endif    // DATA_TYPES_V0_0_0_VALIDATOR_H
