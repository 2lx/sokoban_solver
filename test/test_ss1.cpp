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
    const char * outdata1 = 1 + R"(
17:U 16:L 10:R 11:R 18:R 31:D 30:L 23:U 16:U 9:L 32:R 38:L 25:D 32:D 37:L 39:R
)";
    const char * outdata2 = 1 + R"(
17:U 16:L 10:R 11:R 18:R 31:D 30:L 23:U 16:U 9:L 32:R 38:L 25:D 32:D 39:R 37:L
)";
    test(indata, {outdata1, outdata2});
}

BOOST_AUTO_TEST_CASE(JuniorLevel01)
{
    const char * indata = 1 + R"(
#######
#.   .#
#  $  #
# $@$ #
#  $  #
#.   .#
#######
)";
    const char * outdata1 = 1 + R"(
17:U 10:L 9:L 23:L 22:D 25:U 18:U 11:R 29:D 31:R 32:R 33:D
)";
    const char * outdata2 = 1 + R"(
17:U 10:L 9:L 23:L 22:D 29:D 25:U 18:U 11:R 31:R 32:R 33:D
)";
    test(indata, {outdata1, outdata2});
}

BOOST_AUTO_TEST_CASE(JuniorLevel06)
{
    const char * indata = 1 + R"(
#######
##. .##
#.$$$.#
# $@$ #
#.$$$.#
##. .##
#######
)";
    const char * outdata = 1 + R"(
17:U 16:L 10:R 18:R 31:D 30:L 23:U 16:U 32:R 38:L 25:D 32:D
)";
    test(indata, {outdata});
}

BOOST_AUTO_TEST_CASE(Corral01)
{
    const char * indata = 1 + R"(
#######
#. $  #
#+$   #
#######
)";
    const char * outdata = 1 + R"(
10:R 11:L 10:L 9:L 16:L
)";
    test(indata, {outdata});
}

BOOST_AUTO_TEST_CASE(Bipartite01)
{
    const char * indata = 1 + R"(
######
#.$  #
# @$ #
#$ . #
#. $.#
######
)";
    const char * outdata = 1 + R"(
8:L 15:D 19:D 27:R
)";
    test(indata, {outdata});
}


