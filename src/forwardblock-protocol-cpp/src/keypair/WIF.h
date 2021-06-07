//
// Created by pc on 26/5/21.
//

#ifndef FORWARDBLOCK_WIF_H
#define FORWARDBLOCK_WIF_H

#include "../data-types-v0.0.0/src/buffer/base16.h"
#include "../base58check.h"
#include "../base58/base58encoded.h"
#include "../utils/strencodings.h"

class WIF
{
   public:
   /**
    * @param int networkPrefix
    * @param string privateKey
    * @param bool isCompressed
    * @return Base58Encoded
    */
    static Base58::Base58Encoded Encode(int networkPrefix, const std::string privateKey, bool isCompresed=true);

    /**
     * @param int networkPrefixMatch
     * @param wif
     * @param bool isCompressed
     * @return Base16
     */
     static  Buffer::Base16 Decode(int networkPrefixMatch, std::string wif, bool isCompressed = true);

};


#endif    // FORWARDBLOCK_WIF_H
