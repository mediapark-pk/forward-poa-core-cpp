#ifndef BLOCKCHAIN_SIGNATURE_H
#define BLOCKCHAIN_SIGNATURE_H

#include "../../data-types-v0.0.0/src/buffer/base16.h"
#include <stdexcept>
#include <string>
#include <tuple>
#include "spdlog/spdlog.h"

namespace Protocol
{
class Signature
{
   private:
   Buffer::Base16 m_r;
   Buffer::Base16 m_s;
   int m_v;

   public:
   Signature& operator=(const Signature& signature);
   Signature(Buffer::Base16 r, Buffer::Base16 s, int& v);
   Signature(const Signature& signature);
   std::tuple<Buffer::Base16, Buffer::Base16, int> array();
   Buffer::Base16 r();
   Buffer::Base16 s();
   int v();


};
}    // namespace Protocol
#endif    // BLOCKCHAIN_SIGNATURE_H