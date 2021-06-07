//
// Created by umar on 10/12/20.
//

#ifndef DATA_TYPES_BASE16_H
#define DATA_TYPES_BASE16_H

#include "../../../utils/strencodings.h"
#include "../validator.h"
#include "abstractbuffer.h"
#include "binary.h"
#include <memory>

namespace Buffer
{
class Base16 : public Buffer::AbstractBuffer
{
   // TODO:: {JUST IN CASE} CPPBase16::Decoder *m_decoder;
   public:
   Base16() = default;

   Base16(const std::string& data);    // check required
   std::string validatedDataTypeValue(const std::string& data) override;

//   Base16(Base16& obj);
   Base16& operator=(const Base16& obj);
   std::string hexits(bool prefix = false);
   std::shared_ptr<Buffer::Binary> binary();
   // TODO:: WILL CHECK IF WE NEED DECODE CLASS OR NOT
   //   std::unique_ptr<CPPBase16::Decoder> decode();
};

};    // namespace Buffer

#endif    // DATA_TYPES_BASE16_H
