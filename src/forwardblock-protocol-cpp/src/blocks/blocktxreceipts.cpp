//
// Created by umar on 16/12/20.
//

#include "blocktxreceipts.h"


void Protocol::BlockTxReceipts::append(Protocol::AbstractTxReceipt* r)
{
   append2Tree(r->getReceiptHash().raw(), r);
}
Protocol::AbstractTxReceipt* Protocol::BlockTxReceipts::index(int dec)
{
   return AbstractMerkleMap::txReceiptIndex(dec);
}
