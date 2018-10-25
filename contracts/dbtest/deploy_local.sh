eosiocpp -o /mycontracts/dbtest/dbtest.wast /mycontracts/dbtest/dbtest.cpp
eosiocpp -g /mycontracts/dbtest/dbtest.abi /mycontracts/dbtest/dbtest.cpp
cleos -u http://nodeos:8888 set contract code /mycontracts/dbtest