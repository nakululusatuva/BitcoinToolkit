#ifndef _SCRIPT_
#define _SCRIPT_

#include "../common.h"
#include "../container/CStack.h"
#include "../container/CLinkedlist.h"

#define SCRIPT_ADD_OPCODE_FAILED                (void *)0x003000
#define SCRIPT_ADD_DATA_FAILED                  (void *)0x003001
#define SCRIPT_MULTISIG_M_BIGGER_N              (void *)0x003002
#define SCRIPT_BARE_MULTISIG_PUBKEYS_OVER_LIMIT (void *)0x003003
#define SCRIPT_HAS_NO_ELEMENTS                  (void *)0x003004
#define SCRIPT_REMAIN_BYTES_LESS_THAN_PUSH      (void *)0x003005
#define SCRIPT_ELEMENT_SIZE_OVERLIMIT           (void *)0x003006
#define SCRIPT_SIZE_TO_PUSH_NOT_EQUAL_EXPECTED  (void *)0x003007
#define SCRIPT_CONTAINED_INVALID_ELEMENT        (void *)0x003008

#define MAX_SCRIPT_ELEMENT_SIZE      520 // Maximum number of bytes pushable to the stack
#define MAX_OPS_PER_SCRIPT           201 // Maximum number of non-push operations per script
#define MAX_PUBKEYS_PER_BARE_MULTISIG 20 // Maximum number of public keys per multisig
#define MAX_SCRIPT_SIZE            10000 // Maximum script length in bytes
#define MAX_SCRIPT_STACK_SIZE       1000 // Maximum number of values on script interpreter stack

// Threshold for nLockTime: below this value it is interpreted as block number,
// otherwise as UNIX timestamp.
#define LOCKTIME_THRESHOLD 500000000; // Tue Nov  5 00:53:20 1985 UTC

