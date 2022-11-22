#include "uinteger.h"
#include "doctest.h"

namespace emu::misc {

    void dummy() {
        // To force the cpp file to be compiled and the tests to be registered.
    }

    TEST_CASE("crosscutting: UInteger") {
        SUBCASE("should wrap around in the constructor") {
            UInteger<1000> value(1010);

            CHECK_EQ(value.underlying(), 10);
        }

        SUBCASE("should not have the size parameter as legal value") {
            UInteger<1000> value(1000);

            CHECK_EQ(false, value.underlying() == 1000);
            CHECK_EQ(value.underlying(), 0);
        }

        SUBCASE("should add like usual when result is under the max value") {
            UInteger<1000> value1(10);
            UInteger<1000> value2(20);

            UInteger<1000> result = value1 + value2;

            CHECK_EQ(result.underlying(), UInteger<1000>(30).underlying());

            result = result + result + result;

            CHECK_EQ(result.underlying(), UInteger<1000>(90).underlying());
        }

        SUBCASE("should wrap around when adding") {
            UInteger<1000> value1(500);
            UInteger<1000> value2(501);

            UInteger<1000> result = value1 + value2;

            CHECK_EQ(result.underlying(), UInteger<1000>(1).underlying());
        }

        SUBCASE("should subtract like usual when result is above 0") {
            UInteger<1000> value1(100);
            UInteger<1000> value2(20);

            UInteger<1000> result = value1 - value2;

            CHECK_EQ(result.underlying(), UInteger<1000>(80).underlying());

            result = result - value2 - value2;

            CHECK_EQ(result.underlying(), UInteger<1000>(40).underlying());
        }

        SUBCASE("should wrap around when subtracting") {
            UInteger<1000> value1(500);
            UInteger<1000> value2(501);

            UInteger<1000> result = value1 - value2;

            CHECK_EQ(result.underlying(), UInteger<1000>(999).underlying());
        }

        SUBCASE("should add and subtract at the same time without problem") {
            UInteger<1000> value1(100);
            UInteger<1000> value2(20);
            UInteger<1000> value3(300);

            UInteger<1000> result = value1 - value2 + value3;

            CHECK_EQ(result.underlying(), UInteger<1000>(380).underlying());
        }
    }
}
