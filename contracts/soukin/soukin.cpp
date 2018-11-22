#include <string>
#include <eosiolib/eosio.hpp>
#include <eosiolib/asset.hpp>

using namespace eosio;

class Soukin : public eosio::contract {
    public:
    Soukin(account_name s): contract(s), _soukins(s, s)
    {}

    void transfer(uint64_t sender, uint64_t receiver) 
    {
        auto transfer_data = eosio::unpack_action_data<st_transfer>();
        if (transfer_data.from == _self){
            return;
        }

        auto s = _soukins.find(sender);

        if (s == _soukins.end()) {
            _soukins.emplace(get_self(), [&](auto& sk)
                {
                    sk.sender = sender;
                    sk.amount = (uint64_t)transfer_data.quantity.amount;
                }
            );
            return;
        }

        _soukins.modify(s, get_self(), [&](auto &sk)
            {
                sk.amount = sk.amount + (uint64_t)transfer_data.quantity.amount;
            }
        );
    }
 
    private:
    struct st_transfer {
        name from;
        name to;
        eosio::asset quantity;
        std::string memo;
    };

    // @abi table
    struct soukin 
    {
        account_name sender;
        uint64_t amount;

        uint64_t primary_key() const { return sender; }
    };
    typedef eosio::multi_index<N(soukin), soukin> soukins;
    soukins _soukins;
};

#define EOSIO_ABI_EX(TYPE, MEMBERS) \
extern "C" { \
   void apply( uint64_t receiver, uint64_t code, uint64_t action ) { \
      auto self = receiver; \
      if( code == self || code == N(eosio.token)) { \
      	 if( action == N(transfer) ){ \
      	 	eosio_assert( code == N(eosio.token), "Must transfer EOS"); \
      	 } \
         TYPE thiscontract( self ); \
         switch( action ) { \
            EOSIO_API(TYPE, MEMBERS) \
         } \
         /* does not allow destructor of thiscontract to run: eosio_exit(0); */ \
      } \
   } \
}

EOSIO_ABI_EX(Soukin, (transfer))