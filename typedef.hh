#ifndef _H_JJOUVE_TYPEDEF_H_
#define _H_JJOUVE_TYPEDEF_H_
#include <vector>
#include <list>

#include "prologin.hh"

#define SURFACE_ETABLI (TAILLE_ETABLI * TAILLE_ETABLI)
#define MOI (moi())
#define AUTRE (adversaire())
#define NOMBRE_ELEMENTS 5
#define NOMBRE_ECHANTILLONS (NOMBRE_ELEMENTS * NOMBRE_ELEMENTS)
#define MAXIMUM_DISTANCE (2*TAILLE_ETABLI)

typedef position Position;
typedef position_echantillon PositionEchantillon;
typedef echantillon Echantillon;
typedef case_type Element;
typedef erreur Erreur;
typedef int Apprentie;

using Marquage = std::vector<std::vector<bool> >;

Element ELEMENT_DEFAUT = VIDE;
Position AUCUNE_POSITION = Position{ -1, -1 };
std::list<Position> LISTE_POSITION_DEFAUT;
std::vector<Position> VECTOR_POSITION_DEFAUT;

#endif

