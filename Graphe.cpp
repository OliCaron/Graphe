/**
 * \file Graphe.cpp
 * \brief Implémentation d'un graphe orienté.
 * \author Olivier Caron
 * \version 0.1
 * \date juin-juillet 2023
 *
 *  Travail pratique numéro 2
 *
 */

#include "Graphe.h"
//vous pouvez inclure d'autres librairies si c'est nécessaire

using namespace std;

namespace TP2
{
	//Ajoutez l'implémentation de vos méthodes ici.
    /**
   * \fn  Graphe::Graphe(size_t nbSommets)
    * \brief Constructeur avec parametre
   *  Initialisation de listesAdj, de nbSommets et de noms de l'objet Graphe selon les valeurs recues en parametre
   * \param[in] nbSommets le nombre de sommets du graphe
   * \return Un objet Graphe
   */
    Graphe::Graphe(size_t nbSommets) : listesAdj(nbSommets), nbSommets(nbSommets), nbArcs(0), noms(nbSommets) {
    }

    /**
     * \fn Graphe::~Graphe()
    * \brief Destructeur de la classe Graphe
    */
    Graphe::~Graphe(){
    }

    /**
    * \fn void resize(size_t nouvelleTaille)
    * \brief Fonction servant à redéfinir la taille d'un graphe.
    * \param[in] nouvelleTaille la nouvelle taille du graphe
    */
    void Graphe::resize(size_t nouvelleTaille){
        nbSommets = nouvelleTaille;
        listesAdj.resize(nouvelleTaille);
        noms.resize(nouvelleTaille);
    }

    /**
    * \fn void nommer(size_t sommet, const std::string& nom)
    * \brief Fonction servant à nommer le sommet d'un graphe.
    * \param[in] sommet le sommet à renommer
    * \param[in] nom le nom à donner au sommet
    * \exception logic_error si le sommet n'existe pas
    */
    void Graphe::nommer(size_t sommet, const std::string& nom){
        if(!sommetExiste(sommet)) throw logic_error("Graphe::nommer : sommet inexistant");
        noms.at(sommet) = nom;
    }

    /**
    * \fn void ajouterArc(size_t source, size_t destination, float duree, float cout)
    * \brief Fonction servant à ajouter un arc entre deux sommets d'un graphe
    * \param[in] source le sommet source de l'arc à ajouter
    * \param[in] destination le sommet destination de l'arc à ajouter
    * \param[in] duree la pondération durée de l'arc à ajouter
    * \param[in] cout la pondération cout de l'arc à ajouter
     * \exception logic_error si le sommet source n'existe pas
     * \exception logic_error si le destination n'existe pas
     * \exception logic_error si l'arc existe déjà
    */
    void Graphe::ajouterArc(size_t source, size_t destination, float duree, float cout){
        if(!sommetExiste(source)) throw logic_error("Graphe::ajouterArc : la source est invalide");
        if(!sommetExiste(destination)) throw logic_error("Graphe::ajouterArc : la destination est invalide");
        if(arcExiste(source, destination)) throw logic_error("Graphe::ajouterArc : l'arc existe déjà");

        Ponderations p;
        p.cout = cout;
        p.duree = duree;

        listesAdj.at(source).emplace_back(destination, p);
        nbArcs++;
    }

    /**
    * \fn void enleverArc(size_t source, size_t destination)
    * \brief Fonction servant à enlever un arc entre deux sommets d'un graphe
    * \param[in] source le sommet source de l'arc à enlever
    * \param[in] destination le sommet destination de l'arc à enlever
    * \exception logic_error si le sommet source n'existe pas
    * \exception logic_error si le sommet destination n'existe pas
    * \exception logic_error si l'arc n'existe pas
    */
    void Graphe::enleverArc(size_t source, size_t destination){
        if(!sommetExiste(source)) throw logic_error("Graphe::enleverArc : la source est invalide");
        if(!sommetExiste(destination)) throw logic_error("Graphe::enleverArc : la destination est invalide");

        auto& liste = listesAdj.at(source) ;
        auto it = find_if(liste.begin(), liste.end(), [&destination](Arc e) {return e.destination == destination ; }) ;

        if (it != liste.end())
        {
            liste.erase(it) ;
            nbArcs--;
        }
        else throw logic_error("Graphe::enleverArc : l'arc n'existe pas dans le graphe") ;
    }

