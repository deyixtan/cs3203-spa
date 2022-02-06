#include "node.h"

#include <iostream>

Node::Node(int line) {
  this->lineNumber = line;
}

RootNode::RootNode(std::vector<std::shared_ptr<ProcedureNode>> procedureList, int lineNumber)
    :Node(lineNumber){
  this->ProcList = procedureList;
};
bool RootNode::operator==(const Node& other) const {
  auto casted_other = dynamic_cast<const RootNode*>(&other);
  return casted_other != 0 &&
      std::equal(begin(this->ProcList), end(this->ProcList),
                 begin(casted_other->ProcList), end(casted_other->ProcList),
                 [](const auto t, const auto o) { return *t == *o; });
}
std::string RootNode::to_string() {
  std::string acc = "(RootNode ";
  for (const auto& proc : this->ProcList) {
    acc += proc->to_string();
    acc += " ";
  }
  return acc;
}

NumberNode::NumberNode(const std::string val, int lineNumber) : Node(lineNumber){
  this->Val = val;
};
bool NumberNode::operator==(const Node& other) const {
  auto casted_other = dynamic_cast<const NumberNode*>(&other);
  return casted_other != 0 && std::stoi(this->Val) == std::stoi(casted_other->Val);
};
std::string NumberNode::to_string() { return "(NumberNode " + std::to_string(std::stoi(this->Val)) + ")"; }

VariableNode::VariableNode(const std::string name, int lineNumber) : Node(lineNumber){
  this->Name = name;
};
bool VariableNode::operator==(const Node& other) const {
  auto casted_other = dynamic_cast<const VariableNode*>(&other);
  return casted_other != 0 && this->Name.compare(casted_other->Name) == 0;
};
std::string VariableNode::to_string() {
  return "(VariableNode " + this->Name + ")";
}

ReadNode::ReadNode(std::shared_ptr<VariableNode> var, int lineNumber) : Node(lineNumber) {
  this->Var = std::move(var);
};
bool ReadNode::operator==(const Node& other) const {
  auto casted_other = dynamic_cast<const ReadNode*>(&other);
  return casted_other != 0 && *this->Var == *casted_other->Var;
};

std::string ReadNode::to_string() {
  return "(ReadNode " + this->Var->to_string() + ")";
}

CallNode::CallNode(std::string procName, int lineNumber) : Node(lineNumber) {
  this->ProcName = procName;
};
bool CallNode::operator==(const Node& other) const {
  auto casted_other = dynamic_cast<const CallNode*>(&other);
  return casted_other != 0 &&
      this->ProcName.compare(casted_other->ProcName) == 0;
};

std::string CallNode::to_string() { return "(CallNode " + this->ProcName + ")"; }

PrintNode::PrintNode(std::shared_ptr<VariableNode> var, int lineNumber) : Node(lineNumber){
  this->Var = std::move(var);
};

bool PrintNode::operator==(const Node& other) const {
  auto casted_other = dynamic_cast<const PrintNode*>(&other);
  return casted_other != 0 && *this->Var == *casted_other->Var;
};

std::string PrintNode::to_string() {
  return "(PrintNode " + this->Var->to_string() + ")";
}

ProcedureNode::ProcedureNode(const std::string name, ::StmtList stmtList, int lineNumber) : Node(lineNumber) {
  this->Name = name;
  this->StmtList = std::move(stmtList);
};
bool ProcedureNode::operator==(const Node& other) const {
  auto casted_other = dynamic_cast<const ProcedureNode*>(&other);
  return casted_other != 0 && this->Name.compare(casted_other->Name) == 0 &&
      std::equal(
          begin(this->StmtList), end(this->StmtList),
          begin(casted_other->StmtList), end(casted_other->StmtList),
          [](const auto& t, const auto& o) {
            return std::visit(
                [](const auto& tp, const auto& op) { return *tp == *op; }, t,
                o);
          });
};

std::string ProcedureNode::to_string() {
  std::string acc = "(ProcedureNode " + this->Name + " ";
  for (const auto& stmt : this->StmtList) {
    acc += std::visit([](const auto& s) { return s->to_string(); }, stmt);
  }
  acc += ")";
  return acc;
}

BinaryOpNode::BinaryOpNode(Expr left, Expr right, std::string op, int lineNumber) : Node(lineNumber){
  this->Left = std::move(left);
  this->Right = std::move(right);
  this->Op = op;
};
bool BinaryOpNode::operator==(const Node& other) const {
  auto casted_other = dynamic_cast<const BinaryOpNode*>(&other);
  return casted_other != 0 &&
      std::visit([](auto& t, auto& o) { return *t == *o; }, this->Left,
                 casted_other->Left) &&
      std::visit([](auto& t, auto& o) { return *t == *o; }, this->Right,
                 casted_other->Right);
};
std::string BinaryOpNode::to_string() {
  std::string acc = "(BinOpNode " + this->Op + " ";
  acc += std::visit([](auto& s) { return s->to_string(); }, this->Left);
  acc + " ";
  acc += std::visit([](auto& s) { return s->to_string(); }, this->Right);
  acc += ")";
  return acc;
}

AssignNode::AssignNode(std::shared_ptr<VariableNode> var, Expr exp, int lineNumber)
    : Node(lineNumber){
  this->Var = std::move(var);
  this->Exp = exp;
};
bool AssignNode::operator==(const Node& other) const {
  auto casted_other = dynamic_cast<const AssignNode*>(&other);
  return casted_other != 0 && *this->Var == *casted_other->Var &&
      std::visit([](auto& t, auto& o) { return *t == *o; }, this->Exp,
                 casted_other->Exp);
};
std::string AssignNode::to_string() {
  std::string acc = "(AssignNode ";
  acc += this->Var->to_string() + " ";
  acc += std::visit([](auto& s) { return s->to_string(); }, this->Exp);
  acc += ")";
  return acc;
}

