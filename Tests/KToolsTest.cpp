#include "doctest.h"
#include "Utils/KTools.h"

// sql.db values are stored encoded; existing saves must stay readable.

TEST_CASE("encode shifts every byte by the key and appends the key")
{
    KTools tools;
    std::string value = "25";
    tools.encode(value, 3);
    CHECK(value == std::string("58\x03", 3));
}

TEST_CASE("decode reverses encode")
{
    KTools tools;
    for (int key : {1, 7, 40, 99})
    {
        CAPTURE(key);
        std::string value = "1000";
        tools.encode(value, key);
        tools.decode(value);
        CHECK(value == "1000");
    }
}
