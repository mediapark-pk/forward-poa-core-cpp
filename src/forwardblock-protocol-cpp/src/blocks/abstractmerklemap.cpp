//
// Created by umar on 8/12/20.
//

#include "abstractmerklemap.h"


Protocol::AbstractMerkleMap::AbstractMerkleMap(Protocol::AbstractProtocolChain* p)
   : m_p{p}
   , m_preparedTxCount{0}
   , m_txReceiptTxCount{0}
   , m_txReceiptTree{}
   , m_preparedTxTree{}
   , m_indexMap{}
   , m_rawHashes{}
{
}

/** TODO : In php it is object not sure what it is in cpp **/

void Protocol::AbstractMerkleMap::append2Tree(std::string rawHashes, AbstractPreparedTx* obj)
{
   std::string hexKey = binToHex(rawHashes);
   m_preparedTxTree[hexKey] = obj;
   m_indexMap.push_back(hexKey);
   m_rawHashes.push_back(rawHashes);
   m_preparedTxCount++;
}

void Protocol::AbstractMerkleMap::append2Tree(std::string rawHashes, AbstractTxReceipt* obj)
{
   std::string hexKey = binToHex(rawHashes);
   m_txReceiptTree[hexKey] = obj;
   m_indexMap.push_back(hexKey);
   m_rawHashes.push_back(rawHashes);
   m_txReceiptTxCount++;
}

Protocol::AbstractPreparedTx* Protocol::AbstractMerkleMap::preparedTxIndex(int dec)
{
   return m_preparedTxTree[m_indexMap[dec]];
}

Protocol::AbstractTxReceipt* Protocol::AbstractMerkleMap::txReceiptIndex(int dec)
{
   return m_txReceiptTree[m_indexMap[dec]];
}

const Buffer::Binary Protocol::AbstractMerkleMap::merkleRoot(int count)
{
   if (count) {
      std::string em(32, '\0');
      Buffer::Binary bin(em);
      return bin;
   }
   Buffer::Binary passTofunction("");
   for (int i = 0; i < m_rawHashes.size(); ++i) {
      std::string temp = m_rawHashes[i];
      passTofunction.append(temp);
   }
   return m_p->hash256(passTofunction);
}


std::map<std::string, Protocol::AbstractPreparedTx*> Protocol::AbstractMerkleMap::allPreparedTx()
{
   return m_preparedTxTree;
}
std::map<std::string, Protocol::AbstractTxReceipt*> Protocol::AbstractMerkleMap::allTxReceipt()
{
   return m_txReceiptTree;
}
int Protocol::AbstractMerkleMap::getMPreparedTxCount() const
{
   return m_preparedTxCount;
}
int Protocol::AbstractMerkleMap::getMTxReceiptTxCount() const
{
   return m_txReceiptTxCount;
}

