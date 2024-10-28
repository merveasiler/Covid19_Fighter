#include "Particle.h"

Particle::Particle(float x, float y) {
	xCoord = x;
	yCoord = y;
}

Particle::Particle(const Particle& particle) {
	xCoord = particle.xCoord;
	yCoord = particle.yCoord;
}

float Particle::X() const {
	return xCoord;
}

float Particle::Y() const {
	return yCoord;
}

float Particle::FindDistance(const Particle& particle) const {
	return sqrt( pow((this->xCoord - particle.xCoord), 2) + pow((this->yCoord - particle.yCoord), 2) );
}

bool Particle::operator== (const Particle& particle) const {
  if (abs(this->xCoord - particle.xCoord) < EPSILON && abs(this->yCoord - particle.yCoord) < EPSILON)
		return true;
	return false;
}

ostream& operator<<(ostream& os, const Particle& particle) {
		os << "(" << particle.xCoord << "," << particle.yCoord << ")";
		return os;
}
