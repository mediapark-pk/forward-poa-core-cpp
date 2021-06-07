#ifndef BIP39_H
#define BIP39_H

#include "../../utils/strencodings.h"
#include "../../utils/utils.h"
#include "../pbkdf2.h"
#include "../sha256.h"
#include "mnemonic.h"
#include "wordlist.h"
#include <bitset>
#include <string>
#include <vector>
#ifdef _WIN32
#   include <Windows.h>
#   include <bcrypt.h>
#else
#   include <sys/random.h>
#endif

class MnemonicException : public std::runtime_error
{
   public:
   MnemonicException(std::string&& msg) : std::runtime_error{std::move(msg)} {}

   const char* what() const noexcept
   {
      return std::runtime_error::what();
   }
};


class BIP39
{
   public:
   BIP39(int wordcount = 12);

   /** Checks whether the entropy is of valid length or not
    * @param entropy A hexadecimal string of entropy
    * */
   bool validateEntropy(const std::string& entropy);

   /** Calculates entropyBits, checksumBits & wordsCount from entropy
    * @param entropy A hexadecimal string of entropy
    * */
   Mnemonic GenerateSeedFromEntropy(const std::string& entropy);

   /** Calculates checksum from entropy
    *  * @param entropy A hexadecimal string of entropy
    *  */
   std::string checksum(const std::string& entropy);

   /** Calculates mask against entropyBits
    * @param len Length of entropyBits
    * @return Mask as size_t
    * */
   static constexpr size_t len_to_mask(size_t len) noexcept;

   /** Generates random entropy */
   BIP39 generateSecureEntropy();

   /** Generates mnemonics from random entropy
    * @param wordCount Number of words for seed
    * @return Mnemonics
    * */
   Mnemonic Generate(int wordCount = 12);

   /** Gets wordlist according to given parameter
    * @param wordlist name of language file
    * @return BIP39 object
    * */
   BIP39 wordList(Wordlist* wordlist);

   /**Generates mnemonics against entropy
    * @return mnemonics
    * */
   Mnemonic mnemonic();

   /**
    * Calculates raw binary chunks to find mnemonics(words)
    * from binary string with checksum
    * @param entropy A hexadecimal string of entropy
    * */
   BIP39 useEntropy(const std::string& entropy);

   /**
    * Calculates mnemonic phrases
    * @param words string of words
    * @param wordlist language to use , english is not provided
    * @param verifyChecksum true if not set
    * */
   Mnemonic words(
      const std::string& words, Wordlist* wordlist = Wordlist::english(), bool verifyChecksum = true);

   /**Calculates the entropy from words
    * @param words vector of words
    * @param verifyChecksum true if not set
    * */
   Mnemonic reverse(const std::vector<std::string>& words, bool verifyChecksum = true);

   /** calculates the raw seed value from hashPbkdf2
    * @param algo SHA512 encryption algorithm to generate seed
    * @param pass list of words as string
    * @param salt salt string
    * @param iterations iteration count
    * @return rturns raw hashes as vector
    * */
   std::vector<uint8_t> seed(
      SHA algo, const std::string& pass, const std::string& salt, uint32_t iterations, size_t outKeySize);

   /** calculates the raw seed value as hexadecimal hashes
    * @param entropy Hexadecimal string of entropy
    * @param wordlist Wordlist is a list of words as a string
    * @return seed as a string
    * */
   std::string getSeed(const std::string entropy, const std::string wordlist);
   const std::string& getEntropy();

   private:
   int m_wordsCount;
   int m_overallBits;
   int m_checksumBits;
   int m_entropyBits;
   std::string m_entropy;
   std::string m_checksum;
   std::vector<std::bitset<11>> m_rawBinaryChunks;
   std::vector<std::string> m_words;
   Wordlist* m_wordList;
};


#endif    // BIP39_H
