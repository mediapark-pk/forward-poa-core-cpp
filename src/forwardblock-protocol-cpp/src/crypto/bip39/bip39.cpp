//
// Created by pc on 22/1/21.
//
#include "bip39.h"
#include "iostream"

BIP39::BIP39(int wordCount)
{
   if (wordCount < 12 || wordCount > 24) {
      throw MnemonicException("Mnemonic words count must be between 12-24");
   } else if (wordCount % 3 != 0) {
      throw MnemonicException("Words count must be generated in multiples of 3");
   }
   // Actual words count
   m_wordsCount = wordCount;
   // Overall entropy bits (ENT+CS)
   m_overallBits = m_wordsCount * 11;
   // Checksum Bits are 1 bit per 3 words, starting from 12 words with 4 CS bits
   m_checksumBits = ((m_wordsCount - 12) / 3) + 4;
   // Entropy Bits (ENT)
   m_entropyBits = m_overallBits - m_checksumBits;

   m_wordList = new Wordlist("english");
}

bool BIP39::validateEntropy(const std::string& entropy)
{
   if (!isHex(entropy)) {
      return false;
   }

   auto entropyBits = entropy.length() * 4;
   switch (entropyBits) {
   case 128:
   case 160:
   case 192:
   case 224:
   case 256:
      return true;
   default:
      return false;
   }
}
constexpr size_t BIP39::len_to_mask(size_t len) noexcept
{
   switch (len) {
   case 128:
      return 0xf0;
   case 160:
      return 0xf8;
   case 192:
      return 0xfc;
   case 224:
      return 0xfe;
   case 256:
      return 0xff;
   default:
      return 0;
   }
}

std::string BIP39::checksum(const std::string& entropy)
{
   std::vector<uint8_t> out(SHA256_RAW_BYTES_LENGTH);
   auto entrop = base16Decode(entropy);
   sha256(reinterpret_cast<const uint8_t*>(entrop.c_str()), entrop.size(), &out[0]);

   auto checksumChar = out.at(0);
   auto mask = len_to_mask(m_entropyBits);
   if (mask == 0xf0)
      return std::bitset<4>((checksumChar & mask) >> 4).to_string();
   else if (mask == 0xf8)
      return std::bitset<5>((checksumChar & mask) >> 3).to_string();
   else if (mask == 0xfc)
      return std::bitset<6>((checksumChar & mask) >> 2).to_string();
   else if (mask == 0xfe)
      return std::bitset<7>((checksumChar & mask) >> 1).to_string();
   else if (mask == 0xff)
      return std::bitset<8>((checksumChar & mask)).to_string();
   return "";
}

Mnemonic BIP39::Generate(int wordCount)
{
   return generateSecureEntropy().wordList(Wordlist::english()).mnemonic();
}

BIP39 BIP39::generateSecureEntropy()
{
   time_t t;
   srand((unsigned) time(&t));
   size_t size = m_entropyBits / 8;

   std::string bytes="";
   char hex_characters[]={'a', 'b', 'c', 'd', 'e', 'f', 'g',
                          'h', 'i', 'j', 'k', 'l', 'm', 'n',
                          'o', 'p', 'q', 'r', 's', 't', 'u',
                          'v', 'w', 'x', 'y', 'z','0','1','2',
                          '3','4','5','6','7','8','9'};

   for(int i=0;i<size;i++)
   {
      bytes=bytes+hex_characters[rand()%36];
   }
   std::cout << bytes.length() << "\n";

   auto hex_rand = base16Encode(bytes);
   useEntropy(hex_rand);
   return *this;
}

Mnemonic BIP39::GenerateSeedFromEntropy(const std::string& entropy)
{
   if (!BIP39::validateEntropy(entropy)) {
      throw MnemonicException("Invalid Entropy: " + entropy);
   }

   auto entropyBits = entropy.length() * 4;
   auto checksumBits = ((entropyBits - 128) / 32) + 4;
   auto wordsCount = (entropyBits + checksumBits) / 11;
   try {
      return BIP39(wordsCount).useEntropy(entropy).wordList(Wordlist::english()).mnemonic();
   } catch (...) {
      throw;
   }
}

Mnemonic BIP39::words(const std::string& words, Wordlist* wordlist, bool verifyChecksum)
{
   if (wordlist == nullptr)
      throw MnemonicException("Invalid wordlist");
   std::istringstream w{words};
   std::string word;
   std::vector<std::string> spWords;
   spWords.reserve(m_wordsCount);
   while (std::getline(w, word, ' ')) {
      spWords.emplace_back(word);
   }
   auto wordCount = spWords.size();
   try {
      return BIP39(wordCount).wordList(wordlist).reverse(spWords, verifyChecksum);
   } catch (const MnemonicException& e) {
      throw e;
   }
}

