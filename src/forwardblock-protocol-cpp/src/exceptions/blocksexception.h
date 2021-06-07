
#ifndef BLOCKS_EXCEPTION_H
#define BLOCKS_EXCEPTION_H


#include <exception>
#include <string>

namespace Protocol
{
class BlocksException : public std::exception
{
   std::string m_message;

   public:
   BlocksException(const std::string& message) : m_message{message} {}
   const char* what() const throw()
   {
      return m_message.c_str();
   }
};
}    // namespace Protocol

#endif