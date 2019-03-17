#define BOOST_TEST_MODULE WayOut1Section1Tests

#include <boost/test/unit_test.hpp>
#include "test_common.h"

BOOST_AUTO_TEST_CASE(BasicLevel01)
{
    const char * indata = 1 + R"(
#####
#@$.#
#####
)";
    const char * outdata = 1 + R"(
7:R
)";
    test(indata, {outdata});
}

BOOST_AUTO_TEST_CASE(BasicLevel02)
{
    const char * indata = 1 + R"(
###__
#.###
#*$ #
# @ #
#####
)";
    const char * outdata = 1 + R"(
11:U 12:L
)";
    test(indata, {outdata});
}

BOOST_AUTO_TEST_CASE(BasicLevel03)
{
    const char * indata = 1 + R"(
######
#    #
#@$ .#
#    #
######
)";
    const char * outdata = 1 + R"(
14:R 15:R
)";
    test(indata, {outdata});
}


BOOST_AUTO_TEST_CASE(JuniorLevel03)
{
    const char * indata = 1 + R"(
#######
#.   .#
#.$$$.#
##$@$##
#.$$$.#
#.   .#
#######
)";
    const char * outdata = 1 + R"(
17:U 16:L 10:R 11:R 18:R 31:D 30:L 23:U 16:U 9:L 32:R 38:L 25:D 32:D 37:L 39:R
)";
    test(indata, {outdata});
}

