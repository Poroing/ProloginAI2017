#ifndef _H_JJOUVE_NONAPPRENTIDEPENDANT_H_
#define _H_JJOUVE_NONAPPRENTIDEPENDANT_H_

#include <vector>
#include <algorithm>
#include <iterator>
#include <list>

#include "typedef.hh"
#include "log.hh"
#include "coeff.hh"

/* Les fonctions avoirValeur* sont des heuristiques */

/*ensemble de valeurs constantes initialiser en debut de partie*/
std::vector<Position> ensembles_positions;
std::vector<Echantillon> ensemble_echantillons;
std::vector<Element> ensemble_elements = {
    CUIVRE, PLOMB, FER, SOUFRE, MERCURE
};

std::vector<Position> ensemble_adjacences = {
    { 0, 1 },
    { 0, -1 },
    { 1, 0 },
    { -1, 0 }
};

int avoirNombreAleatoire(int inf, int sup)
{
    return inf + std::rand() % (sup - inf + 1);
}


/* Utilitaire pour des valeurs pas défauts */
template<typename Container>
Container& avoirDefautContainer();

template<>
std::vector<Position>& avoirDefautContainer()
{
    return VECTOR_POSITION_DEFAUT;
}

template<>
std::list<Position>& avoirDefautContainer()
{
    return LISTE_POSITION_DEFAUT;
}

/*Fonctions travalaillants sur une matrice de marquage de l'etablie*/
Marquage avoirMarquagePositions()
{
    return std::vector<std::vector<bool> >(
            TAILLE_ETABLI,
            std::vector<bool>(TAILLE_ETABLI, false));
}

bool avoirMarque(Marquage& marquage, const Position& position)
{
    return marquage[position.ligne][position.colonne];
}

void marque(Marquage& marquage, const Position& position)
{
    marquage[position.ligne][position.colonne] = true;
}

template<typename Container>
void marqueContainer(Marquage& marquage, const Container& container)
{
    for (Position position : container)
    {
        marque(marquage, position);
    }
}

void inverserMarque(Marquage& marquage, const Position& position)
{
    marquage[position.ligne][position.colonne] =
        !avoirMarque(marquage, position);
}

void inverserMarquage(Marquage& marquage)
{
    for (Position position : ensembles_positions)
    {
        inverserMarque(marquage, position);
    }
}

/* Renvoie un marquage avec seulement `container` non marque */
template<typename Container>
Marquage avoirMarquagePositionPartielle(const Container& container)
{
    Marquage marquage = avoirMarquagePositions();
    for (Position position : container)
    {
        marque(marquage, position);
    }
    inverserMarquage(marquage);
    return marquage;
}

bool estALExterieur(const Position& position)
{
    return position.ligne < 0
        || position.colonne < 0
        || position.ligne >= TAILLE_ETABLI
        || position.colonne >= TAILLE_ETABLI;
}

Position operator+(const Position& lhs, const Position& rhs)
{
    return Position{lhs.ligne + rhs.ligne, lhs.colonne + rhs.colonne};
}

std::list<Position> avoirCasesAdjacentes(const Position& position)
{
    std::list<Position> cases_adjacentes;
    Position position_courante;
    for (Position direction : ensemble_adjacences)
    {
        position_courante = position + direction;

        if (estALExterieur(position_courante))
            continue;

        cases_adjacentes.push_back(position + direction);
    }
    return cases_adjacentes;
}


std::vector<PositionEchantillon> avoirPositionPossibles(
    const Echantillon& echantillon_a_placer,
    Apprentie apprentie)
{
    std::vector<PositionEchantillon> position_possibles_echantillon;
    position_possibles_echantillon.reserve(SURFACE_ETABLI);
    for (Position position : ensembles_positions)
    {
        for (Position seconde_position : avoirCasesAdjacentes(position))
        {
            if (placement_possible_echantillon(echantillon_a_placer,
                position, seconde_position, apprentie))
            {
                position_possibles_echantillon.push_back(
                    { position, seconde_position });
            }
        }
    }
    return position_possibles_echantillon;
}

Element avoirElementAPosition(const Position& position,
    Apprentie apprentie)
{
    Element element = type_case(position, apprentie);
    return element;
}

bool estVide(const Position& position, Apprentie apprentie)
{
    return avoirElementAPosition(position, apprentie) == VIDE;
}

