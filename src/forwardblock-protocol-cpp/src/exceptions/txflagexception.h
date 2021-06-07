#ifndef TRANSACTION_EXCEPTION_H
#define TRANSACTION_EXCEPTION_H

#include <exception>
#include <string>

namespace Protocol
{
class TxFlagException : public std::exception
{
std::string m_message;

   public:
   TxFlagException(const std::string& message) : m_message{message} {}
   const char* what() const throw()
   {
      return m_message.c_str();
   }
};
}    // namespace Protocol

#endif