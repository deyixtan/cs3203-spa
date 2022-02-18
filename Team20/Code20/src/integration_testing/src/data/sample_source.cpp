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

std::string sample_source2 =
    "procedure bar {"
    "    read x;"
    "    read y;"
    "    if ((x >= 0) && (y >= 0)) then {"
    "        while (x != 0) {"
    "            tan = y / x;"
    "            sin = y / length;"
    "            cos = x / length;"
    "            if (((sin * sin + cos * cos) == 1) || (!(tan != (sin / cos)))) then {"
    "                x = y / 2;"
    "                y = x / 2;"
    "            } else {"
    "                x = 0;"
    "            }"
    "            length = x * x + y * y;"
    "        }"
    "    } else {"
    "        x = 10;"
    "        y = 10;"
    "        length = x * x + y * y;"
    "    }"
    "}";
