#ifndef _btk_status_
#define _btk_status_

#include "common.h"

/* 0x0000 ~ 0x00ff : Common reserved */
#define FAILED                 (void *)0x0000
#define SUCCEEDED              (void *)0x0001
#define BYTE_TYPE              (void *)0x0002
#define INT8_TYPE              (void *)0x0003
#define INT16_TYPE             (void *)0x0004
#define INT32_TYPE             (void *)0x0005
#define INT64_TYPE             (void *)0x0006
#define UINT8_TYPE             (void *)0x0007
#define UINT16_TYPE            (void *)0x0008
#define UINT32_TYPE            (void *)0x0009
#define UINT64_TYPE            (void *)0x000A
#define FLOAT32_TYPE           (void *)0x000B
#define FLOAT64_TYPE           (void *)0x000C
#define BOOL_TYPE              (void *)0x000D

/* 0x0100 ~ 0x0fff : Global */
#define INDEX_OUT_RANGE        (void *)0x0100
#define INVALID_RIPEMD160_SIZE (void *)0x0101
#define INVALID_SHA256_SIZE    (void *)0x0102
#define INVALID_PUBKEY_SIZE    (void *)0x0103
#define INVALID_PRIVKEY_SIZE   (void *)0x0104
#define PASSING_NULL_POINTER   (void *)0x0105
#define MEMORY_ALLOCATE_FAILED (void *)0x0102

/* 0x1000 ~ 0x100f : CLinkedlist */
#define CLINKEDLIST_EMPTY (void *)0x1000

/* 0x001010 ~ 0x00101f : CStack */
#define CSTACK_EMPTY            (void *)0x1010
#define CSTACK_FULL             (void *)0x1011
#define CSTACK_INVALID_CAPACITY (void *)0x1012

/* 0x1020 ~ 0x103f : Script */
#define SCRIPT_MULTISIG_M_BIGGER_N              (void *)0x1020
#define SCRIPT_HAS_NO_ELEMENTS                  (void *)0x1021
#define SCRIPT_REMAIN_BYTES_LESS_THAN_PUSH      (void *)0x1022
#define SCRIPT_ELEMENT_SIZE_OVERLIMIT           (void *)0x1023
#define SCRIPT_SIZE_TO_PUSH_NOT_EQUAL_EXPECTED  (void *)0x1024
#define SCRIPT_CONTAINED_INVALID_ELEMENT        (void *)0x1025

/* 0x1040 ~ 0x1050 : Interpreter */
// Opcode execution status.
#define OPERATION_EXECUTED     (void *)0x1040 // No error and executed.
#define OPERATION_NOT_EXECUTED (void *)0x1041 // No error but not executed.
// Interpretation result.
#define INTERPRETER_TRUE  (void *)0x1042
#define INTERPRETER_FALSE (void *)0x1043
// Interpret-time errors.
#define INTERPRETER_ERROR                  (void *)0x1044
#define INTERPRETER_OP_ELSE_WITHOUT_PREFIX (void *)0x1045
#define INTERPRETER_IF_WITHOUT_ENDIF       (void *)0x1046
#define INTERPRETER_ENDIF_WITHOUT_IF       (void *)0x1047
#define INTERPRETER_OP_RETURN_OVERSIZE     (void *)0x1048
#define INTERPRETER_OPCODE_DISABLED        (void *)0x1049
// Functional errors.
#define INTERPRETER_ALREADY_LOADED   (void *)0x104A
#define INTERPRETER_NO_SCRIPT_LOADED (void *)0x104B

/* 0x1051 ~ 0x1060 Address */


bool __btc_is_status_ptr(void *ptr);
void * __btc_status_ptr(const uintptr_t statusnum);
uintptr_t __btc_status_num(const void *errptr);

#endif