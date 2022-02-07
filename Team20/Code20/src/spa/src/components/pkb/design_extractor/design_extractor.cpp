#include "design_extractor.h"
#include "components/pkb/pkb.h"

DesignExtractor::DesignExtractor(Program root_node, PKB *pkb, UsageStore storage) :
    root_node(root_node), pkb(pkb), storage(storage) {
  this->root_node = root_node;
  this->pkb = pkb;
  this->storage = storage;
}

void DesignExtractor::traverse_ast() {
  std::vector<std::shared_ptr<Procedure>> proc_list = this->root_node.getProcedures();
  for (auto &proc : proc_list) {
    std::string proc_name = proc->getName();
    populate_proc(proc_name);
    process_proc(proc);
  }
}

void DesignExtractor::process_proc(std::shared_ptr<Procedure> proc) {
  std::shared_ptr<StatementList> stmtLst = proc->getStatementList();
  std::vector<std::shared_ptr<Statement>> stmts = stmtLst->getStatements();
  for (auto &stmt : stmts) {
    int stmt_num = stmt->getLineNumber();
    StmtType stmt_type = stmt->getStatementType();
    std::string var_name = "";

    switch(stmt_type) {
      case STMT:
        populate_stmt(stmt_num);
        break;
      case READ: {
        std::shared_ptr<ReadStatement> read_stmt = static_pointer_cast<ReadStatement>(stmt);
        var_name = read_stmt->getId()->getName();
        populate_read(stmt_num);
        //populate_modifies(stmt_num, var_name);
        break;
      }
      case PRINT: {
        std::shared_ptr<PrintStatement> print_stmt = static_pointer_cast<PrintStatement>(stmt);
        var_name = print_stmt->getId()->getName();
        populate_print(stmt_num);
        populate_uses(stmt_num, var_name);
        break;
      }
      case ASSIGN: {
        std::shared_ptr<AssignStatement> assign_stmt = static_pointer_cast<AssignStatement>(stmt);
        var_name = assign_stmt->getId()->getName();
        populate_assign(stmt_num);
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
  this->pkb->add_stmt(stmt, READ);
}

void DesignExtractor::populate_vars(std::string var) {
  this->pkb->add_stmt(var, VARS);
}


