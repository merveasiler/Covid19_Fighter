#include "Wall.h"

Wall::Wall() {

}

Wall::Wall(float length) {
	this->length = length;
	this->p1 = NULL;
	this->p2 = NULL;
}

Wall::Wall(const Particle& p1, const Particle& p2) {
	this->p1 = new Particle(p1);
	this->p2 = new Particle(p2);
	ComputeLength();
}

Wall::Wall(const Wall& wall) {
	length = wall.length;
	if (wall.p1 == NULL)
		p1 = NULL;
	else
		p1 = new Particle(*(wall.p1));
	if (wall.p2 == NULL)
		p2 = NULL;
	else
		p2 = new Particle(*(wall.p2));
}

float Wall::FindAngleBetween(const Wall& wall) const {
  float cosAngle = ((p2->X() - p1->X()) / length) * ((wall.p2->X() - wall.p1->X()) / wall.length) +
                  ((p2->Y() - p1->Y()) / length) * ((wall.p2->Y() - wall.p1->Y()) / wall.length);
  return acos(cosAngle);
}

const Particle& Wall::GetInitialParticle() const {
    return *p1;
}

const Particle& Wall::GetFinalParticle() const {
  return *p2;
}

float Wall::GetLength() const {
	return length;
}

void Wall::ComputeLength() {
		length = sqrt(pow(p1->Y() - p2->Y(),2) + pow(p1->X() - p2->X(),2));
}

Wall* Wall::Clone() const {
	Wall* wall = new Wall(*this);
	return wall;
}

bool Wall::IsLinear() const {
  return true;
}

bool Wall::IsContinuousLinear(const Wall& wall) const {
	if (wall.IsLinear() && (*(this->p2) == *(wall.p1) || *(this->p1) == *(wall.p2))) {
		// Vertical line test
		if (abs(this->p2->X() - this->p1->X()) < EPSILON && abs(wall.p2->X() - wall.p1->X()) < EPSILON)
			return true;
		else if (abs(this->p2->X() - this->p1->X()) < EPSILON)
			return false;
		else if (abs(wall.p2->X() - wall.p1->X()) < EPSILON)
			return false;
		else;
		float slope1 = (this->p2->Y() - this->p1->Y()) / (this->p2->X() - this->p1->X());
		float slope2 = (wall.p2->Y() - wall.p1->Y()) / (wall.p2->X() - wall.p1->X());
		if (abs(slope1-slope2) < EPSILON)
			return true;
	}
	return false;
}

const Wall& Wall::operator+(const Wall& wall) const {
  if (this->p1 != NULL && this->p2 != NULL && this->IsContinuousLinear(wall)) {
    if (*(this->p1) == *(wall.p2)) {
      Wall* joinedWall = new Wall(*(wall.p1), *(this->p2));
      return *joinedWall;
    }

    if (*(this->p2) == *(wall.p1)) {
      Wall* joinedWall = new Wall(*(this->p1), *(wall.p2));
      return *joinedWall;
    }
  }

  throw ApplePearException();
}

Wall::~Wall() {
	if (p1 != NULL)
    delete p1;
	if( p2 != NULL)
    delete p2;
}
