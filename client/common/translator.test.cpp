#define CATCH_CONFIG_MAIN
#include "../vendor/catch.hpp"

#include <string>
#include "./translator.h"

TEST_CASE("Chars are decoded", "[translator]" ) {
    const char ctest1[] = { 0x1b, 0x1c, 0x1d };
    std::string test1(ctest1);
    REQUIRE(decode(test1, false, false) == std::string("abc"));
}

TEST_CASE("Chars are encoded", "[translator]" ) {
    const char ctest1[] = { 0x1b, 0x1c, 0x1d };
    std::string test1("abc");
    const char* result = encode(test1, false, false).c_str();
    REQUIRE(
        strcmp(
            result,
            ctest1
        ) == 0
    );
}