/* renvoie les cases adjacentes a `position` ayant l'element `element` */
std::list<Position> avoirCasesAdjacentesElement(
        const Position& position,
        Element element,
        Apprentie apprentie)
{
    std::list<Position> cases_adjacentes = avoirCasesAdjacentes(position);
    cases_adjacentes.remove_if([&](const Position& position_adjacente){
                    return avoirElementAPosition(position_adjacente, apprentie)
                        != element;});
    return cases_adjacentes;
}

/* renvoie les cases adjacentes a `position` ayant pour element l'element
 * a `position`
 */
std::list<Position> avoirCasesAdjacentesMemeElement(const Position& position,
        Apprentie apprentie)
{
    return avoirCasesAdjacentesElement(
            position,
            avoirElementAPosition(position, apprentie),
            apprentie);
}

float avoirDistance(const Position& pos1, const Position& pos2)
{
    return std::abs(pos1.ligne - pos2.ligne)
        + std::abs(pos1.colonne - pos2.colonne);
}

float avoirMinimumDistance(
        const Position& position,
        std::list<Position> autres_position)
{
    float minimum_distance = MAXIMUM_DISTANCE;
    for (Position& autre_position : autres_position)
    {
        minimum_distance = std::min(
                avoirDistance(position, autre_position),
                minimum_distance);
    }
    return minimum_distance;
}

/* minimum de la distance entre l'element d'un echantillon et les
 * element adjacents a l'autre element de l'echantillon et qui sont du
 * meme element que l'autre element
 *
 * cette fonction pemet en particulier de gerer l'orientation lors
 * du placement des echantillons
 *
 */
float avoirMinimumDistanceAuElementAdjacent(
        const Echantillon& echantillon,
        const PositionEchantillon& position_echantillon,
        Apprentie apprentie)
{
    return std::min(
            avoirMinimumDistance(
                position_echantillon.pos1,
                avoirCasesAdjacentesElement(
                    position_echantillon.pos2,
                    echantillon.element1,
                    apprentie)),
            avoirMinimumDistance(
                position_echantillon.pos2,
                avoirCasesAdjacentesElement(
                    position_echantillon.pos1,
                    echantillon.element2,
                    apprentie)));
}

float avoirValeurOrientation(
        const Echantillon& echantillon,
        const PositionEchantillon& position_echantillon,
        Apprentie apprentie)
{
    return appliquerCoeffOrientation(
            avoirMinimumDistanceAuElementAdjacent(
                echantillon,
                position_echantillon,
                apprentie),
            apprentie);
}

std::list<Position> avoirComposanteEtMarque(
        Marquage& marquage,
        const Position& position,
        Apprentie apprentie)
{
    if (avoirMarque(marquage, position))
        return LISTE_POSITION_DEFAUT;

    marque(marquage, position);

    std::list<Position> composante = { position };
    std::list<Position> sous_composante_courante;

    for (Position case_adjacente :
            avoirCasesAdjacentesMemeElement(position, apprentie))
    {
        sous_composante_courante =
            avoirComposanteEtMarque(marquage, case_adjacente, apprentie);
        composante.insert(composante.end(),
                sous_composante_courante.begin(),
                sous_composante_courante.end());
    }
    return composante;
}

/* renvoie la taille de la composante dans laquel sera `element` */
int avoirTailleApresPlacementElement(
        Element element,
        const Position& position,
        Apprentie apprentie)
{
    //TODO: certaine composantes peuvent etre compte plusieurs fois
    int taille = 0;
    for (Position direction : ensemble_adjacences)
    {
        if (avoirElementAPosition(position + direction, apprentie) == element)
            taille += taille_region(position + direction, apprentie);
    }
    return taille + 1;
}

/* renvoie la taille des composantes agrandies */
std::pair<int, int> avoirTailleApresPlacementEchantillon(
    const Echantillon& echantillon,
    const PositionEchantillon& position_echantillon,
    Apprentie apprentie)
{
    return std::make_pair(
            avoirTailleApresPlacementElement(
                echantillon.element1, position_echantillon.pos1, apprentie),
            avoirTailleApresPlacementElement(
                echantillon.element2, position_echantillon.pos2, apprentie));
}

int avoirOrProduiteComposante(const Position& representant,
        Apprentie apprentie)
{
    switch(propriete_case(representant, apprentie))
    {
    case TRANSMUTABLE_CATALYSEUR:
        return quantite_transmutation_catalyseur_or(
                taille_region(representant, apprentie));
    case TRANSMUTABLE_OR:
        return quantite_transmutation_or(
                taille_region(representant, apprentie));
    default:
        return 0;
    }
}

