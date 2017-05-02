#ifndef _H_JJOUVE_LOG_H_
#define _H_JJOUVE_LOG_H_

#include "typedef.hh"
#include <iostream>
#include <chrono>
#include <list>

std::chrono::time_point<std::chrono::system_clock> derniere_initialisation;
using time_unit = std::chrono::duration<float>;

void reinitialiserTemps()
{
    derniere_initialisation = std::chrono::system_clock::now();
}

auto avoirTempsDepuisInitialisation()
{
    return std::chrono::duration_cast<time_unit>(
            std::chrono::system_clock::now() - derniere_initialisation).count();
};

std::ostream& operator<<(std::ostream& os, Erreur element)
{
    switch(element)
    {
    case OK:
        os << "Tout s'est bien passe";
        break;
    case POSITION_INVALIDE:
        os << "La position n'est pas sur l'etablie";
        break;
    case PLACEMENT_INVALIDE:
        os << "Les positions choisie ne sont pas adjacentes";
        break;
    case PLACEMENT_INCORRECT:
        os << "La position n'est pas adjacente a un meme element";
        break;
    case PLACEMENT_IMPOSSIBLE:
        os << "La position est non vide";
        break;
    case CASE_VIDE:
        os << "La position est vide";
        break;
    case ECHANTILLON_INCOMPLET:
        os << "L'echantillon n'as qu'un element";
        break;
    case ECHANTILLON_INVALIDE:
        os << "L'echantillon n'as pas d'element en commun";
        break;
    case AUCUN_CATALYSEUR:
        os << "Aucun catalyseur disponible";
        break;
    case CATALYSE_INVALIDE:
        os << "Catalysation sur zone vide";
        break;
    case DEJA_POSE:
        os << "Echantillon deja pose";
        break;
    case DEJA_DONNE:
        os << "Echantillon deja donne";
    }
    return os;
}

template<typename T>
std::ostream& operator<<(std::ostream& os, const std::list<T>& rhs)
{
    os << "[";
    for (T v : rhs)
    {
        os << v << "; ";
    }
    os << "]";
    return os;
}

template<typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& rhs)
{
    os << "[";
    for (T v : rhs)
    {
        os << v << "; ";
    }
    os << "]";
    return os;
}

std::ostream& operator<<(std::ostream& os, Element element)
{
    switch(element)
    {
    case VIDE:
        os << "VIDE";
        break;
    case PLOMB:
        os << "PLOMB";
        break;
    case FER:
        os << "FER";
        break;
    case CUIVRE:
        os << "SOUFRE";
        break;
    case MERCURE:
        os << "MERCURE";
        break;
    case SOUFRE:
        os << SOUFRE;
        break;
    }
    return os;
}


std::ostream& operator<<(std::ostream& os, const Position& position)
{
    os << "(" << position.ligne << ", " << position.colonne << ")";
    return os;
}

std::ostream& operator<<(std::ostream& os,
        const PositionEchantillon& position_echantillon)
{
    os << "{" << position_echantillon.pos1 << ", "
        << position_echantillon.pos2 << "}";
    return os;
}

std::ostream& operator<<(std::ostream& os, const Echantillon& echantillon)
{
    os << "[" << echantillon.element1 << ", " << echantillon.element2 << "]";
    return os;
}

#endif

