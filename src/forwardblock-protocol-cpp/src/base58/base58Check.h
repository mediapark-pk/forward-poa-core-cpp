//
// Created by umar on 23/12/20.
//

#ifndef BLOCKCHAIN_BASE58BASE58CHECK_H
#define BLOCKCHAIN_BASE58BASE58CHECK_H

#include "../data-types-v0.0.0/src/buffer/base16.h"
#include "base58encoded.h"
#include "base58.h"
#include <regex>
#include <string>

namespace Base58
{
class Base58Check
{
   public:
   static const int CHECKSUM_BYTES = 4;

   Base58Check();

   Base58Check charset(std::string charset);
   Base58Check checksum(int bytes, Buffer::Binary (*m_checksumCalculateFunc)(std::string data) = nullptr);

   /** decodes the base58 string to base16
    * @param encoded string in base58
    * @return base16 object with decoded base58 string as its value
    */
   Buffer::Base16 decode( std::string encoded, bool convertLeadingOnes = true);

   /** ecodes hexstring to base58
    *  @param hexits string of hexadecimal
    *  @return Base58Check object with encode hexstring as its value
    */
   Base58Encoded encode(std::string hexits);

   private:
   std::string m_charset;
   int m_checksumBytes;
   // function pointer  not sure what the usecase will be like
   Buffer::Binary (*m_checksumCalculateFunc)(std::string data);
};

}    // namespace Base58

#endif    // BLOCKCHAIN_BASE58BASE58CHECK_H
