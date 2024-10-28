#include "MicroOrganism.h"

MicroOrganism::MicroOrganism() {  // used in copy constructor
  this->cell = NULL;
}

MicroOrganism::MicroOrganism(int id) {
  this->id = id;
  this->cell = NULL;
}

MicroOrganism::MicroOrganism(int id, const vector<Wall*>& edges) {
  for (int i=0; i < edges.size(); i++) {
    Wall* newEdge = edges[i]->Clone();
    (this->edges).push_back(newEdge);
  }
  this->cell = NULL;
  this->id = id;
}

void MicroOrganism::ConnectToCell(Cell* const cell) {
  // delete the old edges which do not have exact coordinates (having just lengths)
  for (int i=0; i < edges.size(); i++)
    delete edges[i];
  edges.clear();
  // create the new edges which have exact coordinates
  const vector<Wall*>& cellWall = cell->GetCellWall();
  for (int i=0; i<cellWall.size(); i++)
    edges.push_back(cellWall[i]->Clone());
  this->cell = cell;
}

MicroOrganism::~MicroOrganism() {
  for (int i=0; i < edges.size(); i++)
    delete edges[i];
}

ostream& operator<<(ostream& os, const MicroOrganism& microorganism) {
  if (microorganism.cell != NULL)
    os << "The microorganism " << microorganism.id << " was successfully placed into the cell " << microorganism.cell->GetCellID() << ".";
  else
    os << "The microorganism " << microorganism.id << " could not be placed into any cell!";
  return os;
}
