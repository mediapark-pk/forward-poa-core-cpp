#ifndef CHECK_TX_EXCEPTION_H
#define CHECK_TX_EXCEPTION_H

#include "transactionsexception.h"

namespace Protocol
{
class CheckTxException : public std::exception
{
   std::string m_message;

   public:
   CheckTxException(const std::string& message) : m_message{message} {}
   const char* what() const throw()
   {
      return m_message.c_str();
   }
};
}    // namespace Protocol

#endif