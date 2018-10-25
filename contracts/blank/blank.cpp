#include <stdio.h>
#include <string>
#include <eosiolib/eosio.hpp>
#include <eosiolib/print.hpp>

using namespace eosio;

class Blank : public contract {
    public:
    // constructor
    Blank(account_name s):contract(s)
    {}

    //@abi action
    void version() {
        print("0.0.1");
    }
};

EOSIO_ABI(Blank, (version));
