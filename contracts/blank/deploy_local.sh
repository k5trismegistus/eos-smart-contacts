eosiocpp -o /mycontracts/blank/blank.wast /mycontracts/blank/blank.cpp
eosiocpp -g /mycontracts/blank/blank.abi /mycontracts/blank/blank.cpp
cleos -u http://nodeos:8888 set contract code /mycontracts/blank