#include "Cell.h"

Cell::Cell(int id, const vector<Wall*>& wallPieces, Tissue* tissue) {
	this->id = id;
	for (int i=0; i < wallPieces.size(); i++) {
		Wall* wall = wallPieces[i]->Clone();
		(this->cellWall).push_back(wall);
	}
	this->tissue = tissue;
}

Cell::Cell(const Cell& cell) {
	this->id = cell.id;
	for (int i=0; i < cell.cellWall.size(); i++) {
		Wall* wall = cell.cellWall[i]->Clone();
		(this->cellWall).push_back(wall);
	}
	this->tissue = cell.tissue;
}

int Cell::GetCellID() const {
	return id;
}

Tissue* Cell::GetTissue() const {
	return tissue;
}

const vector<Wall*>& Cell::GetCellWall() const {
	return cellWall;
}

void Cell::RenewCellWall(vector<Wall*>& newCellWall) {
	for (int i=0; i < cellWall.size(); i++)
		delete cellWall[i];
	cellWall = newCellWall;
}

void Cell::StrengthenCellWall() {
	vector<Wall*> strongCellWall;
	Wall* integralWall = cellWall[0]->Clone();

	for (int i=1; i < cellWall.size(); i++) {
		try {
			// Add to the previous wall
			const Wall* combinedWall = &(*integralWall + *cellWall[i]);
			delete integralWall;
			integralWall = combinedWall->Clone();
			delete combinedWall;
		} catch (ApplePearException e) {
			// Save the last integral wall piece and start a new wall
			strongCellWall.push_back(integralWall);
			integralWall = cellWall[i]->Clone();
		}
	}

	// Check for the last and the initial wall combination
	if (strongCellWall.size() > 0) {
		try {
			const Wall* combinedWall = &(*strongCellWall[0] + *integralWall);
			delete strongCellWall[0];
			strongCellWall[0] = combinedWall->Clone();
			delete combinedWall;
			delete integralWall;
		}
		catch (ApplePearException e) {
			strongCellWall.push_back(integralWall);	// put the last wall into array
		}
	}
	else
		strongCellWall.push_back(integralWall);

	RenewCellWall(strongCellWall);
}

Cell::~Cell() {
	for (int i=0; i < cellWall.size(); i++)
		delete cellWall[i];
}

ostream& operator<<(ostream& os, const Cell& cell) {

	for (int i=0; i < cell.cellWall.size(); i++) {
		os << cell.cellWall[i]->GetInitialParticle();
		if (i+1 < cell.cellWall.size())
			os << " - ";
	}
  return os;
}
