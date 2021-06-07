//
// Created by umar on 5/20/21.
//

#ifndef FORWARDBLOCK_ABSTRACTTXRECEIPT_H
#define FORWARDBLOCK_ABSTRACTTXRECEIPT_H

#include "../abstractprotocolchain.h"
#include "abstractpreparedTx.h"
#include "buffer/binary.h"
#include "../exceptions/txreceiptdecodeexception.h"
#include "receipts/ledgerentries.h"
#include "receipts/ledgerflags.h"
#include "spdlog/spdlog.h"

namespace Protocol
{
// class LedgerEntry;
class AbstractTxReceipt
{
   Buffer::Binary halfSerialize();

   protected:
   AbstractProtocolChain* m_p;
   AbstractPreparedTx* m_tx;
   int m_blockHeightContext;
   int m_status;
   Buffer::Binary m_data;
   LedgerEntries m_ledgerEntries;
   int m_totalIn;
   int m_totalOut;

   int m_totalFee;
   virtual void defaultLedgerEntries() = 0;
   virtual void generateLedgerEntries() = 0;
   virtual void calculateTotalFee() = 0;
   virtual void finalizeCallback() = 0;

   public:
   virtual void applyCallback() = 0;
   virtual void undoCallback() = 0;
   static AbstractTxReceipt* Decode(
      AbstractProtocolChain* m_p,
      AbstractPreparedTx* m_tx,
      int m_blockHeightContext,
      Buffer::Binary m_encoded,
      AbstractTxReceipt* m_receipt);
   AbstractTxReceipt(AbstractProtocolChain* p, AbstractPreparedTx* tx, int blockHeightContext);
   void calculateLedgerEntries(bool applicableOnly = false);
   std::tuple<int, std::string, std::tuple<int, int, std::vector<std::vector<Protocol::LedgerEntries>>>, int, int, int> dump();
   bool isFinalised();
   AbstractPreparedTx* getTx();
   Buffer::Binary data();
   void setStatus(int code);
   int totalIn();
   int totalOut();
   int totalFee();
   int status();
   LedgerEntries ledgerEntries();
   Buffer::Binary ledgerEntriesHash();
   Buffer::Binary getReceiptHash();
   Buffer::Binary serialize();
   LedgerEntry createLedgerEntry(
      LedgerFlag lf, std::string hash160, int amount, std::string assetId = nullptr);
   void registerLedgerEntriesBatch(std::vector<LedgerEntry>& entries);
};
}    // namespace Protocol

#endif    // FORWARDBLOCK_ABSTRACTTXRECEIPT_H