    /**
    * \fn bool arcExiste(size_t source, size_t destination) const
    * \brief Fonction servant à savoir si un arc existe entre deux sommets d'un graphe
    * \param[in] source le sommet source de l'arc à vérifier
    * \param[in] destination le sommet destination de l'arc à vérifier
    * \exception logic_error si le sommet source n'existe pas
    * \exception logic_error si le sommet destination n'existe pas
    * \return bool true si l'arc existe, false sinon
    */
    bool Graphe::arcExiste(size_t source, size_t destination) const{
        if(!sommetExiste(source)) throw logic_error("Graphe::arcExiste : la source est invalide");
        if(!sommetExiste(destination)) throw logic_error("Graphe::arcExiste : la destination est invalide");

        auto liste = listesAdj.at(source);
        for (auto element : liste)
        {
            if(element.destination == destination)
            {
                return true;
            }
        }
        return false;
    }

    /**
    * \fn std::vector<size_t> listerSommetsAdjacents(size_t sommet) const
    * \brief Fonction servant à lister tous les voisins d'un sommet d'un graphe
    * \param[in] sommet le sommet à partir duquel nous devons énumérer les voisins
    * \exception logic_error si le sommet n'existe pas
    * \return sommetsAdjacents un vecteur de size_t contenant tous les sommets adjacents au sommet passé en paramètre
    */
    std::vector<size_t> Graphe::listerSommetsAdjacents(size_t sommet) const
    {
        if(!sommetExiste(sommet)) throw logic_error("Graphe::listerSommetsAdjacents : le sommet est invalide");

        vector<size_t> sommetsAdjacents;

        auto liste = listesAdj.at(sommet);
        for(auto arc : liste)
        {
            sommetsAdjacents.push_back(arc.destination);
        }

        return sommetsAdjacents;
    }

    /**
    * \fn std::string getNomSommet(size_t sommet) const
    * \brief Fonction servant à connaître le nom associé au sommet d'un graphe
    * \param[in] sommet le sommet dont nous voulons savoir le nom
    * \exception logic_error si le sommet n'existe pas
    * \return std::string le nom du sommet
    */
    std::string Graphe::getNomSommet(size_t sommet) const
    {
        if(!sommetExiste(sommet)) throw logic_error("Graphe::getNomSommet : le sommet est invalide");
        return noms.at(sommet);
    }

    /**
    * \fn std::string getNumeroSommetconst std::string& nom) const
    * \brief Fonction servant à connaître le numéro associé au nom du sommet d'un graphe
    * \param[in] nom le nom dont nous voulons savoir le numéro de sommet
     * \exception logic_error si le nom n'existe pas
    * \return size_t le numéro du sommet
    */
    size_t Graphe::getNumeroSommet(const std::string& nom) const
    {
        for (int i = 0; i < noms.size(); ++i)
        {
            if (noms[i] == nom) return i;
        }
        throw logic_error("Graphe::getNumeroSommet : Le nom n'existe pas");
    }

    /**
     * \fn  getNombreSommets() const
    * \brief Donne le nombre de sommets du graphe
    * \return int nbSommets le nombre de sommets du graphe
    */
    int Graphe::getNombreSommets() const
    {
        return nbSommets;
    }

    /**
     * \fn  getNombreArcs() const
    * \brief Donne le nombre d'arcs du graphe
    * \return int nbArcs le nombre d'arcs du graphe
    */
    int Graphe::getNombreArcs() const
    {
        return nbArcs;
    }

    /**
    * \fn bool getPonderationsArc(size_t source, size_t destination) const
    * \brief Fonction servant à savoir la pondération entre deux sommets d'un graphe
    * \param[in] source le sommet source de l'arc à vérifier
    * \param[in] destination le sommet destination de l'arc à vérifier
    * \exception logic_error si le sommet source n'existe pas
    * \exception logic_error si le destination n'existe pas
    * \exception logic_error si l'arc n'existe pas
    * \return Ponderations la struct Pondération de l'arc contenant la durée et le coût
    */
    Ponderations Graphe::getPonderationsArc(size_t source, size_t destination) const
    {
        if (!sommetExiste(source)) throw logic_error("Graphe::getPonderationsArc : la source est invalide");
        if(!sommetExiste(destination)) throw logic_error("Graphe::getPonderationsArc : la destination est invalide");
        if(!arcExiste(source, destination)) throw logic_error("Graphe::getPonderationsArc : l'arc n'existe pas");

        auto liste = listesAdj.at(source);
        for(auto arc : liste)
        {
            if(arc.destination == destination)
            {
                return arc.poids;
            }
        }
    }

