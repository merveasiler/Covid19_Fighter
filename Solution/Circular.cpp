#include "Circular.h"

Circular::Circular(int id, float radius) : MicroOrganism(id) {
	this->radius = radius;
	this->center = NULL;
}

Circular::~Circular() {
	if (center != NULL)
		delete center;
}

void Circular::ConnectToCell(Cell* const cell) {
	CurvyWall* circle =(CurvyWall*)(cell->GetCellWall()[0]);
	this->center = new Particle(circle->GetCenter());
	MicroOrganism::ConnectToCell(cell);
}

bool Circular::DoesFitInto(const Cell& cell) const {
	Cell* virtualCell = new Cell(cell);
	virtualCell->StrengthenCellWall();
	const vector<Wall*> cellWall = virtualCell->GetCellWall();
	bool doesFit = false;

	if (cellWall.size() == 1) {
		if ( abs(((CurvyWall*)cellWall[0])->GetRadius() - this->radius) < EPSILON )
			doesFit = true;
	}

	delete virtualCell;
	return doesFit;
}

void Circular::React() {
	float newRadius = radius*2;
	Particle rootParticle(center->X() + newRadius, center->Y());
	vector<Wall*> newCellWall;
	newCellWall.push_back(new CurvyWall(rootParticle, rootParticle, *center));
	this->cell->RenewCellWall(newCellWall);
}

bool Circular::DoesContain(const Particle& particle) const {
	if (pow(particle.X()-center->X(), 2) + pow(particle.Y()-center->Y(), 2) <= this->radius)
		return true;
	else
		return false;
}
