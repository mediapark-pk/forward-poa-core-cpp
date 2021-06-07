//
// Created by umar on 16/12/20.
//

#ifndef BLOCKCHAIN_BLOCKTXRECEIPTS_H
#define BLOCKCHAIN_BLOCKTXRECEIPTS_H

#include "../transactions/abstracttxreceipt.h"
#include "abstractmerklemap.h"


namespace Protocol
{
class BlockTxReceipts : public AbstractMerkleMap
{
   public:
   void append(AbstractTxReceipt* r);
   AbstractTxReceipt* index(int dec);
};
}

#endif    // BLOCKCHAIN_BLOCKTXRECEIPTS_H
