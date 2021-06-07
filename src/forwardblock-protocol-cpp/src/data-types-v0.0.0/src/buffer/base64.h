
#ifndef DATA_TYPES_BASE64_H
#define DATA_TYPES_BASE64_H

#include "abstractbuffer.h"
#include "binary.h"

namespace Buffer
{
class Base64 : public Buffer::AbstractBuffer
{
   public:
   Base64(std::string data);
   std::string validatedDataTypeValue(const std::string& data) override;
   std::string encoded();
   Buffer::Binary binary();
};
}    // namespace Buffer

#endif    // DATA_TYPES_BASE64_H