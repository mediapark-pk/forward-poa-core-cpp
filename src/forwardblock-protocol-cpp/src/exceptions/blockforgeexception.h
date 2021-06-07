
#ifndef BLOCK_FORGE_EXCEPTION_H
#define BLOCK_FORGE_EXCEPTION_H

#include <exception>
#include <string>

namespace Protocol
{
class BlockForgeException : public std::exception
{
   std::string m_message;

   public:
   BlockForgeException(const std::string& message) : m_message{message} {}
   const char* what() const throw()
   {
      return m_message.c_str();
   }
};
}    // namespace Protocol

#endif