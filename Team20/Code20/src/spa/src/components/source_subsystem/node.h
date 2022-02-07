#ifndef NODE_H
#define NODE_H

#include <memory>
#include <string>
#include <unordered_set>
#include <vector>

class Node
{
 public:
  Node();
};

enum class ExpressionType
{
  CONSTANT,
  COMBINATION,
  IDENTIFIER,
  NONE, // Should not happen
};

class Expression : public Node
{
 public:
  Expression();
  virtual ExpressionType getExpressionType();
  std::string format(int level);
};

class Constant : public Expression
{
 private:
  std::string value;

 public:
  Constant(std::string value);
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

class CombinationExpression : public Expression
{
 private:
  Operation op;
  std::shared_ptr<Expression> lhs;
  std::shared_ptr<Expression> rhs;

 public:
  CombinationExpression(Operation op, std::shared_ptr<Expression> lhs, std::shared_ptr<Expression> rhs)
  {
    this->op = op;
    this->lhs = lhs;
    this->rhs = rhs;
  }

  CombinationExpression(Operation op, std::shared_ptr<Expression> right)
  {
    this->op = op;
    this->rhs = right;
    this->lhs = NULL;
  }

  void setLeftExpression(std::shared_ptr<Expression> left);
  ExpressionType getExpressionType();
  std::shared_ptr<Expression> getLHS();
  std::shared_ptr<Expression> getRHS();
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

class ConditionalExpression : public Node
{
 public:
  ConditionalExpression();
  virtual ConditionalType getConditionalType();
  std::string format(int level);
};

enum class BooleanOperator
{
  AND,
  OR
};

class BooleanExpression : public ConditionalExpression
{
 private:
  BooleanOperator op;
  std::shared_ptr<ConditionalExpression> lhs;
  std::shared_ptr<ConditionalExpression> rhs;

 public:
  BooleanExpression(BooleanOperator op, std::shared_ptr<ConditionalExpression> lhs, std::shared_ptr<ConditionalExpression> rhs)
  {
    this->op = op;
    this->lhs = lhs;
    this->rhs = rhs;
  }

  BooleanExpression(BooleanOperator op, std::shared_ptr<ConditionalExpression> rhs)
  {
    this->op = op;
    this->rhs = rhs;
    this->lhs = NULL;
  }

  void setLeft(std::shared_ptr<ConditionalExpression> lhs);
  ConditionalType getConditionalType();
  std::shared_ptr<ConditionalExpression> getLHS();
  std::shared_ptr<ConditionalExpression> getRHS();
  std::string format(int level);
};

class NotExpression : public ConditionalExpression
{
 private:
  std::shared_ptr<ConditionalExpression> expr;

 public:
  NotExpression(std::shared_ptr<ConditionalExpression> expr)
  {
    this->expr = expr;
  }

  std::shared_ptr<ConditionalExpression> getExpr();
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

class RelationalExpression : public ConditionalExpression
{
 private:
  RelationalOperation rop;
  std::shared_ptr<Expression> lhs;
  std::shared_ptr<Expression> rhs;

 public:
  RelationalExpression(RelationalOperation op, std::shared_ptr<Expression> lhs, std::shared_ptr<Expression> rhs)
  {
    this->rop = op;
    this->lhs = lhs;
    this->rhs = rhs;
  }

  ConditionalType getConditionalType();
  std::shared_ptr<Expression> getLHS();
  std::shared_ptr<Expression> getRHS();
  std::string format(int level);
};

enum class StatementType
{
  ERROR,
  WHILE,
  IF,
  READ,
  PRINT,
  CALL,
  ASSIGN,
  STATEMENT, // Used for Next* (meant to be AllStatement)
  NONE,      // Should not happen
};

class Statement : public Node
{
 private:
  int index = 0;

 protected:
  std::string getStatementLabel();

 public:
  Statement(int index)
  {
    this->index = index;
  }

  virtual std::vector<std::shared_ptr<Statement>> getStatementList();
  virtual StatementType getStatementType();
  int getIndex();
  std::string format(int level);
};

class StatementList : public Node
{
 private:
  std::vector<std::shared_ptr<Statement>> statements;

