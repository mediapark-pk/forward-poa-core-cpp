//
// Created by umar on 28/5/21.
//
#ifndef FORWARDBLOCK_TRANSACTIONCONSTRUCTOR_H
#define FORWARDBLOCK_TRANSACTIONCONSTRUCTOR_H

#include "../abstractprotocolchain.h"
#include "abstracttxconstructor.h"
#include "abstracttxflag.h"
#include "../exceptions/txconstructexception.h"
#include "../protocolconstants.h"
#include "spdlog/spdlog.h"

namespace Protocol
{
class TransactionConstructor : public AbstractTxConstructor
{
   public:
   TransactionConstructor(AbstractProtocolChain* p, int ver, AbstractTxFlag* flag, int epoch);
   void beforeSerialize() override;
   TransactionConstructor* sendToRecipient(PublicKey recipient);
   TransactionConstructor* setData(Buffer::Binary data);
   TransactionConstructor* addTransfer(int value, std::string assetId = "");
};
}    // namespace Protocol


#endif    // FORWARDBLOCK_TRANSACTIONCONSTRUCTOR_H
