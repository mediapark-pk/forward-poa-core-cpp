#ifndef ACCOUNT_PROTO_H
#define ACCOUNT_PROTO_H

#include "../abstractprotocolchain.h"
#include "chainaccountinterface.h"

namespace Protocol{

class AccountsProto
{
public:
    AccountsProto() = default;
    AccountsProto(AbstractProtocolChain* protocolChain);
    ~AccountsProto();

    int sigRequiredCount(const ChainAccountInterface& acc);
    /*int verifyAllSignatures(
        const ChainAccountInterface* acc,
        std::vector<unsigned char> msgHash,
        std::vector<Signature> signature);*/

private:
    AbstractProtocolChain* m_p;
};
}    // namespace Protocol
#endif
