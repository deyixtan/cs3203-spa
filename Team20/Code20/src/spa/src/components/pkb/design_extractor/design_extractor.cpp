#include "design_extractor.h"
#include "components/pkb/pkb.h"

DesignExtractor::DesignExtractor(ProgramNode root_node, PKB *pkb) :
    root_node(root_node), pkb(pkb) {
  this->root_node = root_node;
  this->pkb = pkb;
}

void DesignExtractor::TraverseAst() {
  std::vector<std::shared_ptr<ProcedureNode>> proc_list = this->root_node.GetProcedures();
  for (auto &proc : proc_list) {
    std::shared_ptr<StatementListNode> stmtLst = proc->GetStatementList();
    std::vector<std::string> visited;
    PopulateProc(proc->GetIdentifier());
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
      PopulateFollows(std::to_string(curr_stmt), std::to_string(next_stmt));
      PopulateFollowsStar(stmts, i);
    }

    PopulateParentStar(stmt_num, visited);

    switch(stmt_type) {
      case PROC:
        PopulateProc(proc->GetIdentifier());
        break;
      case STMT:
        PopulateStmt(stmt_num);
        break;
      case READ: {
        PopulateStmt(stmt_num);
        std::shared_ptr<ReadStatementNode> read_stmt = static_pointer_cast<ReadStatementNode>(stmt);
        var_name = read_stmt->GetIdentifier()->GetIdentifier();
        PopulateVars(var_name);
        PopulateRead(stmt_num);
        for (std::string s : visited) {
          PopulateModifies(s, var_name);
        }
        PopulateModifies(stmt_num, var_name);
        break;
      }
      case PRINT: {
        PopulateStmt(stmt_num);
        std::shared_ptr<PrintStatementNode> print_stmt = static_pointer_cast<PrintStatementNode>(stmt);
        var_name = print_stmt->GetIdentifier()->GetIdentifier();
        PopulateVars(var_name);
        PopulatePrint(stmt_num);
        for (std::string s : visited) {
          PopulateUses(s, var_name);
        }
        PopulateUses(stmt_num, var_name);
        break;
      }
      case ASSIGN: {
        PopulateStmt(stmt_num);
        std::shared_ptr<AssignStatementNode> assign_stmt = static_pointer_cast<AssignStatementNode>(stmt);
        var_name = assign_stmt->GetIdentifier()->GetIdentifier();
        PopulateVars(var_name);
        for (std::string s : visited) {
          PopulateModifies(s, var_name);
        }
        PopulateModifies(stmt_num, var_name);
        std::shared_ptr<ExpressionNode> expr = assign_stmt->GetExpression();

        std::string rhs_expr = ExprNodeHandler(visited, stmt_num, expr, 0, "");
        //rhs_expr.erase(0, 1);
        pkb->AddPattern(stmt_num, var_name, rhs_expr);

        PopulateAssign(stmt_num);
        break;
      }
      case WHILE: {
        PopulateStmt(stmt_num);
        std::shared_ptr<WhileStatementNode> while_stmt = static_pointer_cast<WhileStatementNode>(stmt);
        std::shared_ptr<ConditionalExpressionNode> while_stmt_cond = while_stmt->GetCondition();
        std::string while_stmt_num = std::to_string(while_stmt->GetStatementNumber());
        visited.push_back(while_stmt_num);

        CondExprNodeHandler(visited, stmt_num, while_stmt_cond);

        std::shared_ptr<StatementListNode> while_block = while_stmt->GetStatementList();
        std::vector<std::shared_ptr<StatementNode>> while_stmts = while_block->GetStatements();

        for (int j = 0; j < while_stmts.size(); ++j) {
          int curr = while_stmts[j]->GetStatementNumber();
          PopulateParent(stmt_num, std::to_string(curr));
        }

        ProcNodeHandler(visited, proc, while_block);
        PopulateWhile(stmt_num);
        visited.clear();
        break;
      }
      case IF: {
        PopulateStmt(stmt_num);
        std::shared_ptr<IfStatementNode> if_stmt = static_pointer_cast<IfStatementNode>(stmt);
        std::shared_ptr<ConditionalExpressionNode> if_stmt_cond = if_stmt->GetCondition();
        std::string if_stmt_num = std::to_string(if_stmt->GetStatementNumber());
        visited.push_back(if_stmt_num);

        CondExprNodeHandler(visited, stmt_num, if_stmt_cond);

        std::shared_ptr<StatementListNode> if_block = if_stmt->GetIfStatementList();
        std::vector<std::shared_ptr<StatementNode>> if_stmts = if_block->GetStatements();
        std::shared_ptr<StatementListNode> else_block = if_stmt->GetElseStatementList();
        std::vector<std::shared_ptr<StatementNode>> else_stmts = else_block->GetStatements();

        for (int j = 0; j < if_stmts.size(); ++j) {
          int curr = if_stmts[j]->GetStatementNumber();
          PopulateParent(stmt_num, std::to_string(curr));
        }

        for (int j = 0; j < else_stmts.size(); ++j) {
          int curr = else_stmts[j]->GetStatementNumber();
          PopulateParent(stmt_num, std::to_string(curr));
        }

        ProcNodeHandler(visited, proc, if_block);
        ProcNodeHandler(visited, proc, else_block);
        PopulateIf(stmt_num);
        visited.clear();
        break;
      }
    }
  }
}

