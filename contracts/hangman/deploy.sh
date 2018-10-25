eosiocpp -o /mycontracts/hangman/hangman.wast /mycontracts/hangman/hangman.cpp
eosiocpp -g /mycontracts/hangman/hangman.abi /mycontracts/hangman/hangman.cpp
cleos -u https://api-kylin.eoslaomao.com set contract kei5yamamoto /mycontracts/hangman