int estElementACatalyseur(
        const Position& position,
        Apprentie apprentie)
{
    return propriete_case(position, apprentie) == TRANSMUTABLE_CATALYSEUR;
}

/* renvoie la quantite de catalyseur produit en fonction de la taille
 * de la composante et de l'element dont est constitue la composante
 */
int avoirCatalyseurProduitTailleElement(
        int taille,
        Element element)
{
    if(propriete_case_type(element) == TRANSMUTABLE_CATALYSEUR)
    {
        return quantite_transmutation_catalyseur(taille);
    }
    return 0;
}

/* renvoie la quantite d'or produit en fonction de la taille
 * de la composante et de l'element dont est constitue la composante
 */
int avoirOrProduitTailleElement(
        int taille,
        Element element)
{
    switch(propriete_case_type(element))
    {
    case TRANSMUTABLE_CATALYSEUR:
        return quantite_transmutation_catalyseur_or(taille);
    case TRANSMUTABLE_OR:
        return quantite_transmutation_or(taille);
    default:
        return -3;
    }
}

int avoirCatalyseurProduiteComposante(const Position& representant,
        Apprentie apprentie)
{
    if(propriete_case(representant, apprentie) == TRANSMUTABLE_CATALYSEUR)
    {
        return quantite_transmutation_catalyseur(
                taille_region(representant, apprentie));
    }
    return 0;
}

float avoirValeurEnFonctionDeTaille(int taille, Element element,
        Apprentie apprentie, bool borne_catalytique)
{
    return appliquerCoeffRessource(
        avoirOrProduitTailleElement(taille, element),
        avoirCatalyseurProduitTailleElement(taille, element),
        apprentie,
        borne_catalytique);
}

float avoirValeurPosition(
        const Position& position,
        Element element,
        Apprentie apprentie,
        bool borne_catalytique)
{
    float valeur = avoirValeurEnFonctionDeTaille(
            avoirTailleApresPlacementElement(element, position, apprentie),
            element,
            apprentie,
            borne_catalytique);
    return valeur;
}


float avoirValeurPositionEchantillon(
        const PositionEchantillon& position_echantillon,
        const Echantillon& echantillon,
        Apprentie apprentie, bool borne_catalytique)
{
    float valeur_element_1 = avoirValeurPosition(
            position_echantillon.pos1,
            echantillon.element1,
            apprentie, borne_catalytique);
    float valeur_element_2 = avoirValeurPosition(
            position_echantillon.pos2,
            echantillon.element2,
            apprentie, borne_catalytique);
    float valeur_orientaion = avoirValeurOrientation(
            echantillon,
            position_echantillon,
            apprentie);
    return valeur_element_1 + valeur_element_2 + valeur_orientaion;
}

float avoirValeurComposante(const Position& representant, Apprentie apprentie,
        bool borne_catalytique)
{
    return appliquerCoeffRessource(
        avoirOrProduiteComposante(representant, apprentie),
        avoirCatalyseurProduiteComposante(representant, apprentie),
        apprentie,
        borne_catalytique);
}

float avoirValeurEchantillon(Echantillon echantillon, Apprentie apprentie,
        bool borne_catalytique)
{
    std::vector<PositionEchantillon> echantillon_position_possible =
        avoirPositionPossibles(echantillon, apprentie);
    float max_valeur = MINIMUM_VALEUR_POSITION_ELEMENT;
    for (PositionEchantillon position_echantillon : echantillon_position_possible)
    {
        max_valeur = std::max(max_valeur, avoirValeurPositionEchantillon(
                    position_echantillon, echantillon, apprentie,
                    borne_catalytique));
    }
    return max_valeur;
}

/* renvoie les composante contenue dans `container_positions` */
template<typename Container>
std::vector<Position> avoirRepresentantsComposantes(
        Container container_positions,
        Apprentie apprentie,
        bool garder_vide=false)
{
    Marquage composante_verifier =
        avoirMarquagePositionPartielle(container_positions);
    std::vector<Position> representants;
    representants.reserve(SURFACE_ETABLI);
    for (Position position : container_positions)
    {
        if (!avoirMarque(composante_verifier, position)
                && !(estVide(position, apprentie) && !garder_vide))
        {
            avoirComposanteEtMarque(composante_verifier, position, apprentie);
            representants.push_back(position);
        }
    }

    return representants;
}

/* renvoie toutes les composante de l'etabli */
std::vector<Position> avoirRepresentantsComposantesTotal(
        Apprentie apprentie,
        bool garder_vide=false)
{
    return avoirRepresentantsComposantes(ensembles_positions,
            apprentie, garder_vide);
}

