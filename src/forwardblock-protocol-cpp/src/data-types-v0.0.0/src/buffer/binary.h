
#ifndef DATA_TYPES_BINARY_H
#define DATA_TYPES_BINARY_H

#include "../algo/digest.h"
#include "abstractbuffer.h"
#include "../algo/digest.h"
#include "base16.h"
#include "../../../utils/strencodings.h"



namespace Buffer
{
class Base64;
class Binary : public AbstractBuffer
{
   public:
   Binary() ;
   Binary(const std::string& data);
   Binary(const Binary&) = default;
   std::string validatedDataTypeValue(const std::string& data) override;
   std::string raw();
   int sizeInBytes();
   int sizeInBits();
   CAlgo::Digest hash();
   Base16 base16();
   Base64 base64();
};
}    // namespace Buffer
#endif    // DATA_TYPES_BINARY_H
