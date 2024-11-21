#include "crutils.h"
#include <cryptopp/sha.h>
#include <cryptopp/hex.h>
#include <cryptopp/osrng.h>
#include <cryptopp/filters.h>
#include <algorithm>
#include <string>

namespace crutils
{
    std::string calculateHash(const std::string &data)
    {
        CryptoPP::SHA1 sha1;
        std::string hash;
        CryptoPP::StringSource(data, true,
                               new CryptoPP::HashFilter(sha1,
                                                        new CryptoPP::HexEncoder(
                                                            new CryptoPP::StringSink(hash), false)));

        std::transform(hash.begin(), hash.end(), hash.begin(), ::toupper);

        return hash;
    }

}
