#ifndef KEY_PAIR_EXCEPTION_H
#define KEY_PAIR_EXCEPTION_H
#include <exception>
#include <string>

namespace Protocol
{
class KeyPairException : public std::exception
{
std::string m_message;

   public:
   KeyPairException(const std::string& message) : m_message{message} {}
   const char* what() const throw()
   {
      return m_message.c_str();
   }
};
}    // namespace Protocol
#endif