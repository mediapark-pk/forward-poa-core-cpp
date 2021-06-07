#include "masterkey.h"

#include <stdexcept>

MasterKey::MasterKey(const std::vector<unsigned char>& seed, const std::vector<unsigned char>& hmacKey)
{
    size_t size = seed.size();
    switch (size) {
    case 16:
    case 32:
    case 64:
        break;
    default:
        throw std::runtime_error("Invalid seed size: " + std::to_string(size));
    }

    std::vector<unsigned char> key;
    key.assign(seed.begin(), seed.begin() + 32);
    std::vector<unsigned char> chaincode;
    chaincode.assign(seed.begin() + 32, seed.end());

    master = ExtendedKey(key, chaincode);
}

ExtendedKey* MasterKey::getMaster()
{
    return &master;
}
