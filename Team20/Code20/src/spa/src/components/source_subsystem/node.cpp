#include "node.h"

std::string getOperationLabel(Operation op)
{
  switch (op)
  {
    case Operation::PLUS:
      return "+";
    case Operation::MINUS:
      return "-";
    case Operation::DIVIDE:
      return "/";
    case Operation::MULTIPLY:
      return "*";
    case Operation::MOD:
      return "%";
    default:
      return "";
  }
}

std::string getRelationalOpLabel(RelationalOperation op)
{
  switch (op)
  {
    case RelationalOperation::LESS_THAN:
      return "<";
    case RelationalOperation::LESS_THAN_EQUALS:
      return "<=";
    case RelationalOperation::GREATER_THAN:
      return ">";
    case RelationalOperation::GREATER_THAN_EQUALS:
      return ">=";
    case RelationalOperation::EQUALS:
      return "==";
    case RelationalOperation::NOT_EQUALS:
      return "!=";
    default:
      return "";
  }
}

ExpressionType Expression::getExpressionType()
{
  return ExpressionType::NONE;
}

StatementType CallStatement::getStatementType()
{
  return StatementType::CALL;
}

StatementType WhileStatement::getStatementType()
{
  return StatementType::WHILE;
}

StatementType PrintStatement::getStatementType()
{
  return StatementType::PRINT;
}

StatementType ErrorStatement::getStatementType()
{
  return StatementType::ERROR;
}

StatementType IfStatement::getStatementType()
{
  return StatementType::IF;
}

StatementType AssignStatement::getStatementType()
{
  return StatementType::ASSIGN;
}

StatementType ReadStatement::getStatementType()
{
  return StatementType::READ;
}

std::shared_ptr<Identifier> ReadStatement::getId()
{
  return id;
}

std::string Constant::format(int _)
{
  return value;
}

ExpressionType Constant::getExpressionType()
{
  return ExpressionType::CONSTANT;
}

std::string Constant::getValue()
{
  return value;
}

ExpressionType CombinationExpression::getExpressionType()
{
  return ExpressionType::COMBINATION;
}

void CombinationExpression::setLeftExpression(std::shared_ptr<Expression> left)
{
  this->lhs = left;
}

std::shared_ptr<Expression> CombinationExpression::getLHS()
{
  return lhs;
}

std::shared_ptr<Expression> CombinationExpression::getRHS()
{
  return rhs;
}

Operation CombinationExpression::getOperation()
{
  return op;
}

ConditionalType ConditionalExpression::getConditionalType()
{
  return ConditionalType::NONE;
}

ExpressionType Identifier::getExpressionType()
{
  return ExpressionType::IDENTIFIER;
}

ConditionalType BooleanExpression::getConditionalType()
{
  return ConditionalType::BOOLEAN;
}

void BooleanExpression::setLeft(std::shared_ptr<ConditionalExpression> lhs)
{
  this->lhs = lhs;
}

std::shared_ptr<ConditionalExpression> BooleanExpression::getLHS()
{
  return lhs;
}

std::shared_ptr<ConditionalExpression> BooleanExpression::getRHS()
{
  return rhs;
}

ConditionalType NotExpression::getConditionalType()
{
  return ConditionalType::NOT;
}

std::shared_ptr<ConditionalExpression> NotExpression::getExpr()
{
  return expr;
}

ConditionalType RelationalExpression::getConditionalType()
{
  return ConditionalType::RELATIONAL;
}

std::shared_ptr<Expression> RelationalExpression::getLHS()
{
  return lhs;
}

std::shared_ptr<Expression> RelationalExpression::getRHS()
{
  return rhs;
}

std::string CombinationExpression::format(int level)
{
  return "(" + lhs->format(level) + " " + getOperationLabel(op) + " " + rhs->format(level) + ")";
}

std::string BooleanExpression::format(int level)
{
  return "(" + lhs->format(level) + (op == BooleanOperator::AND ? " && " : " || ") + rhs->format(level) + ")";
}

std::string NotExpression::format(int level)
{
  return "(!" + expr->format(level) + ")";
}

std::string RelationalExpression::format(int level)
{
  return "(" + lhs->format(level) + " " + getRelationalOpLabel(rop) + " " + rhs->format(level) + ")";
}