typedef enum opcode
{
	// push value
	OP_0 = 0x00,
	OP_FALSE = OP_0,
	OP_PUSHDATA1 = 0x4c,
	OP_PUSHDATA2 = 0x4d,
	OP_PUSHDATA4 = 0x4e,
	OP_1NEGATE = 0x4f,
	OP_RESERVED = 0x50,
	OP_1 = 0x51,
	OP_TRUE=OP_1,
	OP_2 = 0x52,
	OP_3 = 0x53,
	OP_4 = 0x54,
	OP_5 = 0x55,
	OP_6 = 0x56,
	OP_7 = 0x57,
	OP_8 = 0x58,
	OP_9 = 0x59,
	OP_10 = 0x5a,
	OP_11 = 0x5b,
	OP_12 = 0x5c,
	OP_13 = 0x5d,
	OP_14 = 0x5e,
	OP_15 = 0x5f,
	OP_16 = 0x60,

	// control
	OP_NOP = 0x61,
	OP_VER = 0x62,
	OP_IF = 0x63,
	OP_NOTIF = 0x64,
	OP_VERIF = 0x65,
	OP_VERNOTIF = 0x66,
	OP_ELSE = 0x67,
	OP_ENDIF = 0x68,
	OP_VERIFY = 0x69,
	OP_RETURN = 0x6a,

	// stack ops
	OP_TOALTSTACK = 0x6b,
	OP_FROMALTSTACK = 0x6c,
	OP_2DROP = 0x6d,
	OP_2DUP = 0x6e,
	OP_3DUP = 0x6f,
	OP_2OVER = 0x70,
	OP_2ROT = 0x71,
	OP_2SWAP = 0x72,
	OP_IFDUP = 0x73,
	OP_DEPTH = 0x74,
	OP_DROP = 0x75,
	OP_DUP = 0x76,
	OP_NIP = 0x77,
	OP_OVER = 0x78,
	OP_PICK = 0x79,
	OP_ROLL = 0x7a,
	OP_ROT = 0x7b,
	OP_SWAP = 0x7c,
	OP_TUCK = 0x7d,

	// splice ops
	OP_CAT = 0x7e,
	OP_SUBSTR = 0x7f,
	OP_LEFT = 0x80,
	OP_RIGHT = 0x81,
	OP_SIZE = 0x82,

	// bit logic
	OP_INVERT = 0x83,
	OP_AND = 0x84,
	OP_OR = 0x85,
	OP_XOR = 0x86,
	OP_EQUAL = 0x87,
	OP_EQUALVERIFY = 0x88,
	OP_RESERVED1 = 0x89,
	OP_RESERVED2 = 0x8a,

	// numeric
	OP_1ADD = 0x8b,
	OP_1SUB = 0x8c,
	OP_2MUL = 0x8d,
	OP_2DIV = 0x8e,
	OP_NEGATE = 0x8f,
	OP_ABS = 0x90,
	OP_NOT = 0x91,
	OP_0NOTEQUAL = 0x92,

	OP_ADD = 0x93,
	OP_SUB = 0x94,
	OP_MUL = 0x95,
	OP_DIV = 0x96,
	OP_MOD = 0x97,
	OP_LSHIFT = 0x98,
	OP_RSHIFT = 0x99,

	OP_BOOLAND = 0x9a,
	OP_BOOLOR = 0x9b,
	OP_NUMEQUAL = 0x9c,
	OP_NUMEQUALVERIFY = 0x9d,
	OP_NUMNOTEQUAL = 0x9e,
	OP_LESSTHAN = 0x9f,
	OP_GREATERTHAN = 0xa0,
	OP_LESSTHANOREQUAL = 0xa1,
	OP_GREATERTHANOREQUAL = 0xa2,
	OP_MIN = 0xa3,
	OP_MAX = 0xa4,

	OP_WITHIN = 0xa5,

	// crypto
	OP_RIPEMD160 = 0xa6,
	OP_SHA1 = 0xa7,
	OP_SHA256 = 0xa8,
	OP_HASH160 = 0xa9,
	OP_HASH256 = 0xaa,
	OP_CODESEPARATOR = 0xab,
	OP_CHECKSIG = 0xac,
	OP_CHECKSIGVERIFY = 0xad,
	OP_CHECKMULTISIG = 0xae,
	OP_CHECKMULTISIGVERIFY = 0xaf,

	// expansion
	OP_NOP1 = 0xb0,
	OP_CHECKLOCKTIMEVERIFY = 0xb1,
	OP_NOP2 = OP_CHECKLOCKTIMEVERIFY,
	OP_CHECKSEQUENCEVERIFY = 0xb2,
	OP_NOP3 = OP_CHECKSEQUENCEVERIFY,
	OP_NOP4 = 0xb3,
	OP_NOP5 = 0xb4,
	OP_NOP6 = 0xb5,
	OP_NOP7 = 0xb6,
	OP_NOP8 = 0xb7,
	OP_NOP9 = 0xb8,
	OP_NOP10 = 0xb9,

	OP_PUBKEYHASH = 0xfd,
	OP_PUBKEY = 0xfe,
	OP_INVALIDOPCODE = 0xff,
} Opcode;
#define BYTE_IS_OPCODE(byte) ( (byte >= 0x00 && byte <= 0xb9) || (byte >= 0xfd && byte <= 0xff) ) ? true : false
#define BYTE_IS_NONAME_PUSHDATA(byte) (byte >= 0x01 && byte <= 0x4b) ? true : false
#define BYTE_IS_124_PUSHDATA(byte) (byte >= 0x4c && byte <= 0x4e) ? true : false
/* New an opcode object, value range: 0x00~0xFF, return NULL on error */
Opcode * new_Opcode(BYTE value);
/* Delete an opcode object that created by new_opcode() */
void delete_Opcode(Opcode *this);
/* Return the opcode name string */
const char * get_op_name(Opcode op);

/** Script could store a bitcoin script and include some relative functions **/
typedef struct Script Script;
struct Script
{
	CLinkedlist *script;

	bool (*add_opcode)(Script *, Opcode *);
	bool (*add_data)(Script *, BYTE *, size_t);
	bool (*execute)(Script *);
	uint8_t * (*to_string)(Script *, size_t *);
	BYTE * (*to_bytes)(Script *, size_t *);
	bool (*is_p2pkh)(Script *);
	bool (*is_p2pk)(Script *);
	bool (*is_p2sh)(Script *);
	bool (*is_p2sh_multisig)(Script *);
	bool (*is_empty)(Script *);
	uint32_t (*get_length)(Script *);
	void * (*get_element)(Script *, uint64_t, size_t *);
	size_t (*total_size)(Script *);
	uint32_t (*check_element_size)(Script *);
};

/** Construct and Destruct Functions **/
/** Create an empty Script object, return NULL on error(s)
*   \return error codes:
*           MEMORY_ALLOCATE_FAILED
*   \else on success.
**/
Script * new_Script();

/** Create a Script object from bytes.
*   \param  bytes       Byte array.
*   \param  size        How many bytes.
*   \return error codes:
*           MEMORY_ALLOCATE_FAILED
*   \else on success.
**/
Script * new_Script_from_bytes(BYTE *bytes, size_t size);

