#define BOOST_TEST_MODULE SokobanSection1Tests

#include <boost/test/unit_test.hpp>
#include "test_solver_common.h"
#include <fstream>
#include <streambuf>

using namespace std;

const char * filepath = "./levels/";

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

BOOST_AUTO_TEST_CASE(JuniorLevel03)
{
    ifstream fs(string(filepath) + "jr03.sok", ios_base::in);
    string indata(istreambuf_iterator<char>{fs}, {});

    const char * outdata1 = 1 + R"(
17:U 16:L 10:R 11:R 18:R 31:D 30:L 23:U 16:U 9:L 32:R 38:L 25:D 32:D 37:L 39:R
)";
    const char * outdata2 = 1 + R"(
17:U 16:L 10:R 11:R 18:R 31:D 30:L 23:U 16:U 9:L 32:R 38:L 25:D 32:D 39:R 37:L
)";
    const char * outdata3 = 1 + R"(
17:U 16:L 18:R 10:L 9:L 31:D 30:L 32:R 25:U 18:U 38:R 39:R 11:R 23:D 30:D 37:L
)";
    const char * outdata4 = 1 + R"(
17:U 18:R 10:L 9:L 31:D 32:R 25:U 18:U 11:R 30:L 16:L 38:R 23:D 30:D 37:L 39:R
)";
    const char * outdata5 = 1 + R"(
17:U 18:R 10:L 9:L 31:D 32:R 25:U 18:U 11:R 16:L 30:L 38:R 23:D 30:D 37:L 39:R
)";
    test(indata, {outdata1, outdata2, outdata3, outdata4, outdata5});
}

BOOST_AUTO_TEST_CASE(JuniorLevel01)
{
    ifstream fs(string(filepath) + "jr01.sok", ios_base::in);
    string indata(istreambuf_iterator<char>{fs}, {});

    const char * outdata1 = 1 + R"(
17:U 10:L 9:L 23:L 22:D 25:U 18:U 11:R 29:D 31:R 32:R 33:D
)";
    const char * outdata2 = 1 + R"(
17:U 10:L 9:L 23:L 22:D 29:D 25:U 18:U 11:R 31:R 32:R 33:D
)";
    const char * outdata3 = 1 + R"(
17:U 10:L 9:L 23:L 25:U 18:U 11:R 22:D 29:D 31:R 32:R 33:D
)";
    const char * outdata4 = 1 + R"(
17:U 10:L 9:L 25:U 18:U 11:R 23:L 22:D 29:D 31:R 32:R 33:D
)";
    test(indata, {outdata1, outdata2, outdata3, outdata4});
}

BOOST_AUTO_TEST_CASE(JuniorLevel06)
{
    ifstream fs(string(filepath) + "jr06.sok", ios_base::in);
    string indata(istreambuf_iterator<char>{fs}, {});

    const char * outdata1 = 1 + R"(
17:U 16:L 10:R 18:R 31:D 30:L 23:U 16:U 32:R 38:L 25:D 32:D
)";
    const char * outdata2 = 1 + R"(
23:L 16:U 25:R 18:U 30:D 22:U 32:D 26:D 17:R 18:R 31:L 30:L
)";
    const char * outdata3 = 1 + R"(
17:U 16:L 10:R 31:D 30:L 23:U 18:R 16:U 32:R 38:L 25:D 32:D
)";
    const char * outdata4 = 1 + R"(
23:L 16:U 25:R 18:U 22:D 17:L 16:L 32:D 26:U 30:D 31:R 32:R
)";
    test(indata, {outdata1, outdata2, outdata3, outdata4});
}

BOOST_AUTO_TEST_CASE(Bipartite01)
{
    ifstream fs(string(filepath) + "bipartite01.sok", ios_base::in);
    string indata(istreambuf_iterator<char>{fs}, {});

    const char * outdata = 1 + R"(
8:L 15:D 19:D 27:R
)";
    test(indata, {outdata});
}

BOOST_AUTO_TEST_CASE(Example03)
{
    ifstream fs(string(filepath) + "example03.sok", ios_base::in);
    string indata(istreambuf_iterator<char>{fs}, {});

    const char * outdata = 1 + R"(
29:U 20:R 21:R 22:R 23:R 30:R 31:R 32:R 24:R 25:D
)";
    test(indata, {outdata});
}