bool positionEstAValeurPlusPetite(const Position& lhs,
        const Position& rhs, Element element, Apprentie apprentie,
        bool borne_catalytique)
{
    return avoirValeurPosition(lhs, element, apprentie, borne_catalytique)
        < avoirValeurPosition(rhs, element, apprentie, borne_catalytique);
}

bool elementEstAValeurPlusPetite(Element lhs, Element rhs,
        const Position& position, Apprentie apprentie, bool borne_catalytique)
{
    return avoirValeurPosition(position, lhs, apprentie, borne_catalytique)
        < avoirValeurPosition(position, rhs, apprentie, borne_catalytique);
}


bool composanteEstAValeurPlusPetite(const Position& lhs,
        const Position& rhs, Apprentie apprentie, bool borne_catalytique)
{
    return avoirValeurComposante(lhs, apprentie, borne_catalytique)
        < avoirValeurComposante(rhs, apprentie, borne_catalytique);
}

template<typename Container>
Position avoirMeilleurComposanteATransmuterDansContainer(
        const Container& container,
        Apprentie apprentie,
        bool borne_catalytique)
{
    if (container.empty())
        return AUCUNE_POSITION;
    return *std::max_element(
            container.begin(),
            container.end(),
            [&](const Position& lhs, const Position& rhs){
                return composanteEstAValeurPlusPetite(lhs, rhs, apprentie,
                        borne_catalytique);});
}

/* renvoie la meilleure composante a transmuter dont certains element sont
 * contenue dans des conteuneur dans `container`
 */
template<typename ContainerOfContainer>
typename ContainerOfContainer::value_type&
avoirMeilleurContainerATransmuterDansContainer(
        ContainerOfContainer& container,
        Apprentie apprentie,
        bool borne_catalytique)
{
    typedef typename ContainerOfContainer::value_type SousContainer;

    if (container.empty())
        return avoirDefautContainer<SousContainer>();

    bool container_invalide = std::any_of(container.begin(), container.end(),
            [&](const SousContainer& sous_container){
                return sous_container.empty();});

    if (container_invalide)
    {
        return avoirDefautContainer<SousContainer>();
    }

    return *max_element(
            container.begin(),
            container.end(),
            [&](const SousContainer& lhs,
                const SousContainer& rhs)
            {
                return composanteEstAValeurPlusPetite(
                        lhs.front(), rhs.front(), apprentie, borne_catalytique);
            });
}

Position avoirMeilleurComposanteATransmuter(Apprentie apprentie,
        bool borne_catalytique)
{
    return avoirMeilleurComposanteATransmuterDansContainer(
            avoirRepresentantsComposantesTotal(apprentie),
            apprentie, borne_catalytique);
}

int avoirIndiceElement(Element element)
{
    switch(element)
    {
    case CUIVRE:
        return 0;
    case PLOMB:
        return 1;
    case FER:
        return 2;
    case SOUFRE:
        return 4;
    default:
        return -1;
    }
}

Element& avoirNemeElementEchantillon(int index, Echantillon& echantillon)
{
    switch(index)
    {
    case 0:
        return echantillon.element1;
    case 1:
        return echantillon.element2;
    default:
        return ELEMENT_DEFAUT;
    }
}

bool estElementPresentEchantillon(Element element, Echantillon echantillon)
{
    return echantillon.element1 == element
        || echantillon.element2 == element;
}

bool estEchantillonDonnable(Echantillon echantillon_donne,
    Echantillon echantillon)
{
    return estElementPresentEchantillon(echantillon_donne.element1,
            echantillon)
        || estElementPresentEchantillon(echantillon_donne.element2,
            echantillon);
}

int avoirAutreIndexEchantillon(int index)
{
    return 1 - index;
}

Echantillon avoirMeilleureEchantillon(
        const std::vector<Echantillon>& ensemble_echantillons,
        Apprentie apprentie, bool borne_catalytique)
{
    return *std::max_element(ensemble_echantillons.begin(),
            ensemble_echantillons.end(),
            [&](const Echantillon& lhs, const Echantillon& rhs){
                return avoirValeurEchantillon(lhs, apprentie,
                        borne_catalytique)
                    < avoirValeurEchantillon(rhs, apprentie,
                            borne_catalytique);});

}

