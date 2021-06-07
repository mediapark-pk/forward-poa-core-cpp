#ifndef BLOCKCHAIN_PRIVATEKEY_H
#define BLOCKCHAIN_PRIVATEKEY_H

#include "../abstractprotocolchain.h"
#include "../crypto/bip32/extendedkey.h"
#include "../data-types-v0.0.0/src/buffer/base16.h"
#include "../3rdparty/secp256k1-cxx/secp256k1-cxx.hpp"
#include "WIF.h"
#include "privatekey/signature.h"
#include "publickey.h"
#include "tuple"
#include <vector>

// TODO incomplete class
namespace Protocol
{
class PrivateKey
{
   protected:
   Protocol::AbstractProtocolChain* m_Protocol;
   Buffer::Base16 _privateKey;
   ExtendedKey m_extendedKey;
   Protocol::PublicKey _publicKey;
   std::vector<unsigned char> m_Pubkey;
   std::vector<unsigned char> m_PrvtKey;

   public:
   PrivateKey() = default;
   PrivateKey(AbstractProtocolChain* p, Buffer::Base16 entropy, ExtendedKey extendedKey = {});
   PublicKey publicKey();
   std::string _export();
   Signature sign(Buffer::Base16 msgHash);
   Signature signWithValidRecId(Buffer::Base16 msgHAsh, Buffer::Base16* randK = nullptr, int maxA = 10);
   std::string tweakRandK(std::string bits, int itN = 0);
   Buffer::Base16 nullPadded32(Buffer::Base16 value);
};
}    // namespace Protocol
#endif    // BLOCKCHAIN_PRIVATEKEY_H