RelExprNode::RelExprNode(RelFactor lhs, std::string op, RelFactor rhs, int lineNumber)
    : Node(lineNumber){
  this->LHS = lhs;
  this->RHS = rhs;
};
bool RelExprNode::operator==(const Node& other) const {
  auto casted_other = dynamic_cast<const RelExprNode*>(&other);
  return casted_other != 0 &&
      std::visit([](auto& t, auto& o) { return *t == *o; }, this->LHS,
                 casted_other->LHS) &&
      std::visit([](auto& t, auto& o) { return *t == *o; }, this->RHS,
                 casted_other->RHS) &&
      this->Op.compare(casted_other->Op) == 0;
};
std::string RelExprNode::to_string() {
  std::string acc = "(RelExprNode " + this->Op;
  acc += std::visit([](auto& s) { return s->to_string(); }, this->LHS);
  acc += std::visit([](auto& s) { return s->to_string(); }, this->RHS);
  acc += ")";
  return acc;
}

CondExprNode::CondExprNode(std::shared_ptr<RelExprNode> relExpr, int lineNumber)
    :  Node(lineNumber){
  this->RelExpr = std::move(relExpr);
};
CondExprNode::CondExprNode(std::shared_ptr<CondExprNode> condLHS, int lineNumber)
    : Node(lineNumber){
  this->CondLHS = std::move(condLHS);
  this->Op = "!";
};
CondExprNode::CondExprNode(std::shared_ptr<CondExprNode> condLHS,
                           std::string op,
                           std::shared_ptr<CondExprNode> condRHS,
                           int lineNumber)
    : Node(lineNumber){
  this->CondLHS = std::move(condLHS);
  this->Op = op;
  this->CondRHS = std::move(condRHS);
};
bool CondExprNode::operator==(const Node& other) const {
  auto casted_other = dynamic_cast<const CondExprNode*>(&other);
  return casted_other != 0 &&
      (this->RelExpr == casted_other->RelExpr ||
          *this->RelExpr == *casted_other->RelExpr) &&
      (this->CondLHS == casted_other->CondLHS ||
          *this->CondLHS == *casted_other->CondLHS) &&
      (this->CondRHS == casted_other->CondRHS ||
          *this->CondRHS == *casted_other->CondRHS) &&
      (this->Op.compare(casted_other->Op) == 0);
};
std::string CondExprNode::to_string() {
  std::string s;
  // just RelExpr:
  if (RelExpr != nullptr) {
    s = "(cond " + RelExpr->to_string() + ")";
  } else if (CondRHS == nullptr) {
    // ! (CondExpr)
    s = "(cond " + Op + " " + CondLHS->to_string() + ")";
  } else {
    // (CondExpr Op CondExpr)
    s = "(cond " + Op + " " + CondLHS->to_string() + " " + CondRHS->to_string() + ")";
  }
  return s;
}

WhileNode::WhileNode(std::shared_ptr<CondExprNode> condExpr,
                     ::StmtList stmtList, int lineNumber) : Node(lineNumber){
  this->CondExpr = std::move(condExpr);
  this->StmtList = std::move(stmtList);
};
bool WhileNode::operator==(const Node& other) const {
  auto casted_other = dynamic_cast<const WhileNode*>(&other);
  return casted_other != 0 && *this->CondExpr == *casted_other->CondExpr &&
      std::equal(
          begin(this->StmtList), end(this->StmtList),
          begin(casted_other->StmtList), end(casted_other->StmtList),
          [](const auto& t, const auto& o) {
            return std::visit(
                [](const auto& tp, const auto& op) { return *tp == *op; }, t,
                o);
          });
};
std::string WhileNode::to_string() {
  std::string acc = "(WhileNode";
  acc += this->CondExpr->to_string();
  for (const auto& stmt : this->StmtList) {
    acc += std::visit([](const auto& s) { return s->to_string(); }, stmt);
  }
  acc += ")";
  return acc;
}

IfNode::IfNode(std::shared_ptr<CondExprNode> condExpr, StmtList stmtListThen,
               StmtList stmtListElse, int lineNumber) : Node(lineNumber) {
  this->StmtListElse = std::move(stmtListElse);
  this->StmtListThen = std::move(stmtListThen);
  this->CondExpr = std::move(condExpr);
}
bool IfNode::operator==(const Node& other) const {
  auto casted_other = dynamic_cast<const IfNode*>(&other);
  return casted_other != 0 && *this->CondExpr == *casted_other->CondExpr &&
      std::equal(
          begin(this->StmtListThen), end(this->StmtListThen),
          begin(casted_other->StmtListThen), end(casted_other->StmtListThen),
          [](const auto& t, const auto& o) {
            return std::visit(
                [](const auto& tp, const auto& op) { return *tp == *op; }, t,
                o);
          }) &&
      std::equal(
          begin(this->StmtListElse), end(this->StmtListElse),
          begin(casted_other->StmtListElse), end(casted_other->StmtListElse),
          [](const auto& t, const auto& o) {
            return std::visit(
                [](const auto& tp, const auto& op) { return *tp == *op; }, t,
                o);
          });
  ;
};

std::string IfNode::to_string() {
  std::string acc = "(IfNode";
  acc += this->CondExpr->to_string();
  acc += "(";
  for (const auto& stmt : this->StmtListThen) {
    acc += std::visit([](const auto& s) { return s->to_string(); }, stmt);
  }
  acc += ")";
  acc += "(";
  for (const auto& stmt : this->StmtListElse) {
    acc += std::visit([](const auto& s) { return s->to_string(); }, stmt);
  }
  acc += ")";
  acc += ")";
  return acc;
}