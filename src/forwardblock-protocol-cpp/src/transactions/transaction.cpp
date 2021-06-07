//
// Created by Mahnoor on 6/4/2021.
//

#include "transaction.h"
#include "txflags.h"


Protocol::AbstractPreparedTx* Protocol::Transaction::DecodeAs(
   Protocol::AbstractProtocolChain* p, Buffer::Binary encoded)
{
   auto flagId = encoded.value(1,2);
   if (flagId.empty()) {
      // TODO :: throw Exception
   }
   return p->txFlags()->getFlag(Math::Decode_UInt2LE(flagId))->decode(encoded);

}
