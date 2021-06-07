//
// Created by umar on 5/20/21.
//

#ifndef FORWARDBLOCK_ABSTRACTPREPAREDTX_H
#define FORWARDBLOCK_ABSTRACTPREPAREDTX_H

#include "../validator.h"
#include "abstracttx.h"
#include "buffer/binary.h"
#include "../exceptions/txdecodeexception.h"
#include "../keypair/privatekey/signature.h"
#include "spdlog/spdlog.h"

namespace Protocol
{
class AbstractPreparedTx : public AbstractTx
{
   void decodeTxV1(Buffer::Binary read);

   protected:
   Buffer::Binary m_hash;
   Buffer::Binary m_raw;
   virtual void decodeCallback() = 0;

   public:
   static AbstractPreparedTx* Decode(
      AbstractPreparedTx* self, AbstractProtocolChain* p, Buffer::Binary encoded);
   AbstractPreparedTx(AbstractProtocolChain* p, Buffer::Binary bytes);
   Buffer::Binary hash();
   Buffer::Binary raw();
   int version();
   int flag();
   std::string sender();
   int nonce();
   std::string recipient();
   std::string memo();
   std::map<std::string, int> transfers();
   std::string data();
   std::vector<Signature> signatures();
   int fee();
   int timeStamp();
};
}    // namespace Protocol


#endif    // FORWARDBLOCK_ABSTRACTPREPAREDTX_H
