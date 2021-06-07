//
// Created by umar on 8/12/20.
//

#ifndef BLOCKCHAIN_ABSTRACTMERKLEMAP_H
#define BLOCKCHAIN_ABSTRACTMERKLEMAP_H

#include "../abstractprotocolchain.h"
#include "../transactions/abstractpreparedTx.h"
#include "../transactions/abstracttxreceipt.h"
#include <vector>
#include "../utils/strencodings.h"


namespace Protocol
{
class AbstractMerkleMap
{
   protected:
   AbstractProtocolChain* m_p;
   int m_preparedTxCount;
   int m_txReceiptTxCount;

       // TODO no datatype as Object  in c++
   std::map<std::string, AbstractPreparedTx*> m_preparedTxTree;
   std::map<std::string, AbstractTxReceipt*> m_txReceiptTree;
   std::vector<std::string> m_indexMap;
   std::vector<std::string> m_rawHashes;

   public:
   AbstractMerkleMap() = default;
   AbstractMerkleMap(AbstractProtocolChain* p);

   int getMPreparedTxCount() const;
   int getMTxReceiptTxCount() const;

   void append2Tree(std::string rawHashes, AbstractPreparedTx* obj);
   void append2Tree(std::string rawHashes, AbstractTxReceipt* obj);

   AbstractPreparedTx* preparedTxIndex(int dec);
   AbstractTxReceipt* txReceiptIndex(int dec);

   const Buffer::Binary merkleRoot(int count);//@Argument m_preparedTxCount/m_txReceiptTxCount
   std::map<std::string, AbstractPreparedTx*> allPreparedTx();
   std::map<std::string, AbstractTxReceipt*> allTxReceipt();
   //TODO after php testing implement valid function
//   bool validPreparedTx();
//   bool validTxReceiptTx();

};
}    // namespace Protocol

#endif    // BLOCKCHAIN_ABSTRACTMERKLEMAP_H
