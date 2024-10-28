#include "Triangular.h"

Triangular::Triangular(int id, const vector<Wall*>& edges, const NucleoBase& nbChain) : MicroOrganism(id, edges) {
	for (int i = 0; i < 3; i++)
		vertices[i] = NULL;
	RNA = new NucleoBase(nbChain);
}

Triangular::~Triangular() {
	for (int i = 0; i < 3; i++)
		if (vertices[i] != NULL)
			delete vertices[i];
	if (RNA != NULL)
		delete RNA;
}

bool Triangular::DoesFitInto(const Cell& cell) const {
	Cell* virtualCell = new Cell(cell);
	virtualCell->StrengthenCellWall();
	const vector<Wall*> cellWall = virtualCell->GetCellWall();
	bool doesFit = false;

	if (cellWall.size() == 3)
		for (int i=0; i<3; i++)
			if ( abs(edges[0]->GetLength() - cellWall[i]->GetLength()) < EPSILON) {
				if ( abs(edges[1]->GetLength() - cellWall[(i+1)%3]->GetLength()) < EPSILON && abs(edges[2]->GetLength() - cellWall[(i+2)%3]->GetLength()) < EPSILON)
					doesFit = true;
				else if ( abs(edges[2]->GetLength() - cellWall[(i+1)%3]->GetLength()) < EPSILON && abs(edges[1]->GetLength() - cellWall[(i+2)%3]->GetLength()) < EPSILON)
					doesFit = true;
				break;
			}

	delete virtualCell;
	return doesFit;
}

void Triangular::ConnectToCell(Cell* const cell) {
	cell->StrengthenCellWall();
	const vector<Wall*> cellWall = cell->GetCellWall();
	for (int i = 0; i < 3; i++) {
		if (this->vertices[i] != NULL)
			delete this->vertices[i];
		this->vertices[i] = new Particle(cellWall[i]->GetInitialParticle());
	}
	MicroOrganism::ConnectToCell(cell);
}

void Triangular::React() {
	Tissue* tissue = cell->GetTissue();
	tissue->RemoveCell(*cell);
}

const NucleoBase& Triangular::GetRNA() const {
	return *RNA;
}

void Triangular::Mutate(Triangular& triangular) {
	if (triangular.RNA == NULL)
		return;
	if (this->RNA == NULL) {
		this->RNA = triangular.RNA;
		triangular.RNA = NULL;
		return;
	}

	NucleoBase* otherChain = triangular.RNA;
	NucleoBase* thisChain = this->RNA;
	NucleoBase* otherLast = (triangular.RNA)->GetLast();
	NucleoBase* thisLast = this->RNA->GetLast();

	// Separate the identical part at the beginning
	while (otherChain) {
		if (thisChain == NULL) {
			thisChain = otherChain;
			otherChain = NULL;
			return;
		}
		if (otherChain->data == thisChain->data) {
			thisChain = thisChain->next;
			otherChain = otherChain->next;
		}
		else
			break;
	}

	// Separate the identical part at the end
	while (otherLast->data == thisLast->data) {
		thisLast = thisLast->prev;
		otherLast = otherLast->prev;
	}

	// Apply interchange rule
	while (true) {
		if (thisChain->data == 'A' && otherChain->data == 'U') {
			thisChain->data = 'U';
			otherChain->data = 'A';
		}
		else if (thisChain->data == 'U' && otherChain->data == 'A') {
			thisChain->data = 'A';
			otherChain->data = 'U';
		}

		else if (thisChain->data == 'G' && otherChain->data == 'S') {
			thisChain->data = 'S';
			otherChain->data = 'G';
		}

		else if (thisChain->data == 'S' && otherChain->data == 'G') {
			thisChain->data = 'G';
			otherChain->data = 'S';
		}
		else {
				NucleoBase* temp = thisLast->next;

				NucleoBase* temp1 = thisChain->next;
				thisChain->prev->next = thisChain->next;
				thisChain->next->prev = thisChain->prev;
				thisChain->next = NULL;
				delete thisChain;
				thisChain = temp1;

				NucleoBase* temp2 = otherChain->next;
				otherChain->prev->next = otherChain->next;
				otherChain->next->prev = otherChain->prev;
				otherChain->next = NULL;
				delete otherChain;
				otherChain = temp2;

				if (thisChain == temp)
					break;
				continue;
		}

		if (thisChain == thisLast)
			break;
		thisChain = thisChain->next;
		otherChain = otherChain->next;
	}

}
