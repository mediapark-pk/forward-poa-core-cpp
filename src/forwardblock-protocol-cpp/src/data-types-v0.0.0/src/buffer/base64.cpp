
#include "base64.h"

Buffer::Base64::Base64(std::string data) : AbstractBuffer(data) {

}

std::string Buffer::Base64::validatedDataTypeValue(const std::string& data) {
    if (!Validator::isBase64(data)) {
        throw Exception("Data doesnot match with Base64 type.");
    }
    std::string decoded = Utils::base64_decode(data);
    if (decoded == "false")
        throw Exception("Base64 decode failed");

    return data;
}

std::string Buffer::Base64::encoded() {
    return this->value();
}

Buffer::Binary Buffer::Base64::binary() {

    return Buffer::Binary (Utils::base64_decode(this->encoded()));
}