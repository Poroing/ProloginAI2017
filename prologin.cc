/// This file has been generated, if you wish to
/// modify it in a permanent way, please refer
/// to the script file : gen/generator_cxx.rb

//TODO: essayer de transmuter avant de placer, mieux evaluer la transmutation
//      a faire

#include <vector>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <iterator>
#include <list>


#include "log.hh"
#include "prologin.hh"
#include "typedef.hh"
#include "non_apprenti_dependant.hh"

#define PRENDRE_EN_COMPTE_BORNE_CATALYTIQUE_PLACEMENT false
#define PRENDRE_EN_COMPTE_BORNE_CATALYTIQUE_CATALYSE_AUTRE true
#define PRENDRE_EN_COMPTE_BORNE_CATALYTIQUE_TRANSMUTATION true
#define PRENDRE_EN_COMPTE_BORNE_CATALYTIQUE_DONNER true

std::vector<PositionEchantillon> position_possibles;
std::vector<Echantillon> ensemble_echantillons_donnables;

Echantillon echantillon_du_tour;

/* renvoie la valeur de l'echantillon pour le tour courant */
float avoirValeurPositionMoiTourCourant(
    const PositionEchantillon& position_echantillon)
{
    return avoirValeurPositionEchantillon(
        position_echantillon,
        echantillon_du_tour,
        MOI,
        PRENDRE_EN_COMPTE_BORNE_CATALYTIQUE_PLACEMENT);
}


bool estValeurPlusPetiteEchantillonTourMoi(
    const PositionEchantillon& position_echantillon_1,
    const PositionEchantillon& position_echantillon_2)
{
    return avoirValeurPositionMoiTourCourant(position_echantillon_1)
        < avoirValeurPositionMoiTourCourant(position_echantillon_2);
}

/* renvoie l'element qui pose le plus probleme a l'autre si
 * il est place en `position` */
Element avoirElementEnLequelCatalyser(const Position& position)
{
    return *std::min_element(
            ensemble_elements.begin(),
            ensemble_elements.end(),
            [&](const Element& lhs, const Element& rhs){
                return elementEstAValeurPlusPetite(
                        lhs, rhs,
                        position,
                        AUTRE,
                        PRENDRE_EN_COMPTE_BORNE_CATALYTIQUE_PLACEMENT);});
}

bool estTransmutationNecessaire()
{
    return position_possibles.empty();
}

bool estTaillePlusPetite(const Position& position1, const Position& position2)
{
    return taille_region(position1, MOI) < taille_region(position2, MOI);
}


void transmuteSiNecessaire()
{
    if (!estTransmutationNecessaire())
        return;

    Position meilleure_transmute = avoirMeilleurComposanteATransmuter(MOI,
            PRENDRE_EN_COMPTE_BORNE_CATALYTIQUE_TRANSMUTATION);

    transmuter(meilleure_transmute);
}

void placerEchantillon(const PositionEchantillon& position_echantillon)
{
    placer_echantillon(position_echantillon.pos1,
            position_echantillon.pos2);
}


std::vector<Echantillon> avoirEchantillonsDonnables(const Echantillon& echantillon)
{
    std::vector<Echantillon> echantillon_donnables;
    echantillon_donnables.reserve(NOMBRE_ECHANTILLONS);
    std::copy_if(ensemble_echantillons.begin(), ensemble_echantillons.end(),
            std::back_inserter(echantillon_donnables),
            [&](const Echantillon echantillon_a_donner){
                return estEchantillonDonnable(echantillon_a_donner,
                        echantillon);});
    return echantillon_donnables;
}


Echantillon avoirEchantillonADonnerAleatoire()
{
    Echantillon echantillon_a_donner;
    Element element_a_garder = avoirNemeElementEchantillon(
            avoirNombreAleatoire(0, 1), echantillon_du_tour);
    int index_garde = avoirNombreAleatoire(0, 1);
    avoirNemeElementEchantillon(index_garde, echantillon_a_donner) =
        element_a_garder;
    avoirNemeElementEchantillon(avoirAutreIndexEchantillon(index_garde),
        echantillon_a_donner)
        = ensemble_elements[avoirNombreAleatoire(0, NOMBRE_ELEMENTS - 1)];
    return echantillon_a_donner;
}


void toutTransmuter()
{
    for (Position position : ensembles_positions)
    {
        if (taille_region(position, MOI) > 1)
            transmuter(position);
    }
}

Echantillon avoirEchantillonADonner()
{
    return avoirPireEchantillon(ensemble_echantillons_donnables, AUTRE,
            PRENDRE_EN_COMPTE_BORNE_CATALYTIQUE_DONNER);
}

void atteindreNombreDeCatalyseur(int nombre_catalyseur_a_atteindre)
{
    Erreur erreur = OK;
    std::cout << "Essaie d'atteindre: " << nombre_catalyseur_a_atteindre
        << std::endl;
    Position position_a_transmuter;
    while (nombre_catalyseurs() < nombre_catalyseur_a_atteindre
            && erreur == OK)
    {
        std::cout << "Transmuation tentee" << std::endl;
        position_a_transmuter =
            avoirMeilleurComposanteATransmuterDansContainer(
                avoirComposanteTansmutableEnCatalyseur(MOI),
                MOI,
                false);
        std::cout << "Tenter transmutation: " << position_a_transmuter
            << std::endl;
        erreur = transmuter(position_a_transmuter);
        std::cout << erreur << std::endl;
    }
}

void catalyserAutre()
{
    int maximum_catalyseur = avoirMaximumCatalyseurTransmutable(MOI)
        + nombre_catalyseurs();

    Erreur erreur = OK;
    while (maximum_catalyseur > 0)
    {
        std::list<Position> position_a_catalyser =
            avoirPositionsPourDecouper(AUTRE, maximum_catalyseur,
                PRENDRE_EN_COMPTE_BORNE_CATALYTIQUE_CATALYSE_AUTRE);

        atteindreNombreDeCatalyseur(position_a_catalyser.size());

        Element element_destination;
        for (Position position : position_a_catalyser)
        {
            std::cout << "Catalysation: " << position << std::endl;
            element_destination = avoirElementEnLequelCatalyser(position);
            erreur = catalyser(position, AUTRE, element_destination);
            if (erreur == OK)
                --maximum_catalyseur;
            std::cout << erreur << std::endl;
        }
    }
}

bool cestLeDernierTour()
{
    return tour_actuel() > NB_TOURS - 2;
}

void donnerEchantillon()
{
    donner_echantillon(avoirEchantillonADonner());
}

/// Fonction appelée au début de la partie.
void partie_init()
{
    initialiserEnsemblePosition();
    initialiserEnsembleEchantillon();
    std::srand(std::time(0));
}

void jouer_tour()
{
    echantillon_du_tour = echantillon_tour();
    ensemble_echantillons_donnables =
        avoirEchantillonsDonnables(echantillon_du_tour),
    position_possibles = avoirPositionPossibles(echantillon_du_tour, MOI);

    catalyserAutre();

    while (position_possibles.empty())
    {
        transmuteSiNecessaire();
        position_possibles = avoirPositionPossibles(echantillon_du_tour, MOI);
    }

    auto meilleur_position = std::max_element(position_possibles.begin(),
                position_possibles.end(),
                estValeurPlusPetiteEchantillonTourMoi);
    placerEchantillon(*meilleur_position);
    donnerEchantillon();

    if (cestLeDernierTour())
        toutTransmuter();
}

/// Fonction appelée à la fin de la partie.
void partie_fin()
{
  // fonction a completer
}