 public:
  StatementList()
  {
    statements = std::vector<std::shared_ptr<Statement>>();
  }

  StatementList(std::vector<std::shared_ptr<Statement>> statements)
  {
    this->statements = statements;
  }

  std::vector<std::shared_ptr<Statement>> getStatements();
  std::string format(int level);
};

class Identifier : public Expression
{
 private:
  std::string name;

 public:
  Identifier()
  {
    this->name = "";
  }
  Identifier(std::string name)
  {
    this->name = name;
  }

  std::string getName();
  std::string format(int _);
  ExpressionType getExpressionType();
};

class ErrorStatement : public Statement
{
 public:
  ErrorStatement(int index) : Statement(index)
  {
  }
  std::string format(int _);
  StatementType getStatementType();
};

class ReadStatement : public Statement
{
 private:
  std::shared_ptr<Identifier> id;

 public:
  ReadStatement(int index, std::shared_ptr<Identifier> id) : Statement(index)
  {
    this->id = id;
  }

  std::shared_ptr<Identifier> getId();
  std::string format(int level);
  StatementType getStatementType();
};

class PrintStatement : public Statement
{
 private:
  std::shared_ptr<Identifier> id;

 public:
  PrintStatement(int index, std::shared_ptr<Identifier> id) : Statement(index)
  {
    this->id = id;
  }

  std::shared_ptr<Identifier> getId();
  std::string format(int level);
  StatementType getStatementType();
};

class WhileStatement : public Statement
{
 private:
  std::shared_ptr<ConditionalExpression> condition;
  std::shared_ptr<StatementList> block;

 public:
  WhileStatement(int index, std::shared_ptr<ConditionalExpression> cond, std::shared_ptr<StatementList> block)
      : Statement(index)
  {
    this->condition = cond;
    this->block = block;
  }

  std::string format(int level);
  StatementType getStatementType();
  std::vector<std::shared_ptr<Statement>> getStatementList();
  std::shared_ptr<StatementList> getBody();
  std::shared_ptr<ConditionalExpression> getConditional();
};

class IfStatement : public Statement
{
 private:
  std::shared_ptr<ConditionalExpression> condition;
  std::shared_ptr<StatementList> consequent;
  std::shared_ptr<StatementList> alternative;

 public:
  IfStatement(int index, std::shared_ptr<ConditionalExpression> condition, std::shared_ptr<StatementList> consequent,
              std::shared_ptr<StatementList> alternative)
      : Statement(index)
  {
    this->condition = condition;
    this->consequent = consequent;
    this->alternative = alternative;
  }

  std::string format(int level);
  StatementType getStatementType();
  std::vector<std::shared_ptr<Statement>> getStatementList();
  std::shared_ptr<StatementList> getConsequent();
  std::shared_ptr<StatementList> getAlternative();
  std::shared_ptr<ConditionalExpression> getConditional();
};

class AssignStatement : public Statement
{
 private:
  std::shared_ptr<Identifier> id;
  std::shared_ptr<Expression> expression;

 public:
  AssignStatement(int index, std::shared_ptr<Identifier> id, std::shared_ptr<Expression> expr) : Statement(index)
  {
    this->id = id;
    this->expression = expr;
  }

  std::shared_ptr<Identifier> getId();
  std::shared_ptr<Expression> getExpr();
  std::string format(int level);
  StatementType getStatementType();
};

class Procedure : public Node
{
 private:
  std::string name;
  std::shared_ptr<StatementList> stmtList;

 public:
  Procedure(std::string name, std::shared_ptr<StatementList> stmtList)
  {
    this->name = name;
    this->stmtList = stmtList;
  }

  std::shared_ptr<StatementList> getStatementList();
  std::string getName();
  std::string format(int level);
};

class Program : public Node
{
 private:
  std::vector<std::shared_ptr<Procedure>> procedures;

 public:
  Program(std::vector<std::shared_ptr<Procedure>> procedures)
  {
    this->procedures = procedures;
  }

  std::vector<std::shared_ptr<Procedure>> getProcedures();
  std::string format();
  std::string format(int level);
};

#endif //NODE_H