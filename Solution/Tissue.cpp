#include "Tissue.h"

Tissue::Tissue() {
	parentTissue = NULL;
	microOrganism = NULL;
	numberOfCells = 0;
}

Tissue::Tissue(float minX, float maxX, float minY, float maxY) {
	this->minX = minX;
	this->maxX = maxX;
	this->minY = minY;
	this->maxY = maxY;
	parentTissue = NULL;
	microOrganism = NULL;
	numberOfCells = 0;
}

// shallow copy
Tissue::Tissue(const Tissue& tissue) {
	this->SetCoordinates(tissue.minX, tissue.maxX, tissue.minY, tissue.maxY);
	this->microOrganism = tissue.microOrganism;
	this->parentTissue = tissue.parentTissue;
	for (int i=0; i < tissue.subTissues.size(); i++)
		this->subTissues.push_back(tissue.subTissues[i]);
	this->numberOfCells = tissue.numberOfCells;
}

Tissue::~Tissue() {
	for (int i = 0; i < cells.size(); i++)
		if (cells[i] != NULL)
			delete cells[i];
	for (int i = 0; i < subTissues.size(); i++)
		if (subTissues[i] != NULL)
			delete subTissues[i];
	parentTissue = NULL;
	microOrganism = NULL;		// should I want to delete the microOrganism pointer also?
}

float* Tissue::GetCoordinates() {
	float* borders = new float[4];
	borders[0] = minX;
	borders[1] = maxX;
	borders[2] = minY;
	borders[3] = maxY;
	return borders;
}

void Tissue::AddCell(Cell& cell) {
	this->cells.push_back(&cell);
	numberOfCells++;
}

void Tissue::RemoveCell(Cell& cell) {
	int id = cell.GetCellID();
	delete cells[id];
	cells[id] = NULL;
	numberOfCells--;
}

const Cell& Tissue::GetCell(int id) const {
	if (cells[id] != NULL)
		return *cells[id];
	throw DeadCellException();
}

void Tissue::SetCoordinates(float minX, float maxX, float minY, float maxY) {
	this->minX = minX;
	this->maxX = maxX;
	this->minY = minY;
	this->maxY = maxY;
}

void Tissue::Interact(const vector<MicroOrganism*>& microOrganisms) {
	for (int i = 0; i < microOrganisms.size(); i++) {
		for (int j = 0; j < this->cells.size(); j++)
			if (cells[j] != NULL) {
				if (microOrganisms[i]->DoesFitInto(*cells[j])) {
					microOrganisms[i]->ConnectToCell(cells[j]);
					break;
				}
			}
	}
}
