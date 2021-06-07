#include "3rdparty/secp256k1-cxx/secp256k1-cxx.hpp"
#include "utils/utils.h"
#include "validator.h"
#include <iostream>
#include "crypto/bip32/bip32.h"

int main()
{
   std::string str = "abc";
   const uint8_t* ptr = reinterpret_cast<const uint8_t*>(str.c_str());
   Secp256K1::getInstance()->createPrivateKey();
   std::vector<uint8_t> prvKey = Secp256K1::getInstance()->privateKey();
   std::cout << "SECP PrivateKey "<< base16Encode(std::string{prvKey.begin(), prvKey.end()})<< std::endl;
   Secp256K1::getInstance()->createPublicKeyFromPriv(Secp256K1::getInstance()->privateKey(), false);
   std::vector<uint8_t> PubKey = Secp256K1::getInstance()->publicKey();
//   std::cout << "SECP PublicKey "<< base16Encode(std::string{PubKey.begin(), PubKey.end()})<< std::endl;
//   std::vector<uint8_t> PubKeyComp = Secp256K1::getInstance()->publicKey();
   std::cout << "SECP PublicKey "<< base16Encode(std::string{PubKey.begin(), PubKey.end()})<< std::endl;
   std::vector<uint8_t> T1 = Secp256K1::getInstance()->Sign(Secp256K1::getInstance()->privateKey(), ptr);
   std::vector<uint8_t> T2 = Secp256K1::getInstance()->recoverableSignature(Secp256K1::getInstance()->privateKey(), ptr);
   std::vector<uint8_t> recPubkey= Secp256K1::getInstance()->recoverPublicKeyFromSignature(T2, ptr);
   if(Secp256K1::getInstance()->Verify(ptr, T2, PubKey))
      std::cout<<"Verified";
   else
      std::cout<<"not verified";

   std::cout << "\nHello, World!" << std::endl;
   Protocol::Validator obj;
   std::string var = "ACCOUNT_STATUS";
   std::cout << obj.isValidTxFlagName(var) << std::endl;
   var = "abcdef13514124351541601124045454854856ABCDEFabcdef56ABCDEFabcdea";
   std::cout << obj.isValidChainID(var) << std::endl;
   var = "AY4b-BG4";
   std::cout << obj.isValidAssetID(var) << std::endl;
   std::cout << obj.isValidEpoch(5) << std::endl;
   var = "Acd124 -_@%:;(7867)[]af943";
   std::cout << obj.validatedMemo(var) << std::endl;
   var = "ab2245";
   std::cout << obj.isBase16Int(var, 3) << std::endl;
}