Echantillon avoirPireEchantillon(
        const std::vector<Echantillon>& ensemble_echantillons,
        Apprentie apprentie,
        bool borne_catalytique)
{
    return *std::min_element(ensemble_echantillons.begin(),
            ensemble_echantillons.end(),
            [&](const Echantillon& lhs, const Echantillon& rhs){
                return avoirValeurEchantillon(lhs, apprentie,
                        borne_catalytique)
                    < avoirValeurEchantillon(rhs, apprentie,
                            borne_catalytique);});

}


/* revoie un ensemble de position coupant en deux la composante donne */
std::list<Position> avoirDecoupageComposanteProfondeur(
        const Position& representant,
        Apprentie apprentie)
{

    Marquage marquage = avoirMarquagePositions();
    std::list<Position> profondeur_courante = { representant };
    marque(marquage, representant);
    std::list<Position> profondeur_suivante;
    int taille_composante = taille_region(representant, apprentie);
    int nombre_position_passe = 0;

    while (nombre_position_passe < taille_composante / 2)
    {
        profondeur_suivante.clear();
        for (Position position_courante : profondeur_courante)
        {
            for (Position position_suivante :
                    avoirCasesAdjacentesMemeElement(position_courante,
                        apprentie))
            {
                if (avoirMarque(marquage, position_suivante))
                    continue;
                marque(marquage, position_suivante);
                profondeur_suivante.push_back(position_suivante);
            }
            nombre_position_passe += 1;
        }
        profondeur_courante = std::move(profondeur_suivante);
    }
    return profondeur_courante;
}

int avoirTailleComposanteEtMarquer(
        Marquage& marquage,
        const Position& position,
        Apprentie apprentie)
{
    return avoirComposanteEtMarque(marquage, position, apprentie).size();
}

std::list<Position> avoirComposante(
        const Position& representant,
        Apprentie apprentie)
{
    std::vector<Position> composante =
        positions_region(representant, apprentie);
    return std::list<Position>(composante.begin(), composante.end());
}

int avoirTailleComposante(const Position& representant, Apprentie apprentie)
{
    return taille_region(representant, apprentie);
}

/* renvoie la taille de la plus grande composante apres le decoupage
 * avec l'ensemble `decoupage` */
int avoirMaximumTailleComposanteApresDecoupage(
        std::list<Position> decoupage,
        Apprentie apprentie)
{
    if (decoupage.empty())
        return -1;

    Marquage marquage = avoirMarquagePositions();
    marqueContainer(marquage, decoupage);
    std::list<Position> composante_decoupe =
        avoirComposante(decoupage.front(), apprentie);
    int maximum_taille_composante = 0;
    for (Position position : composante_decoupe)
    {
        if (avoirMarque(marquage, position))
            continue;
        maximum_taille_composante = std::max(
                avoirTailleComposanteEtMarquer(marquage, position, apprentie),
            maximum_taille_composante);
    }
    return maximum_taille_composante;
}

std::list<std::list<Position> > avoirEnsembleDeTaille(
        int n,
        std::list<Position> ensemble)
{
    std::list<std::list<Position> > resultat;
    if (n <= 0)
        return resultat;
    std::list<std::list<Position> > sous_ensemble_courant;
    std::list<Position>::iterator emplacement_courant;
    Position position_courante;
    int nombre_iteration = 0;
    for (auto position_it = ensemble.begin();
            position_it != ensemble.end();
            ++position_it)
    {
        ++nombre_iteration;
        position_courante = *position_it;
        emplacement_courant = ensemble.erase(position_it);
        sous_ensemble_courant = avoirEnsembleDeTaille(
                n - 1,
                ensemble);
        for (std::list<Position> sous_ensemble : sous_ensemble_courant)
        {
            sous_ensemble.push_back(position_courante);
        }
        resultat.insert(resultat.end(),
                sous_ensemble_courant.begin(),
                sous_ensemble_courant.end());
        ensemble.push_front(position_courante);
        position_it = emplacement_courant;
        --position_it;
    }

    return resultat;
}

