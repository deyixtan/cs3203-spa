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

std::string sample_source3 =
    "procedure bar {"
    "    read x;" // 1
    "    call foo;" // 2
    "    call buzz;" // 3
    "}"
    "procedure foo {"
    "    read x;" // 4
    "    call fizz;" // 5
    "}"
    "procedure fizz {"
    "    read x;" // 6
    "    call buzz;" // 7
    "}"
    "procedure buzz {"
    "    read x;" // 8
    "}";

std::string sample_source4 =
    "procedure bar {"
    "    read x;" // 1
    "    read y;" // 2
    "    while (x != 0) {" // 3
    "        tan = y / x;" // 4
    "        sin = y / length;" // 5
    "        sin = y / length;" // 6
    "    }"
    "    if (((sin * sin + cos * cos) == 1) || (!(tan != (sin / cos)))) then {" // 7
    "        x = y / 2;" // 8
    "    } else {"
    "        x = 0;" // 9
    "    }"
    "    x = 10;" // 10
    "    y = 10;" // 11
    "    length = x * x + y * y;" // 12
    "}";

std::string sample_source5 =
   "procedure main {"
   "    if (a == b) then {" // 1
   "        if ((3 + 5) % 2 / (2 + 3) == 1) then {" // 2
   "            if (((a == 10101010) || (2 == 2)) && (a != 7)) then {" // 3
   "                y = 0;" // 4
   "            } else {"
   "                while ((y == x) && (5 == 5)) {" // 5
   "                    z = 0;" // 6
   "                }"
   "            }"
   "        } else {"
   "            y = 2;" // 7
   "        }"
   "    } else {"
   "        while (c == d) {" // 8
   "            if (x != y) then {" // 9
   "                y = 4;" // 10
   "            } else {"
   "                x = 1;" // 11
   "                while ((x + y == 3) || (2 == 3)) {" // 12
   "                    x = 0;" // 13
   "                }"
   "                x = 2;" // 14
   "            }"
   "            y = 3;" // 15
   "        }"
   "    }"
   "}";
