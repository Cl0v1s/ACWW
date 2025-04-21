#include "../vendor/catch.hpp"

#include <string>
#include "./translator.h"

TEST_CASE("Chars are decoded", "[translator]" ) {
    std::wstring target(L"abc’");
    const uint8_t input[] = { 0x1b, 0x1c, 0x1d, 177 };
    std::wstring output = decode(input, 4, false, false);
    REQUIRE(output == target);
}

TEST_CASE("Chars are encoded", "[translator]" ) {
    const uint8_t target[] = { 0x1b, 0x1c, 0x1d, 177 };
    std::wstring input(L"abc'");
    uint8_t output[input.length()];
    encode(input, output, false, false);
    for(unsigned int i = 0; i < input.length(); i++) {
        REQUIRE(output[i] == target[i]);
    }
    REQUIRE(wwCharacterDictionary[output[3]] == L'’');
}

TEST_CASE("Single quote is correctly encoded and decoded", "[translator]") {
    std::wstring input(L"'");
    uint8_t encoded[input.length()];
    encode(input, encoded, false, false);
    std::wstring decoded = decode(encoded, 1, false, false);
    REQUIRE(decoded == L"’");
}

TEST_CASE("Special Single quote is correctly encoded and decoded", "[translator]") {
    std::wstring base(L"’");
    uint8_t encoded[2];
    encode(base, encoded, false, false);
    std::wstring decoded = decode(encoded, 1, false, false);
    REQUIRE(decoded == L"’");
}