/* decoupage degueulasse inutile et une perte de temps plus qu'enorme */
std::list<Position> avoirDecoupageComposanteGlouton(
        const Position& representant,
        Apprentie apprentie)
{
    //UN PEU TROP GLOUTON
    Marquage marquage = avoirMarquagePositions();
    std::list<Position> composante = avoirComposanteEtMarque(
            marquage,
            representant,
            apprentie);

    std::list<Position> decoupage;
    int taille_maximum_minimum = SURFACE_ETABLI;
    int taille_maximum_courante;
    bool decoupe = false;
    int taille_decoupe = 1;
    while (!decoupe && taille_decoupe < (int) composante.size())
    {

        for (std::list<Position> decoupage_courant :
                avoirEnsembleDeTaille(taille_decoupe, composante))
        {
            taille_maximum_courante =
                avoirMaximumTailleComposanteApresDecoupage(
                        decoupage_courant,
                        apprentie);
            if (taille_maximum_courante
                    >= (int) (composante.size() - decoupage_courant.size()))
            {
                continue;
            }
            decoupe = true;
            if (taille_maximum_courante < taille_maximum_minimum)
            {
                decoupage = decoupage_courant;
                taille_maximum_minimum = taille_maximum_courante;
            }
        }
        ++taille_decoupe;
    }
    return decoupage;
}

std::list<Position> avoirDecoupageComposante(
        const Position& representant,
        Apprentie apprentie)
{
    return avoirDecoupageComposanteProfondeur(representant, apprentie);
}

/* pas terminer, aurait pus servir a optimiser le decoupage */
bool estBordDeComposante(const Position& position, Apprentie apprentie)
{
    //Pas bon pour le moment
    std::list<Position> cases_adjacentes = avoirCasesAdjacentesMemeElement(
            position, apprentie);
    if (cases_adjacentes.size() > 2)
        return false;
    if (cases_adjacentes.size() < 1)
        return true;
    return false;
}

/* renvoie les position qui permetrait de decouper une bonne composante */
std::list<Position> avoirPositionsPourDecouper(
        Apprentie apprentie,
        int maximum_catalyseur,
        bool borne_catalytique)
{

    std::vector<Position> composantes =
        avoirRepresentantsComposantesTotal(apprentie);

    if (composantes.empty())
    {
        return std::list<Position>();
    }

    std::list<std::list<Position> > decoupages_composantes;
    std::transform(composantes.begin(), composantes.end(),
            std::back_inserter(decoupages_composantes),
            [&](const Position& composante){
                return avoirDecoupageComposante(composante, apprentie);
                });

    decoupages_composantes.remove_if(
            [&](const std::list<Position>& decoupage){
                return (int) decoupage.size() > maximum_catalyseur
                    || decoupage.empty();});

    std::list<Position> decoupage =
        avoirMeilleurContainerATransmuterDansContainer(
            decoupages_composantes,
            apprentie,
            borne_catalytique);

    return decoupage;
}

void initialiserEnsemblePosition()
{
    ensembles_positions.reserve(SURFACE_ETABLI);
    for (int i = 0; i < TAILLE_ETABLI; ++i)
    {
        for (int j = 0; j < TAILLE_ETABLI; ++j)
        {
            ensembles_positions.push_back(position{ i , j });
        }
    }
}

void initialiserEnsembleEchantillon()
{
    ensemble_echantillons.reserve(NOMBRE_ECHANTILLONS);
    for (int i = 0; i < NOMBRE_ELEMENTS; ++i)
    {
        for (int j = 0; j < NOMBRE_ELEMENTS; ++j)
        {
            ensemble_echantillons.push_back(Echantillon{
                    ensemble_elements[i],
                    ensemble_elements[j]});
        }
    }
}

int avoirIndexApprentie(Apprentie apprentie)
{
    if (apprentie == MOI)
        return 0;
    return 1;
}

std::vector<Position> avoirComposanteTansmutableEnCatalyseur(
        Apprentie apprentie)
{
    std::vector<Position> composante = avoirRepresentantsComposantesTotal(
            apprentie);
    std::vector<Position> composante_transmutable_en_catalyseur;

    std::copy_if(composante.begin(), composante.end(),
            std::back_inserter(composante_transmutable_en_catalyseur),
            [&](const Position& position){
                return estElementACatalyseur(position, apprentie);
                });

    return composante_transmutable_en_catalyseur;
}

/* renvoie le nombre de catalyseurs produit si on transmute toute les
 * composante donne
 */
int avoirNombreCatalyseurProduits(
        std::vector<Position> representants,
        Apprentie apprentie)
{
    int nombre_catalyseur_produit = 0;
    for (Position representant : representants)
    {
        nombre_catalyseur_produit +=
            avoirCatalyseurProduiteComposante(representant, apprentie);
    }
    return nombre_catalyseur_produit;
}

int avoirMaximumCatalyseurTransmutable(Apprentie apprentie)
{
    std::vector<Position> composante = avoirRepresentantsComposantesTotal(
            apprentie);

    int maximum = avoirNombreCatalyseurProduits(composante, apprentie);

    return maximum;
}

#endif
