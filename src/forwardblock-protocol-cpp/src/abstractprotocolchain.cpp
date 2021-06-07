#include "abstractprotocolchain.h"
#include "accounts/accountproto.h"
#include "transactions/abstracttx.h"
#include "crypto/sha256.h"
#include "transactions/txflags.h"
#include "keypair/keypairfactory.h"

#include <regex>

Protocol::AbstractProtocolChain::~AbstractProtocolChain() {}

Protocol::AbstractProtocolChain::AbstractProtocolChain(const std::string& path)
   : m_config{std::make_shared<Config>(path)}
   , m_ap{std::make_shared<AccountsProto>(this)}
   , m_kpF{std::make_shared<KeyPairFactory>()}
   , m_txFlags{std::make_shared<TxFlags>()}
{
//   registerTxFlags(m_txFlags);
}

std::string Protocol::AbstractProtocolChain::chainName()
{
   // NOT TESTED
   return typeid(this).name();
}

std::shared_ptr<Protocol::TxFlags> Protocol::AbstractProtocolChain::txFlags()
{
   return m_txFlags;
}

Secp256K1* Protocol::AbstractProtocolChain::secp256K1()
{
   return Secp256K1::getInstance();
}

std::shared_ptr<Protocol::Config> Protocol::AbstractProtocolChain::config()
{
   return m_config;
}

std::shared_ptr<Protocol::KeyPairFactory> Protocol::AbstractProtocolChain::keyPair()
{
   return m_kpF;
}

std::shared_ptr<Protocol::AccountsProto> Protocol::AbstractProtocolChain::accounts()
{
   return m_ap;
}

Buffer::Binary Protocol::AbstractProtocolChain::hash256(Buffer::Binary& bin)
{
   return bin.hash().digest("sha256", 2);
}

std::string Protocol::AbstractProtocolChain::version()
{
   return ProtocolConstants::VERSION;
}
int Protocol::AbstractProtocolChain::versionId()
{
   std::smatch matches;
   std::string input = this->version();
   if (std::regex_match(input, matches, std::regex("[0-9]+"))) {
      int v1 = !matches[0].str().empty() ? std::stoi(matches[0].str()) : 0;
      int v2 = !matches[1].str().empty() ? std::stoi(matches[1].str()) : 0;
      int v3 = !matches[2].str().empty() ? std::stoi(matches[2].str()) : 0;
      m_versionId = v1 * 1000 + v2 * 100 + v3;
      return m_versionId;
   }
   return 0;
}
Buffer::Binary Protocol::AbstractProtocolChain::serializeSignatures(std::vector<Signature> signature)
{
   Buffer::Binary* ser;
   for (const Signature &sign:signature) {

   }
}
