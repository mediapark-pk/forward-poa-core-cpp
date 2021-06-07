#ifndef SIGN_MESSAGE_EXCEPTION_H
#define SIGN_MESSAGE_EXCEPTION_H

#include <exception>
#include <string>

namespace Protocol
{
class SignMessageException : public std::exception
{
std::string m_message;

   public:
   SignMessageException(const std::string& message) : m_message{message} {}
   const char* what() const throw()
   {
      return m_message.c_str();
   }
};
}    // namespace Protocol
#endif