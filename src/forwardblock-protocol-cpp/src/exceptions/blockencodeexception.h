#ifndef BLOCK_ENCODE_EXCEPTION_H
#define BLOCK_ENCODE_EXCEPTION_H
#include <exception>
#include <string>

namespace Protocol
{
class BlockEncodeException : public std::exception
{
   std::string m_message;

   public:
   BlockEncodeException(const std::string& message) : m_message{message} {}
   const char* what() const throw()
   {
      return m_message.c_str();
   }
};
}    // namespace Protocol

#endif