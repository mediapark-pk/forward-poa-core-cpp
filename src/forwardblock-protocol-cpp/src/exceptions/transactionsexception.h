#ifndef FORWARDBLOCK_TRANSACTIONSEXCEPTION_H
#define FORWARDBLOCK_TRANSACTIONSEXCEPTION_H

#include <exception>
#include <string>

namespace Protocol
{
class TransactionsException : public std::exception
{
std::string m_message;

   public:
   TransactionsException(const std::string& message) : m_message{message} {}
   const char* what() const throw()
   {
      return m_message.c_str();
   }
};
}    // namespace Protocol
#endif