    /**
    * \fn Graphe grapheInverse() const
    * \brief Fonction servant à inverser un Graphe et à le retourner
    * \return Graphe inverse le graphe inversé
    */
    Graphe Graphe::grapheInverse() const {

        Graphe inverse(listesAdj.size()) ;

        for (size_t depart = 0; depart < listesAdj.size(); ++depart) {
            auto liste = listesAdj.at(depart) ;
            for (auto arc: liste) {
                inverse.ajouterArc(arc.destination, depart, arc.poids.duree, arc.poids.cout) ;
            }
        }

        return inverse ;
    }

    /**
    * \fn size_t ariteEntree(size_t sommet) const
    * \brief Fonction servant à connaître l'arité d'entrée d'un sommet
    * \param[in] sommet le sommet dont nous voulons savoir l'arité d'entrée
    * \exception logic_error si le sommet n'existe pas
    * \return size_t l'arité d'entrée
    */
    size_t Graphe::ariteEntree(size_t sommet) const {
        if (!sommetExiste(sommet)) throw std::invalid_argument("ariteEntree: sommet invalide.") ;
        auto arite = 0 ;

        for (const auto& liste: listesAdj)
            for (auto elem: liste) if (elem.destination == sommet) ++ arite ;

        return arite ;
    }


    /**
    * \fn size_t ariteSortie(size_t sommet) const
    * \brief Fonction servant à connaître l'arité de sortie d'un sommet
    * \param[in] sommet le sommet dont nous voulons savoir l'arité de sortie
     * \exception logic_error si le sommet n'existe pas
    * \return size_t l'arité de sortie
    */
    size_t Graphe::ariteSortie(size_t sommet) const {
        if (!sommetExiste(sommet)) throw logic_error("Graphe::ariteSortie : le sommet est invalide");
        return listesAdj.at(sommet).size() ;
    }

    /**
    * \fn void retirerSommet(size_t sommet)
    * \brief Fonction servant à retirer le sommet d'un graphe
    * \param[in] sommet le sommet à retirer
     * \exception logic_error si le sommet n'existe pas
    */
    void Graphe::retirerSommet(size_t sommet) {
        if (!sommetExiste(sommet)) throw logic_error("Graphe::retirerSommet: sommet inexistant") ;

        listesAdj.erase(listesAdj.begin() + static_cast<std::vector<size_t>::difference_type> (sommet)) ;

        for (auto& liste: listesAdj) {
            auto it = std::find_if(liste.begin(), liste.end(), [sommet](Arc element) {return element.destination == sommet; }) ;
            if (it != liste.end()) liste.erase(it) ;
        }

        for (auto& liste: listesAdj) {
            for (auto& voisin: liste) if (voisin.destination > sommet) --voisin.destination ;
        }
        nbSommets--;
    }

    /**
    * \fn  size_t taille() const
    * \brief Donne la taille d'un graphe
    * \return size_t la taille du graphe
    */
    size_t Graphe::taille() const {
        return listesAdj.size() ;
    }

    /**
    * \fn  bool sommetExiste(size_t numero) const
    * \brief Fonction servant à savoir si un sommet existe dans le graphe
    * \return bool true si le sommet existe, false sinon
    */
    bool Graphe::sommetExiste(size_t numero) const {
        return numero < nbSommets ;
    }

    /**
    * \fn  std::vector<size_t> triTopologique()
    * \brief Fonction servant faire le tri topologique d'un graphe
    * \exception invalid_argument si le graphe est cyclique
    * \return std::vector<size_t> un vecteur comprenant les numéros de sommets dans l'ordre topologique
    */
    std::vector<size_t> Graphe::triTopologique() {

        std::vector<size_t> tri(taille(), taille()) ;
        std::vector<size_t> index(taille()) ;
        std::iota(index.begin(), index.end(), 0) ;
        size_t restants = taille() ;

        while (restants > 0) {
            size_t i = 0 ;

            while (i < taille() && ariteSortie(i) != 0) ++i ;
            if (i == taille())
                throw std::invalid_argument("Tri topologique: tentative de trier un graphe cyclique") ;

            retirerSommet(i) ;
            tri.at(-- restants) = index.at(i) ;
            index.erase(index.begin() + static_cast<std::vector<size_t>::difference_type> (i)) ;
        }
        return tri ;
    }

}//Fin du namespace
