//
// Created by umar on 5/20/21.
//

#include "abstracttxreceipt.h"
#include "receipts/ledgerentry.h"
#include "txflags.h"


Protocol::AbstractTxReceipt* Protocol::AbstractTxReceipt::Decode(
   Protocol::AbstractProtocolChain* p,
   Protocol::AbstractPreparedTx* tx,
   int blockHeightContext,
   Buffer::Binary encoded,
   AbstractTxReceipt* receipt)
{
   /** TODO::Might review it later **/
   int next = 32;
   receipt->m_ledgerEntries.purge();
   std::string read = encoded.getBufferData();
   std::string txId = read.substr(0, next);

   // Step 1
   if (txId != tx->hash().raw()) {
      spdlog::error(
         "AbstractTxReceipt::{} Receipt for tx {} does not match transaction hash",
         __FUNCTION__,
         binToHex(txId),
         binToHex(tx->hash().raw()));
      throw TxReceiptDecodeException(
         "Receipt for tx " + (binToHex(txId)) + " does not match transaction hash " +
         binToHex(tx->hash().raw()));
   }

   // step2
   auto status = Math::Decode_UInt2LE(read.substr(next, 2));
   next += 2;
   receipt->setStatus(status);

   // step3
   int dataLen = Math::Decode_UInt1LE(read.substr(next, 2));
   next += 2;
   if (dataLen > 0) {
      receipt->data().append(read.substr(next, dataLen));
   }

   // step 4,5,6
   int totalIn = Math::Decode_UInt8LE(read.substr(next, 8));
   next += 8;
   int totalOut = Math::Decode_UInt8LE(read.substr(next, 8));
   next += 8;
   int totalFee = Math::Decode_UInt8LE(read.substr(next, 8));
   next += 8;

   // step 7
   int leBatches = Math::Decode_UInt1LE(read.substr(next, 1));
   ++next;
   if (leBatches > 0) {
      for (int lb = 0; lb < leBatches; ++lb) {
         int lec = Math::Decode_UInt1LE(read.substr(next, 1));
         ++next;
         std::vector<LedgerEntry> leBatches;
         if (lec < 0 || lec > ProtocolConstants::MAX_LEDGER_ENTRIES) {
            spdlog::error(
               "AbstractTxReceipt::{} Receipt batch contains {}  ledger entries, allowed are 1 to {}",
               __FUNCTION__,
               lec,
               ProtocolConstants::MAX_LEDGER_ENTRIES);
            throw TxReceiptDecodeException(
               "Receipt batch contains" + std::to_string(lec) +
               " ledger entries, allowed are 1 to " +
               std::to_string(ProtocolConstants::MAX_LEDGER_ENTRIES));
         }

         for (int len = 0; len < lec; ++len) {
            std::string hash160 = read.substr(next, 20);
            next += 20;
            auto flag = p->txFlags()->ledgerFlags().get(Math::Decode_UInt2LE(read.substr(next, 2)));
            next += 2;
            int amount = Math::Decode_UInt8LE(read.substr(next, 8));
            next += 8;
            std::string asset = Utils::trim(read.substr(next, 8));
            next += 8;
            if (asset.empty()) {
               asset = "";
            }
            if (!(read[next] == '\0' || read[next] == '\1')) {
               spdlog::error(
                  "AbstractTxReceipt::{} Invalid ledger entry #  {} status byte",
                  __FUNCTION__,
                  len + 1);
               throw TxReceiptDecodeException(
                  "Invalid ledger entry # " + std::to_string(len + 1) + " status byte");
            }
            LedgerEntry decodedLE(p, receipt, flag, hash160, amount, asset);
            if (read[next] == '\1') {
               decodedLE.markApplicable();
            }
            next += 1;
            leBatches.push_back(decodedLE);
         }
         receipt->registerLedgerEntriesBatch(leBatches);

         // Validations

         if (receipt->totalOut() != totalOut) {
            spdlog::error(
               "AbstractTxReceipt::{} Receipt total in {} does not match actual total in {}",
               __FUNCTION__,
               receipt->totalIn(),
               totalIn);
            throw TxReceiptDecodeException(
               "Receipt total in " + std::to_string(receipt->totalIn()) +
               " does not match actual total in " + std::to_string(totalIn));
         }

         if (receipt->totalIn() != totalIn) {
            spdlog::error(
               "AbstractTxReceipt::{} Receipt total out {} does not match actual total out {}",
               __FUNCTION__,
               receipt->totalOut(),
               totalOut);
            throw TxReceiptDecodeException(
               "Receipt total out " + std::to_string(receipt->totalOut()) +
               " does not match actual total out " + std::to_string(totalOut));
         }

         if (receipt->totalFee() != totalFee) {
            spdlog::error(
               "AbstractTxReceipt::{} Receipt total fees {} does not match actual total fees {}",
               __FUNCTION__,
               receipt->totalFee(),
               totalFee);
            throw TxReceiptDecodeException(
               "Receipt total fees " + std::to_string(receipt->totalFee()) +
               " does not match actual total fees " + std::to_string(totalFee));
         }
         return receipt;
      }
   }
}
Buffer::Binary Protocol::AbstractTxReceipt::halfSerialize()
{
   if (m_status == -1) {
      spdlog::error("AbstractTxReceipt::{} TxReceipt status not set",  __FUNCTION__);
      throw TxReceiptDecodeException("TxReceipt status not set");
   }
   Buffer::Binary ser("");
   ser.append(getTx()->hash().raw());
   ser.append(Math::Encode_UInt2LE(m_status));

   if (m_data.sizeInBytes() > 0xff) {
      spdlog::error(
         "AbstractTxReceipt::{} TxReceipt data cannot exceed 255 bytes",  __FUNCTION__);
      throw TxReceiptDecodeException("TxReceipt data cannot exceed 255 bytes");
   }
   ser.append(Math::Encode_UInt1LE(m_data.sizeInBytes()));
   ser.append(m_data.value());

   ser.append(Math::Encode_UInt8LE(m_totalIn));
   ser.append(Math::Encode_UInt8LE(m_totalOut));
   ser.append(Math::Encode_UInt8LE(m_totalFee));

   return ser;
}
Protocol::AbstractTxReceipt::AbstractTxReceipt(
   Protocol::AbstractProtocolChain* p, Protocol::AbstractPreparedTx* tx, int blockHeightContext)
   : m_data(""), m_p{p}, m_blockHeightContext{blockHeightContext}, m_tx{tx}
{
   m_status = -1;
   // TODO::CRITICAL -> When ever this class will be called these method will be called in the
   // constructor of base class Commenting for now
   //    defaultLedgerEntries();
   //    generateLedgerEntries();
   //    calculateLedgerEntries();
}
void Protocol::AbstractTxReceipt::calculateLedgerEntries(bool applicableOnly)
{
   m_totalOut = 0;
   m_totalFee = 0;
   m_totalIn = 0;

   auto batches = m_ledgerEntries.batches();
   if (!batches.empty()) {
      for (auto& batch : batches) {
         for (auto& ledgerEntry : batch) {
            if (!ledgerEntry.asset().empty()) {
               continue;
            }
            if (applicableOnly && !ledgerEntry.isApplicable()) {
               continue;
            }
            if (ledgerEntry.flag().isCredit()) {
               m_totalOut = ledgerEntry.amount();
            } else {
               m_totalIn = ledgerEntry.amount();
            }
         }
      }
   }
   calculateTotalFee();
}
std::tuple<int, std::string, std::tuple<int, int, std::vector<std::vector<Protocol::LedgerEntries>>>, int, int, int>
   Protocol::AbstractTxReceipt::dump()
{
   return std::make_tuple(
      m_status, m_data.raw(), m_ledgerEntries.dump(), m_totalIn, m_totalOut, m_totalFee);
}
bool Protocol::AbstractTxReceipt::isFinalised()
{
   return m_status && m_status >= 0;
}
Protocol::AbstractPreparedTx* Protocol::AbstractTxReceipt::getTx()
{
   return m_tx;
}
Buffer::Binary Protocol::AbstractTxReceipt::data()
{
   return m_data;
}
void Protocol::AbstractTxReceipt::setStatus(int code)
{
   m_status = code;
}
int Protocol::AbstractTxReceipt::totalIn()
{
   return m_totalIn;
}
int Protocol::AbstractTxReceipt::totalOut()
{
   return m_totalOut;
}
int Protocol::AbstractTxReceipt::totalFee()
{
   return m_totalFee;
}
int Protocol::AbstractTxReceipt::status()
{
   return m_status;
}
Protocol::LedgerEntries Protocol::AbstractTxReceipt::ledgerEntries()
{
   return m_ledgerEntries;
}
Buffer::Binary Protocol::AbstractTxReceipt::ledgerEntriesHash()
{
   auto serBatch = m_ledgerEntries.serializeBatches();
   Buffer::Binary binary((std::string(getTx()->hash().raw() + serBatch)));
   return m_p->hash256(binary);
}
Buffer::Binary Protocol::AbstractTxReceipt::getReceiptHash()
{
   Buffer::Binary raw(Buffer::Binary(halfSerialize().value() + ledgerEntriesHash().value()));
   return m_p->hash256(raw);
}
Buffer::Binary Protocol::AbstractTxReceipt::serialize()
{
   auto ser = halfSerialize();
   ser.append(m_ledgerEntries.serializeBatches());
   return ser;
}
Protocol::LedgerEntry Protocol::AbstractTxReceipt::createLedgerEntry(
   Protocol::LedgerFlag lf, std::string hash160, int amount, std::string assetId)
{
   return LedgerEntry(m_p, this, lf, hash160, amount, assetId);
}
void Protocol::AbstractTxReceipt::registerLedgerEntriesBatch(std::vector<LedgerEntry>& entries)
{
   m_ledgerEntries.addBatch(entries);
}
