#ifndef _btk_err_
#define _btk_err_

/* 0x000000 ~ 0x0000ff : Common reserved */
#define FAILED                 (void *)0x000000
#define SUCCESS                (void *)0x000001
#define BYTE_TYPE              (void *)0x000002
#define INT8_TYPE              (void *)0x000003
#define INT16_TYPE             (void *)0x000004
#define INT32_TYPE             (void *)0x000005
#define INT64_TYPE             (void *)0x000006
#define UINT8_TYPE             (void *)0x000007
#define UINT16_TYPE            (void *)0x000008
#define UINT32_TYPE            (void *)0x000009
#define UINT64_TYPE            (void *)0x00000A
#define FLOAT32_TYPE           (void *)0x00000B
#define FLOAT64_TYPE           (void *)0x00000C
#define BOOL_TYPE              (void *)0x00000D

/* 0x000100 ~ 0x000fff : Global */
#define INDEX_OUT_RANGE        (void *)0x000100
#define INVALID_RIPEMD160_SIZE (void *)0x000101
#define INVALID_SHA256_SIZE    (void *)0x000102
#define INVALID_PUBKEY_SIZE    (void *)0x000103
#define INVALID_PRIVKEY_SIZE   (void *)0x000104
#define PASSING_NULL_POINTER   (void *)0x000105
#define MEMORY_ALLOCATE_FAILED (void *)0x000102

/* 0x001000 ~ 0x00100f : CLinkedlist */
#define CLINKEDLIST_EMPTY (void *)0x001000

/* 0x001010 ~ 0x00101f : CStack */
#define CSTACK_EMPTY            (void *)0x001010
#define CSTACK_FULL             (void *)0x001011
#define CSTACK_INVALID_CAPACITY (void *)0x001012

/* 0x001020 ~ 0x00103f : Script */
#define SCRIPT_MULTISIG_M_BIGGER_N              (void *)0x001020
#define SCRIPT_HAS_NO_ELEMENTS                  (void *)0x001021
#define SCRIPT_REMAIN_BYTES_LESS_THAN_PUSH      (void *)0x001022
#define SCRIPT_ELEMENT_SIZE_OVERLIMIT           (void *)0x001023
#define SCRIPT_SIZE_TO_PUSH_NOT_EQUAL_EXPECTED  (void *)0x001024
#define SCRIPT_CONTAINED_INVALID_ELEMENT        (void *)0x001025

/* 0x001040 ~ 0x001050 : Interpreter */
// Opcode execution status.
#define OPERATION_EXECUTED     (void *)0x001040 // No error and executed.
#define OPERATION_NOT_EXECUTED (void *)0x001041 // No error but not executed.
// Interpretation result.
#define INTERPRETER_CORRECT (void *)0x001042
#define INTERPRETER_BREAK   (void *)0x001043
// Interpret-time errors.
#define INTERPRETER_ERROR                  (void *)0x001044
#define INTERPRETER_OP_ELSE_WITHOUT_PREFIX (void *)0x001045
#define INTERPRETER_IF_WITHOUT_ENDIF       (void *)0x001046
#define INTERPRETER_ENDIF_WITHOUT_IF       (void *)0x001047
#define INTERPRETER_OP_RETURN_OVERSIZE     (void *)0x001048
#define INTERPRETER_OPCODE_DISABLED        (void *)0x001049
// Functional errors.
#define INTERPRETER_ALREADY_LOADED   (void *)0x00104A
#define INTERPRETER_NO_SCRIPT_LOADED (void *)0x00104B

#endif