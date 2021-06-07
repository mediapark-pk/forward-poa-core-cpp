

#ifndef DATA_TYPES_ABSTRACTBUFFER_H
#define DATA_TYPES_ABSTRACTBUFFER_H

#include <regex>
#include <string>
#include "../../../utils/utils.h"
#include "../configexception.h"

namespace Buffer
{
class AbstractBuffer
{
   std::string m_data;
   int m_len;
   int m_size;
   bool m_readOnly;

   public:
   AbstractBuffer(std::string data);

   std::string serialize();
   void unSerialize(const std::string& serializeString);
   void setBufferData(const std::string& validatedData);

   std::string value(const int start = 0, const int length = 0) const;
   int getSize();
   int len();
   const std::string &getBufferData();

   void append(std::string data);
   AbstractBuffer* prepend(std::string data);
   AbstractBuffer* readOnly(bool set);
   AbstractBuffer* set(const std::string data = "");

   /** TODO: return bool instead of string in next phase **/
   AbstractBuffer& operator=( std::string data);
   virtual std::string validatedDataTypeValue(const std::string& data) = 0;
};
};    // namespace Buffer

#endif    // DATA_TYPES_ABSTRACTBUFFER_H