#ifndef _PROTOCOL_CONSTANTS
#define _PROTOCOL_CONSTANTS
#include <string>

namespace Protocol
{
class ProtocolConstants
{
   public:
   /** @var int buffer of 512 from PHP_INT_MIN */
   static constexpr long long MIN = INT64_MIN + 512;
   /** @var int buffer of 512 from PHP_INT_MAX */
   static constexpr long long MAX = INT64_MAX - 512;
   /** @var string implemented protocol version */
   static constexpr char* VERSION = "0.0.10";
   /** @var int (Major * 10000 + Minor * 100 + Release) */
   static constexpr int VERSION_ID = 10;
   /** Number of digits after decimal; Warning: Uint are always 64bit */
   static constexpr unsigned int SCALE = 8;
   /** Maximum size of a serialized transactions */
   static constexpr unsigned int MAX_TRANSACTION_SIZE = 0xffff;
   /** Maximum arbitrary data storable in a transactions */
   static constexpr unsigned int MAX_ARBITRARY_DATA = 61440;    // 60 KiB
   /** Maximum size of a block */
   static constexpr unsigned int MAX_BLOCK_SIZE = 0x0F4240;
   /** @var int Maximum length of transactions memo */
   static constexpr unsigned int MAX_TX_MEMO_LEN = 32;
   /** @var int Maximum transfers per transactions to a recipient */
   static constexpr unsigned int MAX_TRANSFERS_PER_TX = 10;
   /** @var int Maximum number of LedgerEntry objects in a transactions */
   static constexpr unsigned int MAX_LEDGER_ENTRIES = 30;
   /** @var int ECC curve Secp256k1 */
   static constexpr unsigned int ECDSA_CURVE = 8;
   /** @var int Genesis transactions flag */
   static constexpr unsigned int GENESIS_TX_FLAG = 0x01;
};
}    // namespace Protocol


#endif
