#   Bitcoin Tool Kit- A Combination of Useful Bitcoin Tools

##  Dependence

* OpenSSL-1.0.2g

##  Testing Enviroment

* Operating System: Ubuntu Linux 16.04.3 / 4.10.0-28-generic x86_64
* Compiler Version: GCC 5.4.0 20160609

## Features

* Generate Bitcoin Address.
* BaseX Encode/Decode
* Check Address Validation and Get hash160 Value
* Check private key validaion and get the address

##  Usage

	sudo apt install libssl-dev
    make
	./btctool
	
	Options:
  	-s                           Get a standard mainnet address.
  	-c                           Get a compressed mainnet address.
  	-t                           Get a standard testnet address.
  	-k                           Get a compressed testnet address.
  	-a <address>                 Check addess validaion and get the hash160 value.
  	-g <privkey>                 Check private key validaion and get the address.
                                 Supported format: [B6] [WIF] [Hexadecimal]
  	--base6e  -S <string>             Base6  encode
  	--base6d  -L <string>             Base6  decode
  	--base58e -F <string>             Base58 encode
  	--base58d -W <string>             Base58 decode

## Future Features

* Create and sign the raw transactions.
* Broadcast the signed transaction.
* Get the balance and UTXO on an address.
