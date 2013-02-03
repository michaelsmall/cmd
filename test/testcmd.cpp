#include "cmd.hpp"

    /*! An command line item.
    struct cmd_item {
        unsigned attr;///< Attribute bitset
        char short_key;///< Short option (e.g. -x)
        std::string long_key;///< Long option (e.g. --xyz)
        cmd_apply apply;///< Successful parse callback
        std::string descr;///< Description of the item
    };
    */

const char* test_argv[] = {"-x", "--foo", "-y", "--bar", "goo", "moo"};

int main(int, char** argv) {
    struct {
        bool foo = false;
        bool bar = false;
        std::string goo;
        std::string moo;
    } data;
    return 0;
}
