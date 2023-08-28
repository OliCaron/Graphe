/**
 * \file ReseauInterurbain.h
 * \brief Gestion d'un réseau de transport interurbain.
 * \author Olivier Caron
 * \version 0.1
 * \date juin-juillet 2023
 *
 *  Travail pratique numéro 2
 *
 */

#include "Graphe.h"
#include <iostream>
#include <string>
#include <stdexcept>
#include <climits>
#include <vector>
#include <list>
#include <stack>
//vous pouvez inclure d'autres librairies de la STL si vous pensez les utiliser


#ifndef _RESEAUINTERURBAIN__H
#define _RESEAUINTERURBAIN__H

namespace TP2
{

struct Chemin 
{
   //listeVilles doit contenir la liste des villes formant le chemin
   //La ville de départ du chemin correspond à l'élément d'indice 0 du vecteur
   //la ville d'arrivée correspond au dernier élément du vecteur.
   std::vector<std::string> listeVilles;

   float dureeTotale;		// Durée totale du trajet en heures
   float coutTotal;			// Cout total en dollars du chemin entre la ville de départ et la ville de destination
   bool reussi;				// Un booléen qui vaut true si le chemin a été trouvé, false sinon
};

class ReseauInterurbain{

public:

	// Constructeur
	ReseauInterurbain(std::string nomReseau, size_t nbVilles = 10);
	
	// Destructeur
	~ReseauInterurbain();

	// Change la taille du réseau en utilisant un nombre de villes = nouvelleTaille
	void resize(size_t nouvelleTaille);

	//Ne touchez pas à cette fonction !
	friend std::ostream& operator<<(std::ostream& out, const ReseauInterurbain& r)
	{
		out << r.unReseau << std::endl;
		return out;
	}
                    
	// Charger un réseau à partir d'un fichier texte en entrée (voir format du fichier dans l'énoncé du Tp).
	// Construit un réseau routier à partir d'une liste de villes ainsi que leurs liens.
	// fichierEntree est ouvert corectement. 
	// fichierEntree n'est pas fermé par la fonction.
	// Exception logic_error si fichierEntree n'est pas ouvert correctement.
	void chargerReseau(std::ifstream & fichierEntree); 
                     
	// Trouve le plus court chemin entre une ville source et une ville destination en utilisant l'algorithme de Dijkstra
	// si dureeCout = true, on utilise la durée comme pondération au niveau des trajets
	// si dureeCout = false, on utilise le coût (en $) comme pondération au niveau des trajets
	// Exception std::logic_error si origine et/ou destination absent du réseau
	Chemin rechercheCheminDijkstra(const std::string& origine, const std::string& destination, bool dureeCout) const;

	// Trouve l’ensemble des composantes fortement connexes en utilisant l'algorithme de Kosaraju
	// Retourne un vecteur de vecteurs de chaînes caractères. Chaque sous-vecteur représente une composante.
	std::vector<std::vector<std::string> > algorithmeKosaraju();

	//Vous pouvez ajoutez d'autres méthodes publiques si vous sentez leur nécessité

    std::stack<std::string> exploreRecursifGrapheDFS(const Graphe &graphe);

private:

	Graphe unReseau;			//Le type ReseauRoutier est composé d'un graphe
	std::string nomReseau;		// Le nom du reseau (exemple: Orleans Express)

	// Vous pouvez définir des constantes ici. À vous de voir!

	//Vous pouvez ajoutez d'autres méthodes privées si vous sentez leur nécessité
    /**
     * \struct infoDFS
     * \brief Struct contenant un graphe, un stack de string représentant les sommets abandonnés et un vector de booleen représentant les sommets visités
     */
    using InfoDFS =  struct infoDFS {
        Graphe graphe ;
        std::stack<std::string> abandonnes ;
        std::vector<bool> visites ;

        explicit infoDFS(const Graphe& g) : graphe(g), abandonnes(), visites(g.taille(), false) {}
    } ;

    size_t localiserSommetMinimal(std::vector<size_t>& nonResolus, std::vector<float>& ponderations) const;

    void relaxer(size_t voisin, size_t courant, std::vector<float>& ponderations, std::vector<std::string>& listeVilles, bool dureeCout) const;

    void auxExploreRecursifDFS(InfoDFS& donneesDFS, size_t depart);

    std::vector<std::string> transfererPileVersVecteur(std::stack<std::string> &pile);

    std::vector<std::string> construireChemin(const std::string& source, const std::string& destination, std::vector<std::string> predecesseurs, Chemin& cheminTrouve) const;

};

}//Fin du namespace

#endif
