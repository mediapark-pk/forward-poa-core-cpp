//
// Created by umar on 5/20/21.
//

#ifndef FORWARDBLOCK_LEDGERFLAGS_H
#define FORWARDBLOCK_LEDGERFLAGS_H

#include "ledgerflag.h"
#include "spdlog/spdlog.h"

#include <map>

namespace Protocol
{
class LedgerFlags
{
    std::map<const int,LedgerFlag> m_flags;
public:
    int count() const;
    void append(int dec, bool isCredit);
    bool has(int dec);
    LedgerFlag get(int dec);
};
}

#endif    // FORWARDBLOCK_LEDGERFLAGS_H
