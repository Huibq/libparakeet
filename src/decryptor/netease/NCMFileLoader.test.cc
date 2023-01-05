#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <cstdint>
#include <filesystem>
#include <fstream>
#include <memory>
#include <vector>

#include "parakeet-crypto/decryptor/netease/NCMFileLoader.h"
#include "test/helper.test.hh"

using namespace parakeet_crypto::decryptor::netease;
using namespace parakeet_crypto::decryptor;
using namespace parakeet_crypto;

const NCMContentKeyProtectionKey kAESKey = []() {
    NCMContentKeyProtectionKey result;
    test::GenerateTestData(result, "ncm-test-key");
    return result;
}();

TEST(NeteaseNCMLoader, SimpleCase) {
    std::vector<uint8_t> test_data(test::kSize4MiB);

    test::GenerateTestData(test_data, "ncm-test-data");

    // key:
    //    neteasecloudmusic625064132972419780152239073outTde996wZqM
    //    k9R2NAS0zMZ9fHd4z37ei2drOBpNEYWFiN0jMiujKyv7pXPkxtj8eTck0
    //    0Jixun0Parakeet

    auto header_override = std::to_array<uint8_t>({
        0x43, 0x54, 0x45, 0x4E, 0x46, 0x44, 0x41, 0x4D,

        0xff, 0xff,

        0x90, 0x00, 0x00, 0x00,

        0x4D, 0x3C, 0x5A, 0x96, 0x74, 0x42, 0x64, 0xD3, 0x14, 0x4F, 0x77, 0xBB, 0x3C, 0x7B, 0x60, 0x56, 0x96, 0xA0,
        0xD0, 0x12, 0xCB, 0xB8, 0xB6, 0x86, 0x13, 0xE6, 0xEF, 0x51, 0x00, 0x7E, 0xED, 0x02, 0xDF, 0xFE, 0xD2, 0xED,
        0x6C, 0x4A, 0xA1, 0x33, 0x0C, 0xEA, 0x8E, 0x00, 0x3A, 0xBC, 0xAA, 0xFB, 0x47, 0xAC, 0xE3, 0x0B, 0xBA, 0xEA,
        0xA4, 0x88, 0x6D, 0x84, 0x74, 0xBE, 0x28, 0x86, 0x1B, 0x43, 0xF7, 0x2F, 0x2A, 0xFD, 0x85, 0x44, 0xA0, 0xFC,
        0xCD, 0xE0, 0xD7, 0xEC, 0x8F, 0xDB, 0xB0, 0xB5, 0x39, 0xFD, 0x94, 0x27, 0xD5, 0x24, 0x98, 0xCE, 0x2E, 0x6B,
        0x7C, 0xBB, 0x16, 0x55, 0x0B, 0x63, 0x63, 0x3E, 0x8E, 0x26, 0x91, 0xF9, 0x32, 0x37, 0x38, 0xC0, 0x93, 0xD9,
        0xCF, 0x40, 0x44, 0x5A, 0x6E, 0xDE, 0xEA, 0xCA, 0x27, 0xCB, 0x50, 0x54, 0x12, 0xFE, 0x12, 0x89, 0x59, 0x06,
        0x72, 0xA9, 0x81, 0x33, 0x1F, 0xBE, 0xCB, 0xC9, 0x38, 0xFA, 0xE8, 0x94, 0xEB, 0xD7, 0x04, 0xF2, 0x58, 0xB4,

        0x03, 0x00, 0x00, 0x00, 0x01, 0x02, 0x03,

        0xff, 0xff, 0xff, 0xff, 0xff,

        0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff,
    });
    std::ranges::copy(header_override, test_data.begin());

    auto result = test::DecryptTestContent(CreateNeteaseDecryptor(kAESKey), test_data);

    ASSERT_EQ(test::kSize4MiB - header_override.size(), result.size());
    test::VerifyHash(result, "dae77d29821092561702e3cde97add3558f21a1607c9aab0599983632ce0d54b");
}
