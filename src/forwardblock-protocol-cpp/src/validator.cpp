#include "validator.h"

bool Protocol::Validator::isValidTxFlagName(const std::string &name) {
    return std::regex_match(name, std::regex("^[a-zA-Z][a-zA-Z0-9]+(_[a-zA-Z0-9]+)*$"));
}
//Chain id size should be 64
bool Protocol::Validator::isValidChainID(std::string &chainID) {
    return std::regex_match(chainID, std::regex("^[a-fA-F0-9]{64}$"));
}

/*
bool Protocol::Validator::isValidMemo(const char *memo) {
    if (std::regex_match(memo, std::regex("^[a-z0-9\\s\\-_.@]{1,16}$"))) {
        return true;
    }
    return false;
}
*/

bool Protocol::Validator::isValidAssetID(std::string &assetID) {
    return std::regex_match(assetID, std::regex("^[a-zA-Z][a-zA-Z0-9]{1,3}-[a-zA-Z]{2}[0-9]$"));
}


bool Protocol::Validator::isValidEpoch(unsigned int timestamp) {
    if (timestamp > 0xFFFFFFFF)
        return false;
    return true;
}

std::string Protocol::Validator::validatedMemo(std::string &memo) {
    memo = Utils::trim(memo);
    if (memo.empty()) {
        /** Empty memos are valid **/
        return memo;
    }

    int memoLen = memo.size();
    if (memoLen > Protocol::ProtocolConstants::MAX_TX_MEMO_LEN) {
       spdlog::error("Validator::{} Memo cannot exceed length more than MAX_BYTES" ,__FUNCTION__ );
        throw std::length_error("Memo cannot exceed length more than MAX_BYTES");
    }
    if (!std::regex_match(memo, std::regex("^[a-zA-Z0-9\\s\\-_.@%:;()\\[\\]\"\']+$"))) {
       spdlog::error("Validator::{} Memo contains an illegal character or invalid format" ,__FUNCTION__ );
        throw std::domain_error("Memo contains an illegal character or invalid format");
    }
    return memo;
}

void Protocol::Validator::checkPublicKey(std::string &pubKey, bool compressed) {
    std::string pubKeyStr = "";
    if (isHex(pubKey)) {
        pubKeyStr = hexToBin(pubKey);
    } else if (isBinary(pubKey)) {
        pubKeyStr = pubKey;
    } else {
        pubKeyStr = pubKey;
    }

    if (pubKeyStr == "") {
       spdlog::error("Validator::{} Invalid public key arg type" ,__FUNCTION__ );
        throw std::runtime_error("Invalid public key arg type");
    }

    std::vector<std::string> prefixes = {"\x02", "\x03", "\x04"};
    std::vector<int> length = {33, 65};
    std::string type = "";

    if (compressed) {
        prefixes.emplace_back("\x02");
        prefixes.emplace_back("\x03");
        type = "compressed";
    } else {
        prefixes.emplace_back("\x04");
        type = "uncompressed";
    }

    int pubKeyLen = pubKeyStr.length();
//    if (!in_array(pubKeyLen, length)) {
//        throw std::runtime_error("Invalid public key length");
//    }
//    if (!in_array(pubKeyStr[0], prefixes)) {
//        throw std::runtime_error("Invalid %spublic key prefix");
//    }
}

bool Protocol::Validator::isBase16Int(std::string &str, int size = 1) {
    return std::regex_match(str, std::regex("^[a-f0-9]{" + std::to_string(size * 2) + "}$"));
}
