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

ExpressionType ExpressionNode::getExpressionType()
{
  return ExpressionType::NONE;
}

StmtType CallStatementNode::getStatementType()
{
  return StmtType::CALL;
}

StmtType WhileStatementNode::getStatementType()
{
  return StmtType::WHILE;
}

StmtType PrintStatementNode::getStatementType()
{
  return StmtType::PRINT;
}

//StmtType ErrorStatementNode::getStatementType()
//{
//  return StmtType::ERROR;
//}

StmtType IfStatementNode::getStatementType()
{
  return StmtType::IF;
}

StmtType AssignStatementNode::getStatementType()
{
  return StmtType::ASSIGN;
}

StmtType ReadStatementNode::getStatementType()
{
  return StmtType::READ;
}

std::shared_ptr<VariableNode> ReadStatementNode::getId()
{
  return id;
}

std::string ConstantNode::format(int _)
{
  return value;
}

ExpressionType ConstantNode::getExpressionType()
{
  return ExpressionType::CONSTANT;
}

std::string ConstantNode::getValue()
{
  return value;
}

ExpressionType CombinationExpressionNode::getExpressionType()
{
  return ExpressionType::COMBINATION;
}

void CombinationExpressionNode::setLeftExpression(std::shared_ptr<ExpressionNode> left)
{
  this->lhs = left;
}

std::shared_ptr<ExpressionNode> CombinationExpressionNode::getLHS()
{
  return lhs;
}

std::shared_ptr<ExpressionNode> CombinationExpressionNode::getRHS()
{
  return rhs;
}

Operation CombinationExpressionNode::getOperation()
{
  return op;
}

ConditionalType ConditionalExpressionNode::getConditionalType()
{
  return ConditionalType::NONE;
}

ExpressionType VariableNode::getExpressionType()
{
  return ExpressionType::VARIABLE;
}

ConditionalType BooleanExpressionNode::getConditionalType()
{
  return ConditionalType::BOOLEAN;
}

void BooleanExpressionNode::setLeft(std::shared_ptr<ConditionalExpressionNode> lhs)
{
  this->lhs = lhs;
}

std::shared_ptr<ConditionalExpressionNode> BooleanExpressionNode::getLHS()
{
  return lhs;
}

std::shared_ptr<ConditionalExpressionNode> BooleanExpressionNode::getRHS()
{
  return rhs;
}

ConditionalType NotExpressionNode::getConditionalType()
{
  return ConditionalType::NOT;
}

std::shared_ptr<ConditionalExpressionNode> NotExpressionNode::getExpr()
{
  return expr;
}

ConditionalType RelationalExpressionNode::getConditionalType()
{
  return ConditionalType::RELATIONAL;
}

std::shared_ptr<ExpressionNode> RelationalExpressionNode::getLHS()
{
  return lhs;
}

std::shared_ptr<ExpressionNode> RelationalExpressionNode::getRHS()
{
  return rhs;
}

std::string CombinationExpressionNode::format(int level)
{
  return "(" + lhs->format(level) + " " + getOperationLabel(op) + " " + rhs->format(level) + ")";
}

std::string BooleanExpressionNode::format(int level)
{
  return "(" + lhs->format(level) + (op == BooleanOperator::AND ? " && " : " || ") + rhs->format(level) + ")";
}

std::string NotExpressionNode::format(int level)
{
  return "(!" + expr->format(level) + ")";
}

std::string RelationalExpressionNode::format(int level)
{
  return "(" + lhs->format(level) + " " + getRelationalOpLabel(rop) + " " + rhs->format(level) + ")";
}

std::string StatementNode::getStatementLabel()
{
  if (line_number == 0)
  {
    return "   ";
  }
  else
  {
    std::string num = std::to_string(line_number);
    return std::string(3 - num.length(), ' ') + num;
  }
}

std::string StatementNode::format(int level)
{
  return this->getStatementLabel();
}

std::vector<std::shared_ptr<StatementNode>> StatementNode::getStatementList()
{
  return {};
}

