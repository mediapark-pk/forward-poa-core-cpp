//
// Created by umar on 28/5/21.
//

#ifndef FORWARDBLOCK_TXRECEIPTDECODEEXCEPTION_H
#define FORWARDBLOCK_TXRECEIPTDECODEEXCEPTION_H

#include <exception>
#include <string>
namespace Protocol{
class TxReceiptDecodeException : public std::exception
{
   std::string m_message;

   public:
   TxReceiptDecodeException(const std::string& message) : m_message{message} {}
   const char* what() const throw()
   {
      return m_message.c_str();
   }
};
}



#endif    // FORWARDBLOCK_TXRECEIPTDECODEEXCEPTION_H
