#ifndef COMMON_DECLARATIONS_H
#define COMMON_DECLARATIONS_H

#include <algorithm>
#include <iostream>
#include <memory>
#include <queue>
#include <stack>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

enum StmtType {
  STMT,
  READ,
  PRINT,
  WHILE,
  IF,
  ASSIGN,
  VARS,
  CONSTS,
  PROC,
  CALL,
  COUNT = CALL + 1 // get max value of StmtType
};

// general
typedef std::exception Exception;
typedef std::string String;
typedef std::vector<String> StringStream;
typedef std::queue<String> StringQueue;
typedef std::unordered_set<String> StringSet;
typedef std::unordered_map<std::string, int> StringToIntMap;
typedef std::unordered_map<String, StringSet> StringToStringSetMap;

#endif //COMMON_DECLARATIONS_H
