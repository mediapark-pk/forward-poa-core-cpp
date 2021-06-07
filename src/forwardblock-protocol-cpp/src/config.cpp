#include "config.h"

Protocol::Config::Config(const std::string& path)
{
    INIReader reader(path);
    if (reader.ParseError() != 0) {
       spdlog::error("Config::{} Unable to parse " + path + " file" ,__FUNCTION__ );
        throw ProtocolConfigException("Unable to parse " + path + " file");
    }
    m_chainId = reader.Get("", "chainId", "");
    if (m_chainId.empty()) {
       spdlog::error("Config::{} Chain Id Empty" ,__FUNCTION__ );
        throw ProtocolConfigException("Chain Id Empty");
    }

   m_forkId = reader.GetInteger("forks","Id",0);
   if(m_forkId < 0 || m_forkId > 0xff)
   {
      spdlog::error("Config::{} Invalid fork id" ,__FUNCTION__ );
      throw ProtocolConfigException("Invalid fork id");
   }

    m_accountsPrefix = reader.GetInteger("accounts", "prefix", 0);

    if (m_accountsPrefix < 0x00 || m_accountsPrefix > 0xff) {
       spdlog::error("Config::{} Invalid accounts prefix" ,__FUNCTION__ );
        throw ProtocolConfigException("Invalid accounts prefix");
    }

    m_fancyPrefix = reader.Get("accounts", "fancyPrefix", "");

    if (std::regex_match(m_fancyPrefix, std::regex("^[a-zA-Z0-9]{0,4}$"))) {
       spdlog::error("Config::{} Invalid accounts fancy prefix" ,__FUNCTION__ );
        throw ProtocolConfigException("Invalid accounts fancy prefix");
    }


    m_fancyPrefixLen = m_fancyPrefix.length();

    m_wifPrvPrefix = reader.GetInteger("wif", "private", 0);
    if (m_wifPrvPrefix < 0 || m_wifPrvPrefix > 0xffffffff) {
       spdlog::error("Config::{} " ,__FUNCTION__ );
        throw ProtocolConfigException("Invalid private key WIF prefix");
    }

    m_wifPubPrefix = reader.GetInteger("wif", "public", 0);
    if (m_wifPubPrefix < 0 || m_wifPubPrefix > 0xffffffff) {
       spdlog::error("Config::{} Invalid public key WIF prefix" ,__FUNCTION__ );
        throw ProtocolConfigException("Invalid public key WIF prefix");
    }
}

const std::string &Protocol::Config::getMChainId() const {
    return m_chainId;
}

int Protocol::Config::getMAccountsPrefix() const {
    return m_accountsPrefix;
}

const std::string &Protocol::Config::getMFancyPrefix() const {
    return m_fancyPrefix;
}

int Protocol::Config::getMFancyPrefixLen() const {
    return m_fancyPrefixLen;
}

int Protocol::Config::getMWifPubPrefix() const {
    return m_wifPubPrefix;
}

int Protocol::Config::getMWifPrvPrefix() const {
    return m_wifPrvPrefix;
}

int Protocol::Config::getMForkId() const
{
   return m_forkId;
}