BIP39 BIP39::useEntropy(const std::string& entropy)
{
   if (!BIP39::validateEntropy(entropy)) {
      throw MnemonicException("Invalid Entropy: " + entropy);
   }
   m_entropy = entropy;
   std::cout << "entropy: " << entropy << "\n";
   m_checksum = checksum(entropy);
   std::cout << "checksum: " << m_checksum << "\n";
   auto str = Utils::hex2bits(m_entropy) + m_checksum;
   std::cout << "final string : " << str << "\n";
   m_rawBinaryChunks.clear();
   for (size_t i = 0; i < str.length(); i += 11) {
      const std::string& word11 = str.substr(i, 11);
      m_rawBinaryChunks.emplace_back(word11);
   }
   return *this;
}

Mnemonic BIP39::mnemonic()
{
   if (m_entropy.empty()) {
      throw MnemonicException("Entropy is empty");
   }
   if (m_wordList->empty()) {
      throw MnemonicException("Wordlist is empty");
   }

   auto _mnemonic = Mnemonic();
   _mnemonic.entropy = m_entropy;
   for (const auto& bit : m_rawBinaryChunks) {
      auto index = bit.to_ulong();
      _mnemonic.wordsIndex.emplace_back(index);
      _mnemonic.words.emplace_back(m_wordList->getWord(index));
      _mnemonic.rawBinaryChunks.emplace_back(bit);
      ++_mnemonic.m_wordsCount;
   }
   return _mnemonic;
}

BIP39 BIP39::wordList(Wordlist* wordlist)
{
   if (wordlist == nullptr)
      throw MnemonicException("Invalid wordlist - wordList()");
   m_wordList = wordlist;
   return *this;
}

Mnemonic BIP39::reverse(const std::vector<std::string>& words, bool verifyChecksum)
{
   if (m_wordList->empty()) {
      throw MnemonicException("Wordlist is empty");
   }

   auto mnemonic = Mnemonic();
   size_t size = words.size();
   mnemonic.words.reserve(size);
   mnemonic.wordsIndex.reserve(size);
   mnemonic.rawBinaryChunks.reserve(size);

   std::stringstream ss;
   for (const auto& word : words) {
      auto index = m_wordList->findIndex(word);
      if (index < 0) {
         return mnemonic;
      }
      std::bitset<11> b(index);

      mnemonic.words.emplace_back(word);
      mnemonic.wordsIndex.emplace_back(index);
      mnemonic.rawBinaryChunks.emplace_back(b);
      ++mnemonic.m_wordsCount;
   }

   std::string rawBinary;
   rawBinary.reserve(mnemonic.rawBinaryChunks.size() * 11);
   for (const auto bit : mnemonic.rawBinaryChunks) {
      rawBinary += bit.to_string();
   }

   const auto& entropyBits = rawBinary.substr(0, m_entropyBits);
   const auto& checksumBits = rawBinary.substr(m_entropyBits, m_checksumBits);

   mnemonic.entropy = Utils::bits2hex(entropyBits);

   // Verify Checksum?
   if (verifyChecksum) {
      auto ch = checksum(mnemonic.entropy);
      if (!Utils::hashEquals(checksumBits, checksum(mnemonic.entropy))) {
         throw MnemonicException("Entropy checksum match failed!");
      }
   }

   return mnemonic;
}

std::vector<uint8_t> BIP39::seed(
   SHA algo, const std::string& pass, const std::string& salt, uint32_t iterations, size_t outKeySize)
{
   std::vector<uint8_t> outKey(outKeySize);
   outKey = hashPbkdf2(algo, pass, salt, iterations, outKeySize);
   return outKey;
}

std::string BIP39::getSeed(const std::string entropy, const std::string wordlist)
{
   std::cout << entropy.length() << "\n";
   useEntropy(entropy);
   Mnemonic m_obj;
   m_obj = mnemonic();
   std::string pass;
   for (auto itr : m_obj.words) {
      pass += " ";
      pass += itr.data();
   }

   std::cout << "mnemonic phrases:   " << pass << "\n";

   // addphrase value might change
   std::string addphrase = "MediaPark";
   std::string salt = "mnemonics" + addphrase;

   uint32_t itrCount = 2048;
   size_t outKeySize = 64;
   SHA algo = SHA::SHA512;

   std::vector<uint8_t> outKey(outKeySize);

   outKey = seed(algo, pass, salt, itrCount, outKeySize);

   pass = "";

   for (auto itr : outKey) {
      pass += itr;
   }

   std::string seed= base16Encode({outKey.begin(),outKey.end()});
   std::cout << "raw hash :"<< pass;
   std::cout << "\n bin2hex :"<< base16Encode(pass);
   std::cout<<"\nhexadecimal: "<<seed;
   return seed;
}
const std::string& BIP39::getEntropy()
{
   return m_entropy;
}
