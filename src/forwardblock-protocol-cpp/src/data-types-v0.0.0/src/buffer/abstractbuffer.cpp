//
// Created by umar on 10/12/20.
//

#include "abstractbuffer.h"

Buffer::AbstractBuffer::AbstractBuffer( std::string data)
{
   m_data = data;
   m_readOnly = false;
   m_len = 0;
   m_size = 0;
}

std::string Buffer::AbstractBuffer::serialize()
{
   char buffer[50];
   sprintf(
      buffer,
      "%d:%d:%s",
      this->m_readOnly ? 1 : 0,
      this->m_size,
      Utils::base64_encode(this->m_data).c_str());

   std::string serialize_str = buffer;

   return serialize_str;
}

void Buffer::AbstractBuffer::unSerialize(const std::string& serializeString)
{
   if (!std::regex_match(serializeString, std::regex("^[0-1]:[0-9]+:[a-z0-9]+={0,2}$"))) {
      throw Exception("Invalid networkPassphrase");
   }
   std::vector<std::string> splits;
   std::stringstream stream(serializeString);
   std::string buffer = "";
   while (std::getline(stream, buffer, ':')) {
      splits.push_back(buffer);
   }

   int dataSize = std::stoi(splits[1]);

   // Construct object
   this->m_readOnly = false;
   this->m_data = "";
   this->m_len = 0;
   this->m_size = 0;

   // Restore data
   std::string output;
   output = Utils::base64_decode(splits[2]);
   this->set(output);
   if (this->m_size != dataSize) {
      throw Exception("Serialized data size does not match");
   }

   // ReadOnly flag
   this->m_readOnly = (splits[0] == "1");
   // splits[0] == "1" ? this->m_readOnly = true : this->m_readOnly = false;
}

void Buffer::AbstractBuffer::setBufferData(const std::string& validatedData)
{
   if (this->m_readOnly) {
      throw Exception("Buffer is in read-only state");
   }

   this->m_data = validatedData;
   this->m_size = std::strlen(this->m_data.c_str());
   this->m_len = this->m_data.length();
}

Buffer::AbstractBuffer* Buffer::AbstractBuffer::readOnly(bool set)
{
   this->m_readOnly = set;
   return this;
}

Buffer::AbstractBuffer* Buffer::AbstractBuffer::set(const std::string data)
{
   if (!data.empty()) {
      this->setBufferData(this->validatedDataTypeValue(data));
   }

   return this;
}

int Buffer::AbstractBuffer::len()
{
   return this->m_data.length();
}

void Buffer::AbstractBuffer::append(std::string data)
{
   std::string validated = this->validatedDataTypeValue(data);
   this->setBufferData(this->m_data + validated);
}

Buffer::AbstractBuffer* Buffer::AbstractBuffer::prepend(std::string data)
{
   std::string validated = this->validatedDataTypeValue(data);
   this->setBufferData(validated + this->m_data);
   return this;
}

std::string Buffer::AbstractBuffer::value(const int start, const int length) const
{
   std::string data = m_data;
   if (start > 0) {
      /// cross check required on below line
      length > 0 ? data = data.substr(start, length) : data = data.substr(start);
      if (data.empty()) {
         return "";
      }
   }
   return data;
}

const std::string& Buffer::AbstractBuffer::getBufferData()
{
   return m_data;
}
int Buffer::AbstractBuffer::getSize()
{
   return m_size;
}
Buffer::AbstractBuffer& Buffer::AbstractBuffer::operator=( std::string data)
{
   m_data = data;
   m_size = 0;
   m_len = 0;
   m_readOnly = false;
   return *this;
}
