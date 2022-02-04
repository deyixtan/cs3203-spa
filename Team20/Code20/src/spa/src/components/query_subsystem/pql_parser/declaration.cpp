#include "declaration.h"

Declaration::Declaration(PqlToken synonym, PqlToken design_entity): synonym(synonym), design_entity(design_entity) {}

PqlToken Declaration::GetDesignEntity() {
  return synonym;
}

PqlToken Declaration::GetSynonym() {
  return design_entity;
}
