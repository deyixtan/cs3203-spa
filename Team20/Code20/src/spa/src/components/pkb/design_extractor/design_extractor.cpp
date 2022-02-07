#include "design_extractor.h"
#include "components/pkb/pkb.h"

DesignExtractor::DesignExtractor(ProgramNode root_node, PKB *pkb, UsageStore storage) :
    root_node(root_node), pkb(pkb), storage(storage) {
  this->root_node = root_node;
  this->pkb = pkb;
  this->storage = storage;
}

void DesignExtractor::traverse_ast() {
  std::vector<std::shared_ptr<ProcedureNode>> proc_list = this->root_node.getProcedures();
  for (auto &proc : proc_list) {
    std::shared_ptr<StatementListNode> stmtLst = proc->getStatementList();
    std::vector<std::shared_ptr<StatementNode>> stmts = stmtLst->getStatements();
    populate_proc(proc->getName());
    process_proc(proc, stmtLst, stmts);
  }
}

void DesignExtractor::process_proc(std::shared_ptr<ProcedureNode> proc, std::shared_ptr<StatementListNode> stmtLst, std::vector<std::shared_ptr<StatementNode>> stmts) {
  for (auto &stmt : stmts) {
    int stmt_num = stmt->getLineNumber();
    StmtType stmt_type = stmt->getStatementType();
    std::string var_name = "";
    switch(stmt_type) {
      case PROC:
        populate_proc(proc->getName());
        break;
      case STMT:
        populate_stmt(stmt_num);
        break;
      case READ: {
        populate_stmt(stmt_num);
        std::shared_ptr<ReadStatementNode> read_stmt = static_pointer_cast<ReadStatementNode>(stmt);
        var_name = read_stmt->getId()->getName();
        populate_vars(var_name);
        populate_read(stmt_num);
        populate_modifies(stmt_num, var_name);
        break;
      }
      case PRINT: {
        populate_stmt(stmt_num);
        std::shared_ptr<PrintStatementNode> print_stmt = static_pointer_cast<PrintStatementNode>(stmt);
        var_name = print_stmt->getId()->getName();
        populate_vars(var_name);
        populate_print(stmt_num);
        populate_uses(stmt_num, var_name);
        break;
      }
      case ASSIGN: {
        populate_stmt(stmt_num);
        std::shared_ptr<AssignStatementNode> assign_stmt = static_pointer_cast<AssignStatementNode>(stmt);
        var_name = assign_stmt->getId()->getName();
        populate_vars(var_name);
        std::shared_ptr<ExpressionNode> expr = assign_stmt->getExpr();
        ExpressionType expr_type = expr->getExpressionType();
        switch (expr_type) {
          case ExpressionType::CONSTANT: {
            std::shared_ptr<ConstantNode> constant = static_pointer_cast<ConstantNode>(expr);
            std::string name = constant->getValue();
            populate_const(name);
            break;
          }
          case ExpressionType::COMBINATION: {
            std::shared_ptr<CombinationExpressionNode> comb = static_pointer_cast<CombinationExpressionNode>(expr);
            std::shared_ptr<ExpressionNode> lhs = comb->getLHS();
            std::shared_ptr<ExpressionNode> rhs = comb->getRHS();
            Operation op = comb->getOperation();
          }
          case ExpressionType::VARIABLE: {
            std::shared_ptr<VariableNode> var = static_pointer_cast<VariableNode>(expr);
            std::string name = var->getName();
            populate_const(name);
          }
        }
        populate_assign(stmt_num);
        //populate_modifies(stmt_num);
        break;
      }
      case WHILE: {
        std::shared_ptr<WhileStatementNode> while_stmt = static_pointer_cast<WhileStatementNode>(stmt);
        populate_while(stmt_num);
        //populate_modifies(stmt_num);
        break;
      }
      case IF: {
        std::shared_ptr<IfStatementNode> if_stmt = static_pointer_cast<IfStatementNode>(stmt);
        populate_if(stmt_num);
        //populate_modifies(stmt_num);
        break;
      }
    }
  }
}

void DesignExtractor::populate_uses(int stmt, std::string var) {
  this->pkb->add_usage_stmt_var(stmt, var);
}

void DesignExtractor::populate_modifies(int stmt, std::string var) {
  this->pkb->add_modify_stmt_var(stmt, var);
}

void DesignExtractor::populate_proc(std::string name) {
  this->pkb->add_stmt(name, PROC);
}

void DesignExtractor::populate_assign(int stmt) {
  this->pkb->add_stmt(stmt, ASSIGN);
}

void DesignExtractor::populate_stmt(int stmt) {
  this->pkb->add_stmt(stmt, STMT);
}

void DesignExtractor::populate_read(int stmt) {
  this->pkb->add_stmt(stmt, READ);
}

void DesignExtractor::populate_print(int stmt) {
  this->pkb->add_stmt(stmt, PRINT);
}

void DesignExtractor::populate_vars(std::string var) {
  this->pkb->add_stmt(var, VARS);
}

void DesignExtractor::populate_while(int stmt) {
  this->pkb->add_stmt(stmt, WHILE);
}

void DesignExtractor::populate_if(int stmt) {
  this->pkb->add_stmt(stmt, IF);
}

void DesignExtractor::populate_const(std::string name) {
  this->pkb->add_stmt(name, CONSTS);
}
