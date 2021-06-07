#ifndef PROTOCOL_CONFIG_EXCEPTION_H
#define PROTOCOL_CONFIG_EXCEPTION_H

#include <exception>
#include <string>

namespace Protocol
{
class ProtocolConfigException : public std::exception
{
std::string m_message;

   public:
   ProtocolConfigException(const std::string& message) : m_message{message} {}
   const char* what() const throw()
   {
      return m_message.c_str();
   }
};
}    // namespace Protocol
#endif