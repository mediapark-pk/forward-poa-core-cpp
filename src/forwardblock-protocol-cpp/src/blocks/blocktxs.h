#ifndef BLOCKCHAIN_BLOCKTXS_H
#define BLOCKCHAIN_BLOCKTXS_H

#include <map>
#include <memory>
#include <vector>

#include "../transactions/abstractpreparedTx.h"
#include "abstractmerklemap.h"


namespace Protocol
{
class BlockTxs : public AbstractMerkleMap
{
   public:
   void append(AbstractPreparedTx* tx);
   AbstractPreparedTx* index(int dec);
};
}    // namespace Protocol

#endif
