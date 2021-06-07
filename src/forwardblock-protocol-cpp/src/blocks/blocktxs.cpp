#include "blocktxs.h"


void Protocol::BlockTxs::append(Protocol::AbstractPreparedTx* tx)
{
   append2Tree(tx->hash().raw(), tx);
}
Protocol::AbstractPreparedTx* Protocol::BlockTxs::index(int dec)
{
   return AbstractMerkleMap::preparedTxIndex(dec);
}
