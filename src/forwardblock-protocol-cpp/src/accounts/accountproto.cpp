#include "accountproto.h"

namespace Protocol {

    AccountsProto::AccountsProto( AbstractProtocolChain* protocolChain):m_p{protocolChain}{

    }

    AccountsProto::~AccountsProto() {}

    int AccountsProto::sigRequiredCount(const ChainAccountInterface &acc) {
        return 0;
    }

}

