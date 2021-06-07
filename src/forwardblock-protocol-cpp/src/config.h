#ifndef BLOCKCHAIN_CONFIG_H
#define BLOCKCHAIN_CONFIG_H

#include <string>
#include <regex>

#include "exceptions/configexception.h"
#include "3rdparty/INIReader/INIReader.h"
#include "spdlog/spdlog.h"
namespace Protocol
{
class Config
{
    std::string m_chainId;
public:
    const std::string &getMChainId() const;

    int getMAccountsPrefix() const;

    const std::string &getMFancyPrefix() const;

    int getMFancyPrefixLen() const;

    int getMWifPubPrefix() const;

    int getMWifPrvPrefix() const;

    int getMForkId() const;


private:
    int m_accountsPrefix;
    std::string m_fancyPrefix;
    int m_fancyPrefixLen;
    int m_wifPubPrefix;
    int m_wifPrvPrefix;
    int m_forkId;


public:
    Config() = default;
    Config (const std::string &path);
};
};    // namespace Protocol

#endif    // BLOCKCHAIN_CONFIG_H
