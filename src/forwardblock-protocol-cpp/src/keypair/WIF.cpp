//
// Created by pc on 26/5/21.
//

#include "WIF.h"
Base58::Base58Encoded  WIF::Encode(int networkPrefix, std::string privateKey, bool isCompresed)
{
   if(!isHex(privateKey)){
      throw std::runtime_error("Private key must be a hexadecimal string");
   }
   // If the private key will correspond to a compressed public key...
   if (isCompresed) {
      privateKey += "01"; // append 0x01 byte
   }

   std::string raw = base16Encode(std::to_string(networkPrefix)) + privateKey;
   return Base58::Base58Check().encode(raw);
}
Buffer::Base16 WIF::Decode(int networkPrefixMatch, std::string wifdata, bool isCompressed)
{
       if (wifdata.empty()) {
          throw std::runtime_error("First argument must be a string");
       }
       auto wif =  Base58::Base58Encoded(wifdata);
   //Decode
   Buffer::Binary privateKey;// = Base58check()->decode(wif)->binary();
   if(isCompressed)
      privateKey.getBufferData().substr(0, -1);
   // Network prefix check
   if (networkPrefixMatch) {
      std::string networkpefixHex(base16Encode(std::to_string(networkPrefixMatch)));
      int networkPrefixByteLen = (base16Decode(networkpefixHex)).length();

      privateKey.setBufferData(privateKey.getBufferData().substr(0, networkPrefixByteLen));
      if (Buffer::Binary(privateKey.getBufferData()).base16().hexits() != networkpefixHex)
         throw std::runtime_error("Network prefix mismatch");
      privateKey.set(privateKey.getBufferData().substr(networkPrefixByteLen));
   }
   return privateKey.base16();
}
