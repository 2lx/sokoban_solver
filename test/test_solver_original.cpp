#define BOOST_TEST_MODULE SolverOriginalTests

#include <boost/test/unit_test.hpp>
#include "test_solver_common.h"
#include <fstream>
#include <streambuf>

using namespace std;

const char * filepath = "./levels/original_sokoban/";

BOOST_AUTO_TEST_CASE(Level01)
{
    ifstream fs(string(filepath) + "01.sok", ios_base::in);
    string indata(istreambuf_iterator<char>{fs}, {});

    const char * outdata = 1 + R"(
83:L 64:U 45:D 81:D 138:R 139:R 140:R 141:R 142:R 143:R 144:R 145:R 146:R 147:R 148:U 129:R 135:R 100:D 136:R 137:R 138:R 139:R 140:R 141:R 142:R 143:R 144:R 145:R 146:R 147:R 148:R 149:R 150:U 119:D 138:R 139:R 140:R 141:R 142:R 143:R 144:R 145:R 146:R 147:R 148:R 149:R 43:D 62:D 81:D 100:D 119:D 138:R 139:R 140:R 141:R 142:R 143:R 144:R 145:R 146:R 147:R 148:R 82:L 64:D 81:D 83:L 100:D 119:D 82:L 81:D 100:D 138:R 139:R 140:R 141:R 119:D 138:R 139:R 142:R 140:R 143:R 144:R 145:R 146:R 147:R 148:D 167:R 168:R 141:R 142:R 143:R 144:R 145:R 146:R 147:R 148:D 167:R
)";
    test(indata, {outdata});
}

BOOST_AUTO_TEST_CASE(Level02)
{
    ifstream fs(string(filepath) + "02.sok", ios_base::in);
    string indata(istreambuf_iterator<char>{fs}, {});

    const char * outdata = 1 + R"(
80:R 95:L 81:U 67:U 38:L 48:D 62:L 61:L 60:L 59:U 45:U 31:U 17:L 16:L 94:D 35:L 34:D 48:D 62:L 61:L 60:L 59:U 45:U 31:U 17:L 37:L 36:L 35:L 34:D 48:D 62:L 61:L 60:L 59:U 45:U 31:L 30:L 53:U 39:L 38:L 37:L 36:L 35:L 34:D 48:D 62:L 61:L 60:L 59:U 45:U 31:L 102:R 103:R 104:U 90:U 76:U 62:U 48:D 62:L 61:L 60:L 59:U 45:L 44:L 105:L 104:L 103:R 104:U 90:U 76:U 62:L 61:L 60:L 59:U 45:L 108:U 94:U 80:R 81:U 67:U 53:U 39:L 38:L 37:L 36:L 35:L 34:D 48:D 62:L 61:L 60:L 59:L 58:L 107:L 93:U 79:R 80:R 81:U 67:U 53:U 39:L 38:L 37:L 36:L 35:L 34:D 48:D 62:L 61:L 60:L 59:L 106:L 105:L 104:L 103:R 104:U 90:U 76:U 62:L 61:L 60:L 59:D 73:L 72:L 109:L 108:L 107:L 106:L 105:L 104:L 103:R 104:U 90:U 76:U 62:L 61:L 60:L 59:D 73:L
)";
    test(indata, {outdata});
}

BOOST_AUTO_TEST_CASE(Level03)
{
    ifstream fs(string(filepath) + "03.sok", ios_base::in);
    string indata(istreambuf_iterator<char>{fs}, {});

    const char * outdata = 1 + R"(
64:D 128:L 127:L 126:L 125:L 124:U 107:L 106:L 105:L 104:L 115:R 131:L 130:L 129:L 128:L 127:L 126:L 125:L 124:U 107:L 106:L 105:L 46:D 81:D 80:L 63:D 79:D 96:D 113:D 130:L 129:L 128:L 127:L 126:L 125:L 124:U 107:L 106:L 80:L 79:D 96:D 113:D 130:L 129:L 128:L 127:L 126:L 125:L 124:U 107:L 116:L 115:L 98:D 115:D 132:L 131:L 130:L 129:L 128:L 127:L 126:L 125:L 124:L 123:L 122:L 114:L 113:D 130:L 129:L 128:L 127:L 126:L 125:L 124:L 123:L 95:R 96:D 113:D 130:L 129:L 128:L 127:L 126:L 125:L 124:L 112:L 111:D 128:L 127:L 126:L 125:L 124:D 141:L 140:L 139:L 138:L 61:R 78:D 44:D 95:D 61:D 78:D 112:L 95:D 112:D 129:L 128:L 127:L 126:L 125:L 124:D 141:L 140:L 139:L 111:D 62:L 61:D 78:D 95:D 112:L 128:L 127:L 126:L 125:L 124:D 141:L 140:L 111:D 128:L 127:L 126:L 125:L 124:D 141:L
)";
    test(indata, {outdata});
}

