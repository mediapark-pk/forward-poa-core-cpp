//
// Created by umar on 5/20/21.
//

#ifndef FORWARDBLOCK_ABSTRACTTXFLAG_H
#define FORWARDBLOCK_ABSTRACTTXFLAG_H

#include "abstractpreparedTx.h"
#include "abstracttxconstructor.h"
#include "abstracttxreceipt.h"
#include "buffer/binary.h"
#include "spdlog/spdlog.h"
#include "../validator.h"


namespace Protocol
{
class AbstractProtocolChain;
class AbstractTxFlag
{
   protected:
   Protocol::AbstractProtocolChain* m_p;
   int m_id;
   std::string m_name;

   public:
   AbstractTxFlag(Protocol::AbstractProtocolChain& p, const int id, const std::string& name);
   int id();
   std::string name();
   /** TODO :: array $args  Not clear yet */
   virtual Protocol::AbstractTxConstructor* create() = 0;
   virtual Protocol::AbstractPreparedTx* decode(const Buffer::Binary& encoded) = 0;
   virtual Protocol::AbstractTxReceipt* newReceipt(Protocol::AbstractPreparedTx* tx, int blockHeight) = 0;
   virtual Protocol::AbstractTxReceipt* decodeReceipt(
      Protocol::AbstractPreparedTx* tx, Buffer::Binary bytes) = 0;
};
};    // namespace Protocol


#endif    // FORWARDBLOCK_ABSTRACTTXFLAG_H
