#ifndef VALIDATOR_H
#define VALIDATOR_H
#include "spdlog/spdlog.h"

#include "protocolconstants.h"
#include "utils/strencodings.h"
#include "utils/utils.h"
#include <regex>

#include <string>

namespace Protocol
{
class Validator
{
public:
//    static bool isValidMemo(const char* memo);
    static bool isValidEpoch(unsigned int timestamp);
    static bool isValidTxFlagName(const std::string& name);
    static bool isValidChainID(std::string& chainId);
    static bool isValidAssetID(std::string& assetId);
    static std::string validatedMemo(std::string& memo);
    static void checkPublicKey(std::string& pubKey, bool compressed=false);
    static bool isBase16Int(std::string& str, int size);
};

}    // namespace Protocol
#endif    // VALIDATOR_H
