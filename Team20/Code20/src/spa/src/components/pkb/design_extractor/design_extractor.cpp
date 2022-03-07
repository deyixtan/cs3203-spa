#include "design_extractor.h"
#include "components/pkb/pkb.h"

DesignExtractor::DesignExtractor(ProgramNode root_node, Populator populator) :
    root_node(root_node), populator(populator) {
  this->root_node = root_node;
  this->populator = populator;
}

void DesignExtractor::TraverseAst() {
  std::vector<std::shared_ptr<ProcedureNode>> proc_list = this->root_node.GetProcedures();
  for (auto &proc : proc_list) {
    std::shared_ptr<StatementListNode> stmtLst = proc->GetStatementList();
    std::vector<std::string> visited;
    populator.PopulateProc(proc->GetIdentifier());
    ProcNodeHandler(visited, proc, stmtLst);
  }
}

void DesignExtractor::ProcNodeHandler(std::vector<std::string> visited, std::shared_ptr<ProcedureNode> proc, std::shared_ptr<StatementListNode> stmtLst) {
  std::vector<std::shared_ptr<StatementNode>> stmts = stmtLst->GetStatements();

  for (int i = 0; i < stmts.size(); ++i) {
    std::shared_ptr<StatementNode> stmt = stmts[i];
    std::string stmt_num = std::to_string(stmt->GetStatementNumber());
    StmtType stmt_type = stmt->GetStatementType();
    std::string var_name = "";

    if (stmt != stmts.back()) {
      int curr_stmt = stmt->GetStatementNumber();
      int next_stmt = stmts[i + 1]->GetStatementNumber();
      populator.PopulateFollows(std::to_string(curr_stmt), std::to_string(next_stmt));
      populator.PopulateFollowsStar(stmts, i);
    }

    stmt->Process(populator, &visited);

    switch(stmt_type) {
      case WHILE: {
        std::shared_ptr<WhileStatementNode> while_stmt = static_pointer_cast<WhileStatementNode>(stmt);
        std::shared_ptr<StatementListNode> while_block = while_stmt->GetStatementList();
        ProcNodeHandler(visited, proc, while_block);
        break;
      }
      case IF: {
        std::shared_ptr<IfStatementNode> if_stmt = static_pointer_cast<IfStatementNode>(stmt);
        std::shared_ptr<StatementListNode> if_block = if_stmt->GetIfStatementList();
        std::shared_ptr<StatementListNode> else_block = if_stmt->GetElseStatementList();
        ProcNodeHandler(visited, proc, if_block);
        ProcNodeHandler(visited, proc, else_block);
        break;
      }
    }
  }
}

void DesignExtractor::ExprNodeHandler(std::vector<std::string> visited, std::string stmt, std::shared_ptr<ExpressionNode> expr) {
  ExpressionType expr_type = expr->GetExpressionType();

  switch (expr_type) {
    case ExpressionType::CONSTANT: {
      std::shared_ptr<ConstantNode> constant = static_pointer_cast<ConstantNode>(expr);
      std::string name = constant->GetValue();
      populator.PopulateConst(name);
      break;
    }
    case ExpressionType::COMBINATION: {
      std::shared_ptr<CombinationExpressionNode> comb = static_pointer_cast<CombinationExpressionNode>(expr);
      std::shared_ptr<ExpressionNode> lhs = comb->GetLeftExpression();
      std::shared_ptr<ExpressionNode> rhs = comb->GetRightExpression();
      ArithmeticOperator op = comb->GetArithmeticOperator();
      ExprNodeHandler(visited, stmt, lhs);
      ExprNodeHandler(visited, stmt, rhs);
      break;
    }
    case ExpressionType::VARIABLE: {
      std::shared_ptr<VariableNode> var = static_pointer_cast<VariableNode>(expr);
      std::string var_name = var->GetIdentifier();
      for (std::string s : visited) {
        populator.PopulateUses(s, var_name);
      }
      populator.PopulateUses(stmt, var_name);
      populator.PopulateVars(var_name);
      break;
    }
  }
}

std::string DesignExtractor::ExprNodeHandler(std::vector<std::string> visited, std::string stmt_num, std::shared_ptr<ExpressionNode> expr, int direction, std::string pattern) {
  ExpressionType expr_type = expr->GetExpressionType();

  switch (expr_type) {
    case ExpressionType::CONSTANT: {
      std::shared_ptr<ConstantNode> constant = static_pointer_cast<ConstantNode>(expr);
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
      std::shared_ptr<CombinationExpressionNode> comb = static_pointer_cast<CombinationExpressionNode>(expr);
      std::shared_ptr<ExpressionNode> lhs = comb->GetLeftExpression();
      std::shared_ptr<ExpressionNode> rhs = comb->GetRightExpression();
      ArithmeticOperator op = comb->GetArithmeticOperator();
      std::string op_label = comb->GetArithmeticOperatorLabel(op);

      pattern = ExprNodeHandler(visited, stmt_num, lhs, 1, pattern) + op_label + ExprNodeHandler(visited, stmt_num, rhs, 2, pattern);

      if (direction == 1) {
        pattern = "(" + pattern;
      } else if (direction == 2) {
        pattern = pattern + ")";
      }
      break;
    }
    case ExpressionType::VARIABLE: {
      std::shared_ptr<VariableNode> var = static_pointer_cast<VariableNode>(expr);
      std::string var_name = var->GetIdentifier();
      if (direction == 1) {
        pattern = "(" + var_name;
      } else if (direction == 2) {
        pattern = var_name + ")";
      } else {
        pattern = var_name;
      }
      for (std::string s : visited) {
        populator.PopulateUses(s, var_name);
      }
      populator.PopulateUses(stmt_num, var_name);
      populator.PopulateVars(var_name);
      break;
    }
  }

  return pattern;
}
