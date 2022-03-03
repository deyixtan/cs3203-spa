#include <string>

std::string sample_source1 =
    "procedure computeCentroid {"
    "    count = 0;" // 1
    "    cenX = 0;" // 2
    "    cenY = 0;" // 3
    "    while ((x != 0) && (y != 0)) {" // 4
    "        count = count + 1;" // 5
    "        cenX = cenX + x;" // 6
    "        cenY = cenY + y;" // 7
    "    }"
    "    if (count == 0) then {" // 8
    "        flag = 1;" // 9
    "    } else {"
    "        cenX = cenX / count;" // 10
    "        cenY = cenY / count;" // 11
    "    }"
    "    normSq = cenX * cenX + cenY * cenY;" // 12
    "}";

std::string sample_source2 =
    "procedure bar {"
    "    read x;" // 1
    "    read y;" // 2
    "    if ((x >= 0) && (y >= 0)) then {" // 3
    "        while (x != 0) {" // 4
    "            tan = y / x;" // 5
    "            sin = y / length;" // 6
    "            cos = x / length;" // 7
    "            if (((sin * sin + cos * cos) == 1) || (!(tan != (sin / cos)))) then {" // 8
    "                x = y / 2;" // 9
    "                y = x / 2;" // 10
    "            } else {"
    "                x = 0;" // 11
    "            }"
    "            length = x * x + y * y;" // 12
    "        }"
    "    } else {"
    "        x = 10;" // 13
    "        y = 10;" // 14
    "        length = x * x + y * y;" // 15
    "    }"
    "}";
