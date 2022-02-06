#include "declaration.h"

Declaration::Declaration() {}
Declaration::Declaration(PqlToken synonym, PqlToken design_entity): synonym(synonym), design_entity(design_entity) {}

PqlToken Declaration::GetDesignEntity() {
  return design_entity;
}

PqlToken Declaration::GetSynonym() {
  return synonym;
}