StmtType StatementNode::getStatementType()
{
  return StmtType::NONE;
}

int StatementNode::getLineNumber()
{
  return line_number;
}

std::vector<std::shared_ptr<StatementNode>> StatementListNode::getStatements()
{
  return statements;
}

std::string StatementListNode::format(int level)
{
  std::string acc = "";
  for (std::shared_ptr<StatementNode> statement : statements)
  {
    acc += statement->format(level);
  }

  return acc;
}

std::string VariableNode::format(int _)
{
  return "$" + name;
}

std::string VariableNode::getName()
{
  return name;
}

std::string ErrorStatementNode::format(int _)
{
  return "ERROR;\n";
}

std::string ReadStatementNode::format(int level)
{
  return StatementNode::format(level) + "read " + id->format(level) + ";\n";
}

std::string PrintStatementNode::format(int level)
{
  return StatementNode::format(level) + "print " + id->format(level) + ";\n";
}

std::shared_ptr<VariableNode> PrintStatementNode::getId()
{
  return id;
}

std::string CallStatementNode::format(int level)
{
  return StatementNode::format(level) + "call " + procedureId->format(level) + ";\n";
}

std::shared_ptr<VariableNode> CallStatementNode::getProcId()
{
  return procedureId;
}

std::string WhileStatementNode::format(int level)
{
  std::string header = StatementNode::format(level);
  return header + "while (" + condition->format(level) + ") {\n" + block->format(level + 1) + header + "}\n";
}

std::vector<std::shared_ptr<StatementNode>> WhileStatementNode::getStatementList()
{
  return block->getStatements();
}

std::shared_ptr<StatementListNode> WhileStatementNode::getBody()
{
  return block;
}

std::shared_ptr<ConditionalExpressionNode> WhileStatementNode::getConditional()
{
  return condition;
}

std::string IfStatementNode::format(int level)
{
  std::string header = StatementNode::format(level);
  return header + "if (" + condition->format(level) + ") then {\n" + consequent->format(level + 1) + header +
      "} else {\n" + alternative->format(level + 1) + header + "}\n";
}

// return a list starting with if statements and ending with else statements
std::vector<std::shared_ptr<StatementNode>> IfStatementNode::getStatementList()
{
  std::vector<std::shared_ptr<StatementNode>> consequentStatements = consequent->getStatements();
  std::vector<std::shared_ptr<StatementNode>> alternativeStatements = alternative->getStatements();
  consequentStatements.insert(end(consequentStatements), begin(alternativeStatements), end(alternativeStatements));
  return consequentStatements;
}

// return a consequent statement list
std::shared_ptr<StatementListNode> IfStatementNode::getConsequent()
{
  return consequent;
}

// return a consequent statement list
std::shared_ptr<StatementListNode> IfStatementNode::getAlternative()
{
  return alternative;
}

std::shared_ptr<ConditionalExpressionNode> IfStatementNode::getConditional()
{
  return condition;
}

std::string AssignStatementNode::format(int level)
{
  return StatementNode::format(level) + id->format(level) + " = " + expression->format(level) + ";\n";
}

std::shared_ptr<VariableNode> AssignStatementNode::getId()
{
  return id;
}

std::shared_ptr<ExpressionNode> AssignStatementNode::getExpr()
{
  return expression;
}

std::string ProcedureNode::format(int level)
{
  return "procedure " + name + " {\n" + stmtList->format(level + 1) + "}\n";
}

std::shared_ptr<StatementListNode> ProcedureNode::getStatementList()
{
  return stmtList;
}

std::string ProcedureNode::getName()
{
  return name;
}

std::vector<std::shared_ptr<ProcedureNode>> ProgramNode::getProcedures()
{
  return procedures;
}

std::string ProgramNode::format()
{
  return this->format(0);
}

std::string ProgramNode::format(int level)
{
  std::string acc = "";
  for (std::shared_ptr<ProcedureNode> procedure : procedures)
  {
    acc += procedure->format(level) + "\n";
  }
  return acc;
}

