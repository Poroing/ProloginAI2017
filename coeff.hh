#ifndef _H_JJOUVE_COEFF_H_
#define _H_JJOUVE_COEFF_H_

#include "typedef.hh"


#define COEFF_CATALYSEUR_AUTRE 0.5 
#define COEFF_OR_AUTRE 1
//Lorsqu'on le prend en compte, retourne une valeur maximal lorsque
//l'on peut avoir plus de catalyseur que sa valeure
#define MAXIMUM_CATALYSEUR_TRANSMUTE_AUTRE 1
#define COEFF_DISTANCE_ORIENTATION_AUTRE 0.05

#define COEFF_CATALYSEUR_MOI 0.25
#define COEFF_OR_MOI 1
#define MAXIMUM_CATALYSEUR_TRANSMUTE_MOI 2
#define COEFF_DISTANCE_ORIENTATION_MOI 1

float MINIMUM_VALEUR_POSITION_ELEMENT = -1e15;

float MAXIMUM_VALEUR = 1e15;

float avoirCoeffCatalyseur(Apprentie apprentie) {
    if (apprentie == MOI)
        return COEFF_CATALYSEUR_MOI;
    return COEFF_CATALYSEUR_AUTRE;
}

float avoirCoeffOr(Apprentie apprentie)
{
    if (apprentie == MOI)
        return COEFF_OR_MOI;
    return COEFF_OR_AUTRE;
}

float avoirBorneCatalytique(Apprentie apprentie)
{
    if (apprentie == MOI)
        return MAXIMUM_CATALYSEUR_TRANSMUTE_MOI;
    return MAXIMUM_CATALYSEUR_TRANSMUTE_AUTRE;
}

float avoirCoefficientDistanceOrientation(Apprentie apprentie)
{
    if (apprentie == MOI)
        return COEFF_DISTANCE_ORIENTATION_MOI;
    return COEFF_DISTANCE_ORIENTATION_AUTRE;
}

float appliquerCoeffRessource(int quantite_or, int quantite_catalyseur,
        Apprentie apprentie, bool borne_catalytique)
{
    if (borne_catalytique
            && quantite_catalyseur >= avoirBorneCatalytique(apprentie))
    {
        return MAXIMUM_VALEUR;
    }
    return quantite_or * avoirCoeffOr(apprentie)
        + quantite_catalyseur * avoirCoeffCatalyseur(apprentie);
}

float appliquerCoeffOrientation(float distance, Apprentie apprentie)
{
    return avoirCoefficientDistanceOrientation(apprentie)
        * distance;
}

#endif
