#include "node_assign_statement.h"

AssignStatementNode::AssignStatementNode(int stmt_no,
                                         std::shared_ptr<VariableNode> identifier,
                                         std::shared_ptr<ExpressionNode> expression)
    : StatementNode(stmt_no), m_identifier(identifier), m_expression(expression) {}

std::shared_ptr<VariableNode> AssignStatementNode::GetIdentifier() {
  return m_identifier;
}

std::shared_ptr<ExpressionNode> AssignStatementNode::GetExpression() {
  return m_expression;
}

StmtType AssignStatementNode::GetStatementType() {
  return StmtType::ASSIGN;
}

std::string AssignStatementNode::ToString(int level) {
  return StatementNode::ToString(level) + m_identifier->ToString(level) + " = " + m_expression->ToString(level) + ";\n";
}

bool AssignStatementNode::operator==(const StatementNode &other) const {
  const auto casted_other = dynamic_cast<const AssignStatementNode *>(&other);
  return m_stmt_no == casted_other->m_stmt_no && *m_identifier == *(casted_other->m_identifier)
      && *m_expression == *(casted_other->m_expression);
}

void AssignStatementNode::Process(Populator populator, std::vector<std::string> *visited) {
  std::string stmt_num = std::to_string(GetStatementNumber());
  std::string var_name = "";
  populator.PopulateStmt(stmt_num);
  var_name = m_identifier->GetIdentifier();
  populator.PopulateVars(var_name);
  for (std::string s : *visited) {
    populator.PopulateModifies(s, var_name);
  }
  populator.PopulateModifies(stmt_num, var_name);

  std::string rhs_expr = Process(populator, visited, stmt_num, 0, "");
  populator.AddPattern(stmt_num, var_name, rhs_expr);

  populator.PopulateAssign(stmt_num);
  populator.PopulateParentStar(stmt_num, *visited);
}

void AssignStatementNode::Process(Populator populator, std::vector<std::string> *visited, std::string stmt) {
  ExpressionType expr_type = m_expression->GetExpressionType();

  switch (expr_type) {
    case ExpressionType::CONSTANT: {
      std::shared_ptr<ConstantNode> constant = static_pointer_cast<ConstantNode>(m_expression);
      std::string name = constant->GetValue();
      populator.PopulateConst(name);
      break;
    }
    case ExpressionType::COMBINATION: {
      std::shared_ptr<CombinationExpressionNode> comb = static_pointer_cast<CombinationExpressionNode>(m_expression);
      std::shared_ptr<ExpressionNode> lhs = comb->GetLeftExpression();
      std::shared_ptr<ExpressionNode> rhs = comb->GetRightExpression();
      ArithmeticOperator op = comb->GetArithmeticOperator();
      Process(populator, visited, stmt);
      Process(populator, visited, stmt);
      break;
    }
    case ExpressionType::VARIABLE: {
      std::shared_ptr<VariableNode> var = static_pointer_cast<VariableNode>(m_expression);
      std::string var_name = var->GetIdentifier();
      for (std::string s : *visited) {
        populator.PopulateUses(s, var_name);
      }
      populator.PopulateUses(stmt, var_name);
      populator.PopulateVars(var_name);
      break;
    }
  }
}

std::string AssignStatementNode::Process(Populator populator, std::vector<std::string> *visited, std::string stmt_num, int direction, std::string pattern) {
  ExpressionType expr_type = m_expression->GetExpressionType();

  switch (expr_type) {
    case ExpressionType::CONSTANT: {
      std::shared_ptr<ConstantNode> constant = static_pointer_cast<ConstantNode>(m_expression);
      std::string name = constant->GetValue();
      if (direction == 1) {
        pattern = "(" + name;
      } else if (direction == 2) {
        pattern = name + ")";
      } else {
        pattern = name;
      }
      populator.PopulateConst(name);
      break;
    }
    case ExpressionType::COMBINATION: {
      std::shared_ptr<CombinationExpressionNode> comb = static_pointer_cast<CombinationExpressionNode>(m_expression);
      std::shared_ptr<ExpressionNode> lhs = comb->GetLeftExpression();
      std::shared_ptr<ExpressionNode> rhs = comb->GetRightExpression();
      ArithmeticOperator op = comb->GetArithmeticOperator();
      std::string op_label = comb->GetArithmeticOperatorLabel(op);

      pattern = Process(populator, visited, stmt_num, 1, pattern) + op_label + Process(populator, visited, stmt_num, 2, pattern);

      if (direction == 1) {
        pattern = "(" + pattern;
      } else if (direction == 2) {
        pattern = pattern + ")";
      }
      break;
    }
    case ExpressionType::VARIABLE: {
      std::shared_ptr<VariableNode> var = static_pointer_cast<VariableNode>(m_expression);
      std::string var_name = var->GetIdentifier();
      if (direction == 1) {
        pattern = "(" + var_name;
      } else if (direction == 2) {
        pattern = var_name + ")";
      } else {
        pattern = var_name;
      }
      for (std::string s : *visited) {
        populator.PopulateUses(s, var_name);
      }
      populator.PopulateUses(stmt_num, var_name);
      populator.PopulateVars(var_name);
      break;
    }
  }

  return pattern;
}