/** Create a Script object from two other Script.
*   \param  p1          Part 1, the first Script.
*   \param  p2          Part 2, the second Script.
*   \return error codes:
*           MEMORY_ALLOCATE_FAILED
*           SCRIPT_ADD_OPCODE_FAILED
*           SCRIPT_ADD_DATA_FAILED
*           SCRIPT_REMAIN_BYTES_LESS_THAN_PUSH
*   \else on success.
*   Notice that p1 and p2 are 'templates', still, need to be freed by delete_Script() manually.
**/
Script * new_Script_assembled(Script *p1, Script *p2);

/** Create a P2PKH Script.
*   \param  pubkey_hash  Byte array.
*   \param  size         How many bytes, must be 20 bytes.
*   \return error codes:
*           INVALID_PUBKEY_HASH_SIZE
*           MEMORY_ALLOCATE_FAILED
*   \else on success.
**/
Script * new_Script_p2pkh(BYTE *pubkey_hash, size_t size);

/** Create a P2PK Script.
*   \param  pubkey       Byte array.
*   \param  size         How many bytes, must be 65 or 33 bytes.
*   \return error codes:
*           INVALID_PUBKEY_SIZE
*           MEMORY_ALLOCATE_FAILED
*   \else on success.
**/
Script * new_Script_p2pk(BYTE *pubkey, size_t size);

/** Create a P2SH Script.
*   \param  hash         Byte array, script hash.
*   \param  size         How many bytes.
*   \return error codes:
*           MEMORY_ALLOCATE_FAILED
*   \else on success.
**/
Script * new_Script_p2sh(BYTE *hash, size_t size);

/** Create a multisig Script.
*   \param  m            How many keys to unlock, must smaller than or equal to total pubkey number.
*   \param  pubkeys      Public keys, must less than 20 keys.
*   \return error codes:
*           SCRIPT_MULTISIG_M_BIGGER_N
*           SCRIPT_MULTISIG_PUBKEYS_OVER_LIMIT
*           MEMORY_ALLOCATE_FAILED
*           SCRIPT_ADD_OPCODE_FAILED
*           SCRIPT_ADD_DATA_FAILED
*   \else on success.
*   Notice that 'pubkeys' is a 'template', still, need to be freed by delete_CLinkedlist() manually.
**/
Script * new_Script_p2sh_multisig(uint8_t m, CLinkedlist *pubkeys);
/* Delete an SCript object that created by construct function */
void delete_Script(Script *this);

/** Member Functions **/
/** Add an opcode.
*   \param  op          An opcode.
*   \return true on success.
*          false on error.
*   Parameter 'op' must be allocated by new_opcode(), once Script_add_opcode() returns true,
*   do not add 'op' to another Script or free 'op' by delete_Opcode() manually,
*   the destruct function will do the job.
**/
bool Script_add_opcode(Script *this, Opcode *op);

/** Add data bytes.
*   \param  data        The data bytes.
*   \param  size        Data size, how many bytes.
*   \return true on success.
*          false on error.
*   Parameter 'data' must point to heap memory, passing a pointer points to stack memory will cause errors.
*   Once Script_add_data() returns true, do not add 'data' to another Script or free 'data' manually,
*   the destruct function will do the job.
**/
bool Script_add_data(Script *this, BYTE *data, size_t size);

/** Script to string.
*   \param  size        Store the string's size, how many bytes.
*   \return error codes:
*           SCRIPT_HAS_NO_STATEMENTS
*           MEMORY_ALLOCATE_FAILED
*   \else on success.
*   The returned string must be freed manually.
**/
uint8_t * Script_to_string(Script *this, size_t *size);

/** Script to byte array.
*   \param  size        Store the byte array's size, how many bytes.
*   \return error codes:
*           SCRIPT_HAS_NO_STATEMENTS
*           MEMORY_ALLOCATE_FAILED
*   \else on success.
*   The returned byte array must be freed manually.
**/
BYTE * Script_to_bytes(Script *this, size_t *size);
/* Check if a valid P2PKH script */
bool Script_is_p2pkh(Script *this);
/* Check if a valid P2PK script */
bool Script_is_p2pk(Script *this);
/* Check if a valid P2SH script */
bool Script_is_p2sh(Script *this);
/* Check if a valid multisig script */
bool Script_is_p2sh_multisig(Script *this);
/* Check if the script has no statements */
bool Script_is_empty(Script *this);
/* How many statements */
uint32_t Script_get_length(Script *this);

/** Get an element's pointer from script
*   \param  index       Position of the element.
*   \param  size        Store the element's size, how many bytes.
*   \return error codes:
*           SCRIPT_HAS_NO_STATEMENTS
*           INDEX_OUT_RANGE
*   \else on success.
**/
void * Script_get_element(Script *this, uint64_t index, size_t *size);
size_t Script_total_size(Script *this);
uint32_t Script_check_element_size(Script *this);

#endif