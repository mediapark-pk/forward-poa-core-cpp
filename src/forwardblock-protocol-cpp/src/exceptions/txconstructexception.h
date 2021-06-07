#ifndef TX_CONSTRUCT_EXCEPTION_H
#define TX_CONSTRUCT_EXCEPTION_H

#include <exception>
#include <string>

namespace Protocol
{
class TxConstructException : public std::exception
{
std::string m_message;

   public:
   TxConstructException(const std::string& message) : m_message{message} {}
   const char* what() const throw()
   {
      return m_message.c_str();
   }
};
}    // namespace Protocol

#endif