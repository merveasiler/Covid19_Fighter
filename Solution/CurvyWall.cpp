#include "CurvyWall.h"

CurvyWall::CurvyWall(float length, float radius) {
	this->length = length;
	this->radius = radius;
	this->center = NULL;
	this->p1 = NULL;
	this->p2 = NULL;
}

CurvyWall::CurvyWall(const Particle& p1, const Particle& p2, const Particle& midParticle) : Wall(p1, p2) {
	center = new Particle(midParticle);
	radius = p1.FindDistance(*center);
	ComputeLength();
}

CurvyWall::CurvyWall(const CurvyWall& curvyWall) {
	length = curvyWall.length;
	radius = curvyWall.radius;
	if (curvyWall.p1 == NULL)
		p1 = NULL;
	else
		p1 = new Particle(*(curvyWall.p1));
	if (curvyWall.p2 == NULL)
		p2 = NULL;
	else
		p2 = new Particle(*(curvyWall.p2));
	if (curvyWall.center == NULL)
		center = NULL;
	else
		center = new Particle(*(curvyWall.center));
}

float CurvyWall::GetRadius() const {
  return radius;
}

const Particle& CurvyWall::GetCenter() const {
	return *center;
}

void CurvyWall::ComputeLength() {
	float chord = p1->FindDistance(*p2);
	float arcAngle = asin((chord/2)/radius) * 2;
	length = arcAngle*radius;
}

Wall* CurvyWall::Clone() const {
	Wall* wall = new CurvyWall(*this);
	return wall;
}

bool CurvyWall::IsLinear() const {
	return false;
}

bool CurvyWall::IsContinuousLinear(const Wall&) const {
	return false;
}

const Wall& CurvyWall::operator+(const Wall& wall) const {
  if (!wall.IsLinear() && this->p1 != NULL && this->p2 != NULL && this->center != NULL) {
    const CurvyWall* curvyWall = (CurvyWall*) &wall;
    if (*(this->p1) == *(wall.p2) && *(this->center) == *(curvyWall->center)) {
      CurvyWall* joinedWall = new CurvyWall(*(wall.p1), *(this->p2), *(this->center));
      return *joinedWall;
    }

    if (*(this->p2) == *(wall.p1) && *(this->center) == *(curvyWall->center)) {
      CurvyWall* joinedWall = new CurvyWall(*(this->p1), *(wall.p2), *(this->center));
      return *joinedWall;
    }
  }

  throw ApplePearException();
}

CurvyWall::~CurvyWall() {
	if (center != NULL)
		delete center;
}
