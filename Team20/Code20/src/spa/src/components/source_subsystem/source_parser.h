#ifndef SOURCE_PARSER_H
#define SOURCE_PARSER_H

#include <vector>

#include "components/source_subsystem/tnode.h"
#include "components/source_subsystem/source_token.h"

class SourceParser {
 private:
  std::vector<SourceToken *> tokens_ptr;
  int cursor;
  int line_number;

 public:
  SourceParser(std::vector<SourceToken *> tokens_ptr);
  SourceToken *FetchCurrentToken();
  void IncrementCursor();
  bool AreTokensProcessed();
  SourceToken *ConsumeToken(TokenType type);
  ProgramNode *ParseProgram();
  ProcedureNode *ParseProcedure();
  StatementListNode *ParseStatementList();
  StatementNode *ParseStatement();
  ReadStatementNode *ParseReadStatement();
  PrintStatementNode *ParsePrintStatement();
  WhileStatementNode *ParseWhileStatement();
  IfStatementNode *ParseIfStatement();
  AssignStatementNode *ParseAssignStatement();
};

#endif //SOURCE_PARSER_H