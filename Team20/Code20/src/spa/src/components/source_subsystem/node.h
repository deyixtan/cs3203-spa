#ifndef NODE_H
#define NODE_H

#include <memory>
#include <string>
#include <unordered_set>
#include <vector>
#include "components/pkb/pkb.h"

class Node
{
 public:
  virtual std::string format(int level) {
    return "";
  }
};

enum class ExpressionType
{
  CONSTANT,
  COMBINATION,
  VARIABLE,
  NONE, // Should not happen
};

class ExpressionNode : public Node
{
 public:
  virtual ExpressionType getExpressionType();
};

class ConstantNode : public ExpressionNode
{
 private:
  std::string value;

 public:
  ConstantNode(std::string value) {
    this->value = value;
  }
  std::string getValue();
  std::string format(int lvl);
  ExpressionType getExpressionType();
};

enum class Operation
{
  PLUS,
  MINUS,
  DIVIDE,
  MULTIPLY,
  MOD,
};

class CombinationExpressionNode : public ExpressionNode
{
 private:
  Operation op;
  std::shared_ptr<ExpressionNode> lhs;
  std::shared_ptr<ExpressionNode> rhs;

 public:
  CombinationExpressionNode(Operation op, std::shared_ptr<ExpressionNode> lhs, std::shared_ptr<ExpressionNode> rhs)
  {
    this->op = op;
    this->lhs = lhs;
    this->rhs = rhs;
  }

  CombinationExpressionNode(Operation op, std::shared_ptr<ExpressionNode> right)
  {
    this->op = op;
    this->rhs = right;
    this->lhs = NULL;
  }

  void setLeftExpression(std::shared_ptr<ExpressionNode> left);
  ExpressionType getExpressionType();
  std::shared_ptr<ExpressionNode> getLHS();
  std::shared_ptr<ExpressionNode> getRHS();
  Operation getOperation();
  std::string format(int level);
};

enum class ConditionalType
{
  BOOLEAN,
  NOT,
  RELATIONAL,
  NONE,
};

class ConditionalExpressionNode : public Node
{
 public:
  virtual ConditionalType getConditionalType();
};

enum class BooleanOperator
{
  AND,
  OR
};

class BooleanExpressionNode : public ConditionalExpressionNode
{
 private:
  BooleanOperator op;
  std::shared_ptr<ConditionalExpressionNode> lhs;
  std::shared_ptr<ConditionalExpressionNode> rhs;

 public:
  BooleanExpressionNode(BooleanOperator op, std::shared_ptr<ConditionalExpressionNode> lhs, std::shared_ptr<ConditionalExpressionNode> rhs)
  {
    this->op = op;
    this->lhs = lhs;
    this->rhs = rhs;
  }

  BooleanExpressionNode(BooleanOperator op, std::shared_ptr<ConditionalExpressionNode> rhs)
  {
    this->op = op;
    this->rhs = rhs;
    this->lhs = NULL;
  }

  void setLeft(std::shared_ptr<ConditionalExpressionNode> lhs);
  ConditionalType getConditionalType();
  std::shared_ptr<ConditionalExpressionNode> getLHS();
  std::shared_ptr<ConditionalExpressionNode> getRHS();
  std::string format(int level);
};

class NotExpressionNode : public ConditionalExpressionNode
{
 private:
  std::shared_ptr<ConditionalExpressionNode> expr;

 public:
  NotExpressionNode(std::shared_ptr<ConditionalExpressionNode> expr)
  {
    this->expr = expr;
  }

  std::shared_ptr<ConditionalExpressionNode> getExpr();
  std::string format(int level);
  ConditionalType getConditionalType();
};

enum class RelationalOperation
{
  LESS_THAN,
  LESS_THAN_EQUALS,
  GREATER_THAN,
  GREATER_THAN_EQUALS,
  EQUALS,
  NOT_EQUALS
};

class RelationalExpressionNode : public ConditionalExpressionNode
{
 private:
  RelationalOperation rop;
  std::shared_ptr<ExpressionNode> lhs;
  std::shared_ptr<ExpressionNode> rhs;

 public:
  RelationalExpressionNode(RelationalOperation op, std::shared_ptr<ExpressionNode> lhs, std::shared_ptr<ExpressionNode> rhs)
  {
    this->rop = op;
    this->lhs = lhs;
    this->rhs = rhs;
  }

  ConditionalType getConditionalType();
  std::shared_ptr<ExpressionNode> getLHS();
  std::shared_ptr<ExpressionNode> getRHS();
  std::string format(int level);
};

class StatementNode : public Node
{
 private:
  int line_number = 0;

 protected:
  std::string getStatementLabel();

 public:
  StatementNode(int line)
  {
    this->line_number = line;
  }

  virtual std::vector<std::shared_ptr<StatementNode>> getStatementList();
  virtual StmtType getStatementType();
  int getLineNumber();
  std::string format(int level);
};