std::string Statement::getStatementLabel()
{
  if (index == 0)
  {
    return "   ";
  }
  else
  {
    std::string num = std::to_string(index);
    return std::string(3 - num.length(), ' ') + num;
  }
}

std::string Statement::format(int level)
{
  return this->getStatementLabel();
}

std::vector<std::shared_ptr<Statement>> Statement::getStatementList()
{
  return {};
}

StatementType Statement::getStatementType()
{
  return StatementType::NONE;
}

int Statement::getIndex()
{
  return index;
}

std::vector<std::shared_ptr<Statement>> StatementList::getStatements()
{
  return statements;
}

std::string StatementList::format(int level)
{
  std::string acc = "";
  for (std::shared_ptr<Statement> statement : statements)
  {
    acc += statement->format(level);
  }

  return acc;
}

std::string Identifier::format(int _)
{
  return "$" + name;
}

std::string Identifier::getName()
{
  return name;
}

std::string ErrorStatement::format(int _)
{
  return "ERROR;\n";
}

std::string ReadStatement::format(int level)
{
  return Statement::format(level) + "read " + id->format(level) + ";\n";
}

std::string PrintStatement::format(int level)
{
  return Statement::format(level) + "print " + id->format(level) + ";\n";
}

std::shared_ptr<Identifier> PrintStatement::getId()
{
  return id;
}

std::string CallStatement::format(int level)
{
  return Statement::format(level) + "call " + procedureId->format(level) + ";\n";
}

std::shared_ptr<Identifier> CallStatement::getProcId()
{
  return procedureId;
}

std::string WhileStatement::format(int level)
{
  std::string header = Statement::format(level);
  return header + "while (" + condition->format(level) + ") {\n" + block->format(level + 1) + header + "}\n";
}

std::vector<std::shared_ptr<Statement>> WhileStatement::getStatementList()
{
  return block->getStatements();
}

std::shared_ptr<StatementList> WhileStatement::getBody()
{
  return block;
}

std::shared_ptr<ConditionalExpression> WhileStatement::getConditional()
{
  return condition;
}

std::string IfStatement::format(int level)
{
  std::string header = Statement::format(level);
  return header + "if (" + condition->format(level) + ") then {\n" + consequent->format(level + 1) + header +
      "} else {\n" + alternative->format(level + 1) + header + "}\n";
}

// return a list starting with if statements and ending with else statements
std::vector<std::shared_ptr<Statement>> IfStatement::getStatementList()
{
  std::vector<std::shared_ptr<Statement>> consequentStatements = consequent->getStatements();
  std::vector<std::shared_ptr<Statement>> alternativeStatements = alternative->getStatements();
  consequentStatements.insert(end(consequentStatements), begin(alternativeStatements), end(alternativeStatements));
  return consequentStatements;
}

// return a consequent statement list
std::shared_ptr<StatementList> IfStatement::getConsequent()
{
  return consequent;
}

// return a consequent statement list
std::shared_ptr<StatementList> IfStatement::getAlternative()
{
  return alternative;
}

std::shared_ptr<ConditionalExpression> IfStatement::getConditional()
{
  return condition;
}

std::string AssignStatement::format(int level)
{
  return Statement::format(level) + id->format(level) + " = " + expression->format(level) + ";\n";
}

std::shared_ptr<Identifier> AssignStatement::getId()
{
  return id;
}

std::shared_ptr<Expression> AssignStatement::getExpr()
{
  return expression;
}

std::string Procedure::format(int level)
{
  return "procedure " + name + " {\n" + stmtList->format(level + 1) + "}\n";
}

std::shared_ptr<StatementList> Procedure::getStatementList()
{
  return stmtList;
}

std::string Procedure::getName()
{
  return name;
}

std::vector<std::shared_ptr<Procedure>> Program::getProcedures()
{
  return procedures;
}

std::string Program::format()
{
  return this->format(0);
}

std::string Program::format(int level)
{
  std::string acc = "";
  for (std::shared_ptr<Procedure> procedure : procedures)
  {
    acc += procedure->format(level) + "\n";
  }
  return acc;
}

