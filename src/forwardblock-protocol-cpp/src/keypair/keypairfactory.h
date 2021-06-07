#ifndef BLOCKCHAIN_KEYPAIRFACTORY_H
#define BLOCKCHAIN_KEYPAIRFACTORY_H

#include <vector>

#include "privatekey.h"
#include "../crypto/bip39/mnemonic.h"
#include "spdlog/spdlog.h"
#include "../abstractprotocolchain.h"
#include "../data-types-v0.0.0/src/buffer/base16.h"
#include "../validator.h"
#include "WIF.h"
namespace Protocol
{
class KeyPairFactory
{
public:
    KeyPairFactory() = default;
    KeyPairFactory(AbstractProtocolChain* Protocol);
     /** Generates public Key from Entropy
     * @param Base16 prv
     * @return PubKey
     */
    PublicKey publicKeyfromEntropy(Buffer::Base16 pub);

    /** Generates private Key from Entropy
     * @param Base16 prv
     * @return PrivateKey
     */
     static PrivateKey privateKeyFromEntropy(Buffer::Base16 priv);

      /** Generates private Key from Mnemonics
     * @param Mnemonic m
     * @param string|null seedPassphrase
     * @return PrivateKey
     */
    PrivateKey privateKeyFromMnemonic(const Mnemonic& m, const std::string seedPassphrase="");

    /** Generates private key from WIF
     * @param string wif
     * @param bool isCompressed
     * @return PrivateKey
     */
    PrivateKey privateKeyFromWIF(const std::string wif, bool isCompressed= true);

private:
    AbstractProtocolChain* m_protocol;
};
}    // namespace Protocol

#endif    // BLOCKCHAIN_KEYPAIRFACTORY_H
