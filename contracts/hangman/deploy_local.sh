eosiocpp -o /mycontracts/hangman/hangman.wast /mycontracts/hangman/hangman.cpp
eosiocpp -g /mycontracts/hangman/hangman.abi /mycontracts/hangman/hangman.cpp
cleos -u http://nodeos:8888 set contract code /mycontracts/hangman