#include "Squadratic.h"

Squadratic::Squadratic(int id, float edgeLength) : MicroOrganism(id) {
	this->edgeLength = edgeLength;
	for (int i = 0; i < 4; i++) {
		vertices[i] = NULL;
		babies[i] = NULL;
	}
}

Squadratic::Squadratic(const Squadratic& squadratic) {
	this->id = squadratic.id;
	this->edgeLength = squadratic.edgeLength;
	for (int i = 0; i < 4; i++) {
		if (squadratic.vertices[i] != NULL)
			this->vertices[i] = new Particle(*squadratic.vertices[i]);
		else
			this->vertices[i] = NULL;
		if ( squadratic.babies[i] != NULL)
			this->babies[i] = new Squadratic(*squadratic.babies[i]);
		else
			this->babies[i] = NULL;
	}
	for (int i=0; i<squadratic.edges.size(); i++)
		this->edges.push_back(new Wall(*squadratic.edges[i]));
}

Squadratic::~Squadratic() {
	for (int i = 0; i < 4; i++) {
		if (vertices[i] != NULL)
			delete vertices[i];
		if (babies[i] != NULL)
			delete babies[i];
		}
}

void Squadratic::ConnectToCell(Cell* const cell) {
	cell->StrengthenCellWall();
	const vector<Wall*> cellWall = cell->GetCellWall();
	for (int i = 0; i < 4; i++)
		this->vertices[i] = new Particle(cellWall[i]->GetInitialParticle());
	MicroOrganism::ConnectToCell(cell);
}

bool Squadratic::DoesFitInto(const Cell& cell) const {
	Cell* virtualCell = new Cell(cell);
	virtualCell->StrengthenCellWall();
	const vector<Wall*> cellWall = virtualCell->GetCellWall();
	bool doesFit = false;

	if (cellWall.size() == 4) {
		doesFit = true;
		for (int i=0; i<4; i++) {
			if ( abs(cellWall[i]->GetLength() - edgeLength) > EPSILON || abs(cellWall[i]->FindAngleBetween(*cellWall[(i+1)%4]) - (PI/2)) > EPSILON) {
				doesFit = false;
				break;
			}
		}
	}

	delete virtualCell;
	return doesFit;
}

void Squadratic::React() {
	// Compute the center of this squadratic
	float x = 0, y = 0;
	for (int i=0; i<4; i++) {
		x += vertices[i]->X();
		y += vertices[i]->Y();
	}
	Particle* center = new Particle(x/4, y/4);

	// Compute baby squadratics
	for (int i=0; i<4; i++) {
		Squadratic* baby = new Squadratic(-1, edgeLength/2);
		Wall* wall = edges[i];
		Wall* previousWall = edges[(i+3)%4];
		const Particle p = wall->GetInitialParticle();	// = previousWall.GetFinalParticle();
		const Particle q = wall->GetFinalParticle();
		const Particle r = previousWall->GetInitialParticle();
		baby->vertices[0] = new Particle(p);
		baby->vertices[1] = new Particle( (p.X() + q.X()) / 2, (p.Y() + q.Y()) / 2);
		baby->vertices[2] = new Particle(*center);
		baby->vertices[3] = new Particle((p.X() + r.X()) / 2, (p.Y() + r.Y()) / 2);
		for (int j=0; j<4; j++)
			(baby->edges).push_back(new Wall(*(baby->vertices[j]), *(baby->vertices[(j+1)%4])));
		this->babies[i] = baby;
	}

	delete center;
}

Tissue Squadratic::DefineOccupation() const {
	if (this->vertices[0] == NULL)
		return Tissue();

	// find bounding box limits
	float minX, maxX, minY, maxY;
	minX = maxX = this->vertices[0]->X();
	minY = maxY = this->vertices[0]->Y();
	for (int i=1; i<4; i++) {
		if (vertices[i]->X() < minX)
			minX = vertices[i]->X();
		else if (vertices[i]->X() > maxX)
			maxX = vertices[i]->X();
		if (vertices[i]->Y() < minY)
			minY = vertices[i]->Y();
		else if (vertices[i]->Y() > maxY)
			maxY = vertices[i]->Y();
	}

	return Tissue(minX, maxX, minY, maxY);	// Define bounding tissue

}

Squadratic& Squadratic::GetChild(float minX, float maxX, float minY, float maxY) const {
	for (int i=0; i<4; i++)
		if (babies[i] != NULL) {
			Tissue t = babies[i]->DefineOccupation();
			float* borders = t.GetCoordinates();
			if (borders[0] <= minX && borders[1] >= maxX && borders[2] <= minY && borders[3] >= maxY) {
				if (abs(borders[0] - minX) < EPSILON && abs(borders[1] - maxX) < EPSILON && abs(borders[2] - minY) < EPSILON && abs(borders[3] - maxY) < EPSILON) {
					delete[] borders;
					return *babies[i];
				}
				delete[] borders;
				return babies[i]->GetChild(minX, maxX, minY, maxY);
			}
			delete[] borders;
		}

	throw NotBornChildException();
}