void DesignExtractor::CondExprNodeHandler(std::vector<std::string> visited, std::string stmt, std::shared_ptr<ConditionalExpressionNode> if_stmt_cond) {
  ConditionalType cond = if_stmt_cond->GetConditionalType();

  switch(cond) {
    case ConditionalType::BOOLEAN: {
      std::shared_ptr<BooleanExpressionNode> bool_node = static_pointer_cast<BooleanExpressionNode>(if_stmt_cond);
      std::shared_ptr<ConditionalExpressionNode> left = bool_node->GetLeftExpression();
      std::shared_ptr<ConditionalExpressionNode> right = bool_node->GetRightExpression();

      CondExprNodeHandler(visited, stmt, left);
      CondExprNodeHandler(visited, stmt, right);
      break;
    }
    case ConditionalType::NOT: {
      std::shared_ptr<NotExpressionNode> not_node = static_pointer_cast<NotExpressionNode>(if_stmt_cond);
      std::shared_ptr<ConditionalExpressionNode> expr = not_node->GetExpression();
      CondExprNodeHandler(visited, stmt, expr);
      break;
    }
    case ConditionalType::RELATIONAL: {
      std::shared_ptr<RelationalExpressionNode> rel_node = static_pointer_cast<RelationalExpressionNode>(if_stmt_cond);
      std::shared_ptr<ExpressionNode> left = rel_node->GetLeftExpression();
      std::shared_ptr<ExpressionNode> right = rel_node->GetRightExpression();

      ExprNodeHandler(visited, stmt, left);
      ExprNodeHandler(visited, stmt, right);
      break;
    }
  }
}

void DesignExtractor::ExprNodeHandler(std::vector<std::string> visited, std::string stmt, std::shared_ptr<ExpressionNode> expr) {
  ExpressionType expr_type = expr->GetExpressionType();

  switch (expr_type) {
    case ExpressionType::CONSTANT: {
      std::shared_ptr<ConstantNode> constant = static_pointer_cast<ConstantNode>(expr);
      std::string name = constant->GetValue();
      PopulateConst(name);
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
        PopulateUses(s, var_name);
      }
      PopulateUses(stmt, var_name);
      PopulateVars(var_name);
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
      }
      PopulateConst(name);
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
      }
      for (std::string s : visited) {
        PopulateUses(s, var_name);
      }
      PopulateUses(stmt_num, var_name);
      PopulateVars(var_name);
      break;
    }
  }

  return pattern;
}

void DesignExtractor::PopulateParent(std::string stmt1, std::string stmt2) {
  this->pkb->AddParentStmt(stmt1, stmt2);
}

void DesignExtractor::PopulateParentStar(std::string stmt, std::vector<std::string> visited) {
  this->pkb->AddParentStarStmt(stmt, visited);
}

void DesignExtractor::PopulateFollows(std::string stmt1, std::string stmt2) {
  this->pkb->AddFollowStmt(stmt1, stmt2);
}

void DesignExtractor::PopulateFollowsStar(std::vector<std::shared_ptr<StatementNode>> stmt_lst, int index) {
  int curr_stmt = stmt_lst[index]->GetStatementNumber();

  for (int i = index + 1; i < stmt_lst.size(); ++i) {
    int next_stmt = stmt_lst[i]->GetStatementNumber();
    this->pkb->AddFollowStarStmt(std::to_string(curr_stmt), std::to_string(next_stmt));
  }
}

void DesignExtractor::PopulateUses(std::string stmt, std::string var) {
  this->pkb->AddUsageStmtVar(stmt, var);
}

void DesignExtractor::PopulateModifies(std::string stmt, std::string var) {
  this->pkb->AddModifyStmtVar(stmt, var);
}

void DesignExtractor::PopulateProc(std::string name) {
  this->pkb->AddStmt(name, PROC);
}

void DesignExtractor::PopulateAssign(std::string stmt) {
  this->pkb->AddStmt(stmt, ASSIGN);
}

void DesignExtractor::PopulateStmt(std::string stmt) {
  this->pkb->AddStmt(stmt, STMT);
}

void DesignExtractor::PopulateRead(std::string stmt) {
  this->pkb->AddStmt(stmt, READ);
}

void DesignExtractor::PopulatePrint(std::string stmt) {
  this->pkb->AddStmt(stmt, PRINT);
}

void DesignExtractor::PopulateVars(std::string var) {
  this->pkb->AddStmt(var, VARS);
}

void DesignExtractor::PopulateWhile(std::string stmt) {
  this->pkb->AddStmt(stmt, WHILE);
}

void DesignExtractor::PopulateIf(std::string stmt) {
  this->pkb->AddStmt(stmt, IF);
}

void DesignExtractor::PopulateConst(std::string name) {
  this->pkb->AddStmt(name, CONSTS);
}
