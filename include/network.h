#ifndef NETWORK_H
#define NETWORK_H

#include "common.h"

// Reference: bitcoin.org/en/developer-reference#version
typedef version_message_st {
	int32_t  version;                 //  4 bytes, Required
	uint64_t services;                //  8 bytes, Required
	int64_t  timestamp;               //  8 bytes, Required
	uint64_t addr_recv_services;      //  8 bytes, Required
	char     addr_recv_ip_address;    // 16 bytes, Required
	uint16_t addr_recv_port;          //  2 bytes, Required
	uint64_t addr_trans_services;     //  8 bytes, Required
	char     addr_trans_ip_address;   // 16 bytes, Required
	uint16_t addr_trans_port;         //  2 bytes, Required
	uint64_t nonce;                   //  8 bytes, Required
	uint8_t  user_agent_bytes;        //  Varies , Required (Choose uint8_t for minimum length)
//	string   user_agent;              //  Varies , Required (if user_agent bytes > 0)
	int32_t  start_height;            //  4 bytes, Required
//	bool     relay;                   //  1 bytes, Optional
} VERSION_MESSAGE;

// Reference: bitcoin.org/en/developer-reference#message-headers
//            en.bitcoin.it/wiki/Protocol_documentation#Message_structure
typedef message_header_st {
	char     magic[4];          //  4 bytes, Required
	char     command_name[12];  // 12 bytes, Required
	uint32_t payload_size;      //  4 bytes, Required
	char     checksum[4];       //  4 bytes, Required
} MESSAGE_HEADER;

#endif