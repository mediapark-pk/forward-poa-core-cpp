//
// Created by umar on 5/20/21.
//

#ifndef FORWARDBLOCK_TXFLAGS_H
#define FORWARDBLOCK_TXFLAGS_H


#include "abstracttxflag.h"
#include "receipts/ledgerflags.h"
#include "spdlog/spdlog.h"
#include "../validator.h"
#include "../exceptions/txflagexception.h"

#include <unordered_map>

namespace Protocol {
    class AbstractProtocolChain;

    class AbstractTxFlag;

    class TxFlags {
        Protocol::AbstractProtocolChain *m_p;
        std::unordered_map<int, Protocol::AbstractTxFlag *> m_flags;
        std::unordered_map<std::string, Protocol::AbstractTxFlag *> m_namesMap;
        Protocol::LedgerFlags m_ledgerFlags;
        int m_count;

    public:
        TxFlags() = default;
        TxFlags(Protocol::AbstractProtocolChain *p);
        Protocol::LedgerFlags ledgerFlags();
        TxFlags append(Protocol::AbstractTxFlag *flag);
        bool hasName(const std::string &name);
        AbstractTxFlag* getFlag(int id);
        AbstractTxFlag *getWithName(const std::string &name);
    };
}    // namespace Protocol

#endif    // FORWARDBLOCK_TXFLAGS_H
