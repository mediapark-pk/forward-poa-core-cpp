
#include <exception>
#include <string>
namespace Protocol
{
class ProtocolException : std::exception
{
std::string m_message;

   public:
   ProtocolException(const std::string& message) : m_message{message} {}
   const char* what() const throw()
   {
      return m_message.c_str();
   }
};
}    // namespace Protocol
