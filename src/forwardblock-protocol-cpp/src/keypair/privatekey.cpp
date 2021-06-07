#include "privatekey.h"

Protocol::PrivateKey::PrivateKey(
   Protocol::AbstractProtocolChain* p, Buffer::Base16 entropy, ExtendedKey extendedKey)
   : m_Protocol{p}
   , _privateKey{entropy}
   , m_extendedKey{extendedKey}
   , _publicKey{Protocol::PublicKey(p, m_Pubkey, m_PrvtKey)}
{
   _privateKey.readOnly(true);
}


Protocol::PublicKey Protocol::PrivateKey::publicKey()
{
   if (m_Pubkey.empty()) {
      _publicKey = _publicKey.fromPrivateKey(m_Protocol, m_PrvtKey);
   }
   return _publicKey;
}
std::string Protocol::PrivateKey::_export()
{
   std::string data{m_PrvtKey.begin(), m_PrvtKey.end()};
   Buffer::Base16 PrvKey(data);
   return (WIF::Encode(m_Protocol->config()->getMWifPrvPrefix(), PrvKey.hexits(false), true)).value();
}
Buffer::Base16 Protocol::PrivateKey::nullPadded32(Buffer::Base16 value)
{
   std::string str = value.binary()->raw();
   int len = str.length();
   return Buffer::Base16(binToHex(std::string((32 - len), '\0') + str));
}
std::string Protocol::PrivateKey::tweakRandK(std::string bits, int itN)
{
   for (int i = 0; i < itN; i++)
      bits = tweakRandK(bits, 0);

   int pos = bits.find_first_of("0");
   bits.replace(pos, 1, "1");
   return bits;
}
Protocol::Signature Protocol::PrivateKey::signWithValidRecId(
   Buffer::Base16 msgHAsh, Buffer::Base16* randK, int maxA)
{
   for (int i = 0; i < maxA; i++) {
      if (randK && i > 0) {
         std::string tweakedBits = tweakRandK(Utils::hex2bits(randK->hexits(false)), i);
         randK = new Buffer::Base16(Utils::bits2hex(tweakedBits));
      }
      std::vector<uint8_t> data = Secp256K1::getInstance()->Sign(
         m_PrvtKey, reinterpret_cast<const uint8_t*>(msgHAsh.getBufferData().c_str()));
      std::string str{data.begin(), data.end()};
      Buffer::Base16 r(str.substr(0, 32));
      Buffer::Base16 s(str.substr(32, 32));
      int v = std::stoi((str.substr(64, 1)));

      return Signature(nullPadded32(r), nullPadded32(s), v);
   }
   throw std::runtime_error(
      "Failed to find signature with valid recovery Id in" + std::to_string(maxA) + " attempts");
}

Protocol::Signature Protocol::PrivateKey::sign(Buffer::Base16 msgHash)
{
   if (msgHash.getBufferData().length() != 64) {
      std::runtime_error("This method must be used to sign 32 bytes msg hash");
   }
   try {
      return this->signWithValidRecId(msgHash);
   } catch (...) {
      std::runtime_error("Failed to sign message");
   }
}
