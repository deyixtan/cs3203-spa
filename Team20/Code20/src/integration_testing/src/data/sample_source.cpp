#include <string>

std::string sample_source1 =
    "procedure computeCentroid {"
    "    count = 0;"
    "    cenX = 0;"
    "    cenY = 0;"
    "    while ((x != 0) && (y != 0)) {"
    "        count = count + 1;"
    "        cenX = cenX + x;"
    "        cenY = cenY + y;"
    "    }"
    "    if (count == 0) then {"
    "        flag = 1;"
    "    } else {"
    "        cenX = cenX / count;"
    "        cenY = cenY / count;"
    "    }"
    "    normSq = cenX * cenX + cenY * cenY;"
    "}";