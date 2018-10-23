#include <stdio.h>
#include <string>
#include <eosiolib/eosio.hpp>
#include <eosiolib/print.hpp>

using namespace eosio;

class Hangman : public contract {
    public:

    // constructor
    Hangman(account_name s):contract(s), _words(s,s), _games(s,s)
    {}

    //@abi action
    void version()
    {
        print("0.0.2");
    }

    //@abi action
    void start(account_name name)
    {
        uint64_t seed = _games.available_primary_key();
        uint64_t idx = seed % countwords();

        auto w = _words.get(idx);

        std::vector<char> initialChars;

        auto k = _games.available_primary_key();

        auto g = _games.emplace(get_self(), [&](auto& gm)
            {
                gm.key = k;
                gm.wordKey = idx;
                gm.player = name;
                gm.status = 0;
                gm.usedChars = initialChars;
                gm.remainingTrial = 8;
            }
        );

        print("Game ID is ", k, ": ");
        print(masked(w.content, initialChars).c_str());
    }

    //@abi action
    void submit(uint64_t gameid, std::string s)
    {
        char c = s.c_str()[0];
        auto g = _games.get(gameid);
        auto w = _words.get(g.wordKey);
        auto gitr = _games.find(gameid);

        if (s.length() != 1) { print("Invalid character"); return; }
        if (included(g.usedChars, c)) { print("Already subitted"); return; }
        if (g.status != 0){ print("Game is already finished"); return; }

        g.usedChars.push_back(c);

        _games.modify(gitr, string_to_name("hello.code"), [&]( auto& gm ) {
            gm.usedChars = g.usedChars;
        });

        if (!hitted(w.content, c)) {
            _games.modify(gitr, string_to_name("hello.code"), [&]( auto& gm ) {
                gm.remainingTrial = g.remainingTrial - 1;
            });
        }

        auto rslt = masked(w.content, g.usedChars);
        auto clrd = cleared(w.content, g.usedChars);

        if (clrd) {
            print("Clear!!!!");
            _games.modify(gitr, string_to_name("hello.code"), [&]( auto& gm ) {
                gm.status = 1;
            });
        } else if (g.remainingTrial == 0) {
            print("Failed...");
            _games.modify(gitr, string_to_name("hello.code"), [&]( auto& gm ) {
                gm.status = 2;
            });
        } else {
            print("Remaining ", g.remainingTrial, " times: ");
            print(rslt);
        }
    }

    //@abi action
    void getgame(uint64_t index)
    {
        auto g = _games.get(index);
        auto w = _words.get(g.wordKey);

        auto m = masked(w.content, g.usedChars);
        auto n = name{g.player}.to_string();
        print("word: ", m.c_str(), ", player: ", n.c_str());
    }

    //@abi action
    void registerword(std::string newWord)
    {
        if (!validword(newWord)) { print("Invalid word"); return; }

        for(auto& item : _words)
        {
            if (item.content == newWord) { print("already registered: ", newWord); return; }
        }

        _words.emplace(get_self(), [&](auto& w)
            {
                w.key = _words.available_primary_key();
                w.content = newWord;
            }
        );
    }

    //@abi action
    void listwords()
    {
        for(auto& item : _words) { print(item.content, ","); }
    }

    private:
    template <class T>
    bool included(std::vector<T> arr, T val)
    {
        for (auto v : arr)
        {
            if (v == val) { return true; }
        }
        return false;
    }

    bool validword(std::string w)
    {
        return true;
    }

    std::string masked(std::string word, std::vector<char> used)
    {
        std::vector<char> m;
        for (auto& c : word)
        {
            if (included(used, c)) {
                m.push_back(c);
            } else {
                m.push_back('_');
            }
        }
        std::string str(m.begin(), m.end());
        return str;
    }

    bool cleared(std::string word, std::vector<char> used) 
    {
        for (auto& c : word) {
            if (!included(used, c)) {
               return false;
            }
        }
        return true;
    }

    bool hitted(std::string word, char submitted)
    {
        std::vector<char> chars(word.begin(), word.end());
        return included(chars, submitted);
    }

    uint64_t countwords()
    {
        uint64_t count = 0;
        for(auto& item : _words) {
            count++;
        }
        return count;
    }

    // @abi table
    struct word {
        uint64_t key;
        std::string content;

        uint64_t primary_key() const { return key; }
    };
    typedef multi_index<N(word), word> words;

    // @abi table
    struct game {
        uint64_t key;
        uint64_t wordKey;
        account_name player;
        uint8_t status; // 0: playing, 1: win, 2: lose
        std::vector<char> usedChars;
        uint8_t remainingTrial;

        uint64_t primary_key() const { return key; }
    };
    typedef multi_index<N(game), game> games;

    words _words;
    games _games;
};

EOSIO_ABI(Hangman, (version)(start)(registerword)(listwords)(getgame)(submit));