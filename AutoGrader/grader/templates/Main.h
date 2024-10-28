#ifndef MAIN_H
#define MAIN_H

#include "Particle.h"
#include "Wall.h"
#include "CurvyWall.h"
#include "Cell.h"
#include "Tissue.h"

#include <fstream>
#include <string>

/*********************************************************************
*				DO NOT MODIFY THIS FILE !!!							 *
/********************************************************************/

/*********************************************************************
*				ReadCells
*
* Takes the file name which holds the cell data as first @param
* Creates the corresponding cell objects into the Tissue object given
  in the second @param.and holds them in vector
* @return the cell whose id is given in the third @param
/********************************************************************/
Cell* ReadCells(const char* fileName, Tissue& tissue, int cellID) {
    Cell* askedCell;    // the cell to be returned
    ifstream cellsFile(fileName);
    if (cellsFile.is_open()) {
        int numberOfParticles, numberOfCells, id;
        cellsFile >> numberOfParticles >> numberOfCells;

		// READ PARTICLES
		vector<Particle*> particles;
		for (int i=0; i<numberOfParticles; i++) {
			float x, y;	                              // particle coordinates
			cellsFile >> id >> x >> y;
			particles.push_back(new Particle(x, y));	// it will locate in particles[id]
		}

		// READ CELLS
		for (int i=0; i<numberOfCells; i++) {
				int p1, p2, center, numberOfWalls;      // start point id, end point id, center point id
                cellsFile >> id >> numberOfWalls;       // read the cell id and the number of walls in the cell
				// read walls
				vector<Wall*> cellWall;
				for (int j=0; j<numberOfWalls; j++) {
                    char wallType;
					cellsFile >> wallType;
					if (wallType == 's') {	              // straight wall
						cellsFile >> p1 >> p2;
                        Wall* wall = new Wall(*particles[p1], *particles[p2]);
						cellWall.push_back(wall);
					}
					else {                               // curvy wall
                        cellsFile >> p1 >> p2 >> center;
                        Wall* wall = new CurvyWall(*particles[p1], *particles[p2], *particles[center]);
						cellWall.push_back(wall);
					}
				}
                Cell* cell = new Cell(id, cellWall, &tissue);
				tissue.AddCell(*cell);      // do not delete cell since it is added directly;
                if (id == cellID)
                    askedCell = cell;
                for (int j=0; j<cellWall.size(); j++)
                    delete cellWall[j];
		}

        cellsFile.close();
        for (int i=0; i<numberOfParticles; i++)
            delete particles[i];
    }
    return askedCell;
}

#endif
