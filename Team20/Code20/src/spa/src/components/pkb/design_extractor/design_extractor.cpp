#include "design_extractor.h"
#include "components/pkb/pkb.h"

DesignExtractor::DesignExtractor(ProgramNode root_node, PKB *pkb, UsageStore storage) :
    root_node(root_node), pkb(pkb), storage(storage) {
  this->root_node = root_node;
  this->pkb = pkb;
  this->storage = storage;
}

void DesignExtractor::traverse_ast() {
  std::vector<std::shared_ptr<ProcedureNode>> proc_list = this->root_node.GetProcedures();
  for (auto &proc : proc_list) {
    std::shared_ptr<StatementListNode> stmtLst = proc->GetStatementList();
    PopulateProc(proc->GetIdentifier());
    ProcessProc(proc, stmtLst);
  }
}

void DesignExtractor::ProcessProc(std::shared_ptr<ProcedureNode> proc, std::shared_ptr<StatementListNode> stmtLst) {
  std::vector<std::shared_ptr<StatementNode>> stmts = stmtLst->GetStatements();
  for (auto &stmt : stmts) {
    std::string stmt_num = std::to_string(stmt->GetStatementNumber());
    StmtType stmt_type = stmt->GetStatementType();
    std::string var_name = "";
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
        PopulateModifies(stmt_num, var_name);
        break;
      }
      case PRINT: {
        PopulateStmt(stmt_num);
        std::shared_ptr<PrintStatementNode> print_stmt = static_pointer_cast<PrintStatementNode>(stmt);
        var_name = print_stmt->GetIdentifier()->GetIdentifier();
        PopulateVars(var_name);
        PopulatePrint(stmt_num);
        PopulateUses(stmt_num, var_name);
        break;
      }
      case ASSIGN: {
        PopulateStmt(stmt_num);
        std::shared_ptr<AssignStatementNode> assign_stmt = static_pointer_cast<AssignStatementNode>(stmt);
        var_name = assign_stmt->GetIdentifier()->GetIdentifier();
        PopulateVars(var_name);
        std::shared_ptr<ExpressionNode> expr = assign_stmt->GetExpression();

        ProcessAssign(expr);
        PopulateAssign(stmt_num);
        //PopulateModifies(stmt_num);
        break;
      }
      case WHILE: {
        std::shared_ptr<WhileStatementNode> while_stmt = static_pointer_cast<WhileStatementNode>(stmt);
        PopulateWhile(stmt_num);
        break;
      }
      case IF: {
        std::shared_ptr<IfStatementNode> if_stmt = static_pointer_cast<IfStatementNode>(stmt);
        std::shared_ptr<ConditionalExpressionNode> if_stmt_cond = if_stmt->GetCondition();
        std::shared_ptr<StatementListNode> if_block = if_stmt->GetIfStatementList();
        std::shared_ptr<StatementListNode> else_block = if_stmt->GetElseStatementList();

        ProcessProc(proc, if_block);
        ProcessProc(proc, else_block);
        PopulateIf(stmt_num);
        break;
      }
    }
  }
}

void DesignExtractor::ProcessAssign(std::shared_ptr<ExpressionNode> expr) {
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
      ProcessAssign(lhs);
      ProcessAssign(rhs);
    }
    case ExpressionType::VARIABLE: {
      std::shared_ptr<VariableNode> var = static_pointer_cast<VariableNode>(expr);
      std::string var_name = var->GetIdentifier();
      PopulateVars(var_name);
    }
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
