//
// Created by Mahnoor on 6/4/2021.
//

#ifndef FORWARDBLOCK_TRANSACTION_H
#define FORWARDBLOCK_TRANSACTION_H

#include "abstractpreparedTx.h"

namespace Protocol
{
class Transaction : public AbstractPreparedTx
{
   public:
       static AbstractPreparedTx *DecodeAs(AbstractProtocolChain *p, Buffer::Binary encoded);
};

}


#endif    // FORWARDBLOCK_TRANSACTION_H
