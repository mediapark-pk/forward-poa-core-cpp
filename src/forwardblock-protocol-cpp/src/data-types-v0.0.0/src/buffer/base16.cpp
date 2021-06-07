
#include "base16.h"
//#include "base16/decoder.h"

Buffer::Base16::Base16(const std::string& data) : Buffer::AbstractBuffer(data) {}

//Buffer::Base16::Base16(Base16& obj) : AbstractBuffer(obj.getBufferData()) {}

std::string Buffer::Base16::validatedDataTypeValue(const std::string& data)
{
   //    CDataTypes obj;
   if (!Validator::isBase16(data)) {
      throw Exception("First argument must be a Hexadecimal value");
   }

   // Remove "0x" prefix
   std::string str_data = data;
   if (data.substr(0, 2) == "0x") {
      str_data = data.substr(2);
   }

   if (str_data.length() % 2 != 0) {
      str_data = "0" + str_data;
   }
   return str_data;
}

std::string Buffer::Base16::hexits(const bool prefix)
{
   std::string hexits = !this->value().empty() ? this->value() : "";
   if (hexits.c_str() && prefix) {
      return "0x" + hexits;
   }
   return hexits;
}

std::shared_ptr<Buffer::Binary> Buffer::Base16::binary()
{
   return std::make_shared<Buffer::Binary>(Binary(hexToBin(this->hexits(false))));
}

Buffer::Base16& Buffer::Base16::operator=(const Buffer::Base16& obj) {
   AbstractBuffer::operator=(obj.value());
   return *this;
}

// std::unique_ptr<CPPBase16::Decoder> Buffer::Base16::decode()
//{
//   CPPBase16::Decoder decoder(*this);
//   return std::make_unique<CPPBase16::Decoder>(decoder);
//}
