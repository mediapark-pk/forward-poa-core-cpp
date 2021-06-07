#ifndef _ABSTRACT_TX_H
#define _ABSTRACT_TX_H


#include "../data-types-v0.0.0/src/buffer/binary.h"
#include "../validator.h"
#include "../abstractprotocolchain.h"
#include "buffer/binary.h"
#include "../exceptions/txencodeexception.h"
#include "../keypair/privatekey/signature.h"
#include "spdlog/spdlog.h"
#include "../utils/mathencodings.h"


#include <string>
#include <vector>


namespace Protocol
{
class AbstractTx
{
   protected:
   AbstractProtocolChain* m_p;
   int m_version;
   int m_flag;
   std::string m_sender;
   int m_nonce = 0;
   std::string m_recipient;
   std::string m_memo;
   std::map<std::string, int> m_transfers;
   Buffer::Binary m_data;
   std::vector<std::vector<unsigned char>> m_signatures = {};
   int m_timeStamp;
   uint64_t m_fee = 0;
   std::vector<unsigned char> m_hash;
   std::vector<Signature> m_signs;

   public:
   AbstractTx() = default;
   AbstractTx(AbstractProtocolChain* p);
   Buffer::Binary hashPreImage(std::string chainId = "", int forkId = 0);
   const Buffer::Binary serialize(bool includeSignatures);
   const Buffer::Binary serializeTransfers();
};

}    // namespace Protocol

#endif
