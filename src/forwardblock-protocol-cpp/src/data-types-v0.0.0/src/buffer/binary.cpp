
#include "binary.h"
#include "base64.h"
Buffer::Binary::Binary(const std::string& data) : AbstractBuffer(data) {}

std::string Buffer::Binary::validatedDataTypeValue(const std::string& data)
{
   return data.empty() ? "" : data;
}

std::string Buffer::Binary::raw()
{
   return this->value().empty() ? "" : this->value();
}
int Buffer::Binary::sizeInBytes()
{
   return getBufferData().length();
}
int Buffer::Binary::sizeInBits()
{
   return getBufferData().length() * 8;
}

Buffer::Base16 Buffer::Binary::base16()
{
   return Base16(binToHex(this->raw()));
}

Buffer::Base64 Buffer::Binary::base64()
{
   return Buffer::Base64(Utils::base64_encode(this->raw()));
}

CAlgo::Digest Buffer::Binary::hash()
{
   return CAlgo::Digest(*this);
}

Buffer::Binary::Binary() : AbstractBuffer("") {

}
