#include "signature.h"

Protocol::Signature::Signature(Buffer::Base16 r, Buffer::Base16 s, int& v) : m_s{s}, m_r{r}
{
   if (r.getBufferData().size() != 64) {
      spdlog::error(
         "Signature::{} Signature R must be precisely 32 bytes",  __FUNCTION__);
      throw std::length_error("Signature R must be precisely 32 bytes");
   }
   if (s.getBufferData().size() != 64) {
      spdlog::error(
         "Signature::{} Signature S must be precisely 32 bytes",  __FUNCTION__);
      throw std::length_error("Signature S must be precisely 32 bytes");
   }
   if (v < 0 || v > 0xff) {
      spdlog::error("Signature::{} Signature V is out of range",  __FUNCTION__);
      throw std::out_of_range("Signature V is out of range");
   }
   m_r = r;
   m_s = s;
   m_v = v;
}

std::tuple<Buffer::Base16, Buffer::Base16, int> Protocol::Signature::array() {
    return std::make_tuple(m_r, m_s, m_v);
}

Buffer::Base16 Protocol::Signature::r() {
    return m_r;
}

Buffer::Base16 Protocol::Signature::s() {
    return m_s;
}

int Protocol::Signature::v()
{
   return m_v;
}
Protocol::Signature::Signature(const Protocol::Signature& signature)
   : m_s{signature.m_s}, m_r{signature.m_r}, m_v{signature.m_v}
{
}

Protocol::Signature& Protocol::Signature::operator=(const Protocol::Signature& signature)
{
   m_r = signature.m_r;
   m_s = signature.m_s;
   m_v = signature.m_v;
   return *this;
}