class StatementListNode : public Node
{
 private:
  std::vector<std::shared_ptr<StatementNode>> statements;

 public:
  StatementListNode()
  {
    statements = std::vector<std::shared_ptr<StatementNode>>();
  }

  StatementListNode(std::vector<std::shared_ptr<StatementNode>> statements)
  {
    this->statements = statements;
  }

  std::vector<std::shared_ptr<StatementNode>> getStatements();
  std::string format(int level);
};

class VariableNode : public ExpressionNode
{
 private:
  std::string name;

 public:
  VariableNode()
  {
    this->name = "";
  }
  VariableNode(std::string name)
  {
    this->name = name;
  }

  std::string getName();
  std::string format(int _);
  ExpressionType getExpressionType();
};

class ErrorStatementNode : public StatementNode
{
 public:
  ErrorStatementNode(int line) : StatementNode(line)
  {
  }
  std::string format(int _);
  StmtType getStatementType();
};

class ReadStatementNode : public StatementNode
{
 private:
  std::shared_ptr<VariableNode> id;

 public:
  ReadStatementNode(int line, std::shared_ptr<VariableNode> id) : StatementNode(line)
  {
    this->id = id;
  }

  std::shared_ptr<VariableNode> getId();
  std::string format(int level);
  StmtType getStatementType();
};

class PrintStatementNode : public StatementNode
{
 private:
  std::shared_ptr<VariableNode> id;

 public:
  PrintStatementNode(int line, std::shared_ptr<VariableNode> id) : StatementNode(line)
  {
    this->id = id;
  }

  std::shared_ptr<VariableNode> getId();
  std::string format(int level);
  StmtType getStatementType();
};

class CallStatementNode : public StatementNode
{
 private:
  std::shared_ptr<VariableNode> procedureId;

 public:
  CallStatementNode(int line, std::shared_ptr<VariableNode> procId) : StatementNode(line)
  {
    this->procedureId = procId;
  }

  std::shared_ptr<VariableNode> getProcId();
  std::string format(int level);
  StmtType getStatementType();
};

class WhileStatementNode : public StatementNode
{
 private:
  std::shared_ptr<ConditionalExpressionNode> condition;
  std::shared_ptr<StatementListNode> block;

 public:
  WhileStatementNode(int line, std::shared_ptr<ConditionalExpressionNode> cond, std::shared_ptr<StatementListNode> block)
      : StatementNode(line)
  {
    this->condition = cond;
    this->block = block;
  }

  std::string format(int level);
  StmtType getStatementType();
  std::vector<std::shared_ptr<StatementNode>> getStatementList();
  std::shared_ptr<StatementListNode> getBody();
  std::shared_ptr<ConditionalExpressionNode> getConditional();
};

class IfStatementNode : public StatementNode
{
 private:
  std::shared_ptr<ConditionalExpressionNode> condition;
  std::shared_ptr<StatementListNode> consequent;
  std::shared_ptr<StatementListNode> alternative;

 public:
  IfStatementNode(int line, std::shared_ptr<ConditionalExpressionNode> condition, std::shared_ptr<StatementListNode> consequent,
              std::shared_ptr<StatementListNode> alternative)
      : StatementNode(line)
  {
    this->condition = condition;
    this->consequent = consequent;
    this->alternative = alternative;
  }

  std::string format(int level);
  StmtType getStatementType();
  std::vector<std::shared_ptr<StatementNode>> getStatementList();
  std::shared_ptr<StatementListNode> getConsequent();
  std::shared_ptr<StatementListNode> getAlternative();
  std::shared_ptr<ConditionalExpressionNode> getConditional();
};

class AssignStatementNode : public StatementNode
{
 private:
  std::shared_ptr<VariableNode> id;
  std::shared_ptr<ExpressionNode> expression;

 public:
  AssignStatementNode(int line, std::shared_ptr<VariableNode> id, std::shared_ptr<ExpressionNode> expr) : StatementNode(line)
  {
    this->id = id;
    this->expression = expr;
  }

  std::shared_ptr<VariableNode> getId();
  std::shared_ptr<ExpressionNode> getExpr();
  std::string format(int level);
  StmtType getStatementType();
};

class ProcedureNode : public Node
{
 private:
  std::string name;
  std::shared_ptr<StatementListNode> stmtList;

 public:
  ProcedureNode(std::string name, std::shared_ptr<StatementListNode> stmtList)
  {
    this->name = name;
    this->stmtList = stmtList;
  }

  std::shared_ptr<StatementListNode> getStatementList();
  std::string getName();
  std::string format(int level);
};

class ProgramNode : public Node
{
 private:
  std::vector<std::shared_ptr<ProcedureNode>> procedures;

 public:
  ProgramNode(std::vector<std::shared_ptr<ProcedureNode>> procedures)
  {
    this->procedures = procedures;
  }

  std::vector<std::shared_ptr<ProcedureNode>> getProcedures();
  std::string format();
  std::string format(int level);
};

#endif //NODE_H