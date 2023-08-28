/**
 * \file ReseauInterurbain.cpp
 * \brief Implémentattion de la classe ReseauInterurbain.
 * \author Olivier Caron
 * \version 0.1
 * \date juin-juillet 2023
 *
 *  Travail pratique numéro 2
 *
 */
#include <sstream>
#include <fstream>
#include "ReseauInterurbain.h"
//vous pouvez inclure d'autres librairies si c'est nécessaire

namespace TP2
{
    /**
    * \fn  ReseauInterurbain::ReseauInterurbain(std::string nomReseau, size_t nbVilles)
    * \brief Constructeur avec parametre
    *  Initialisation de nomReseau et de unReseau de l'objet ReseauInterurbain selon les valeurs recues en parametre
     *  \param[in] nomReseau le nom du réseau
    * \param[in] nbVilles le nombre de villes du réseau
    * \return Un objet Graphe
    */
    ReseauInterurbain::ReseauInterurbain(std::string nomReseau, size_t nbVilles) : nomReseau(nomReseau), unReseau(nbVilles){
    }

    /**
   * \fn ReseauInterurbain::~ReseauInterurbain()
  * \brief Destructeur de la classe ReseauInterurbain
  */
    ReseauInterurbain::~ReseauInterurbain() {

    }

    /**
    * \fn void resize(size_t nouvelleTaille)
    * \brief Fonction servant à redéfinir la taille d'un réseau.
    * \param[in] nouvelleTaille la nouvelle taille du réseau
    */
    void ReseauInterurbain::resize(size_t nouvelleTaille){
        unReseau.resize(nouvelleTaille);
    }

    /**
    * \fn void chargerReseau(std::ifstream & fichierEntree)
    * \brief Fonction servant à charger un réseau.
    * \param[in] fichierEntree le fichier contenant toutes les villes du réseau
     * \exception logic_error si le fichier n'est pas ouvert correctement
    */
    // Méthode fournie
    void ReseauInterurbain::chargerReseau(std::ifstream & fichierEntree)
    {
        if (!fichierEntree.is_open())
            throw std::logic_error("ReseauInterurbain::chargerReseau: Le fichier n'est pas ouvert !");

        std::string buff;

        getline(fichierEntree, nomReseau);
        nomReseau.erase(0, 20); // Enlève: Reseau Interurbain:

        int nbVilles;

        fichierEntree >> nbVilles;
        getline(fichierEntree, buff); //villes

        unReseau.resize(nbVilles);

        getline(fichierEntree, buff); //Liste des villes

        size_t i = 0;

        getline(fichierEntree, buff); //Premiere ville

        while(buff != "Liste des trajets:")
        {
            unReseau.nommer(i, buff);
            getline(fichierEntree, buff);
            i++;
        }

        while(!fichierEntree.eof())
        {
            getline(fichierEntree, buff);
            std::string source = buff;
            getline(fichierEntree, buff);
            std::string destination = buff;

            getline(fichierEntree, buff);
            std::istringstream iss(buff);

            float duree;
            iss >> duree;

            float cout;
            iss >> cout;

            unReseau.ajouterArc(unReseau.getNumeroSommet(source), unReseau.getNumeroSommet(destination), duree, cout);
        }
    }

    /**
    * \fn Chemin rechercheCheminDijkstra(const std::string& origine, const std::string& destination, bool dureeCout) const
    * \brief Fonction servant à trouver les trajets les plus courts ou les moins coûteux en partant d'un sommet de départ vers une destination en utilisant l'agorithme de Dijkstra
    * \param[in] origine le sommet de départ du trajet
    * \param[in] destination le sommet de destination du trajet
    * \param[in] dureeCout booléen déterminant si la pondération à choisir doit être la durée ou le coût du trajet
    * \exception logic_error si origine et/ou destination absent du réseau
    * \return Un struct Chemin contenant un vecteur de strings contenant le nom des villes du chemin le plus court, un float représentant la
     * durée totale du trajet, un float représentant le coût total du trajet et un booléen true si le chemin est possible, false sinon
    */
    Chemin ReseauInterurbain::rechercheCheminDijkstra(const std::string& origine, const std::string& destination, bool dureeCout) const
    {
        if(!unReseau.sommetExiste(unReseau.getNumeroSommet(origine))) throw std::logic_error("ReseauInterurbain::rechercheCheminDijkstra : le sommet origine est invalide");
        if(!unReseau.sommetExiste(unReseau.getNumeroSommet(destination))) throw std::logic_error("ReseauInterurbain::rechercheCheminDijkstra : le sommet destination est invalide");

        Chemin cheminTrouve;
        //Initialisation de la struct cheminTrouve
        cheminTrouve.coutTotal = 0;
        cheminTrouve.dureeTotale = 0;
        cheminTrouve.listeVilles.push_back(origine);
        cheminTrouve.reussi = false;

        //Initialisation des vecteurs servant à retenir les informations
        std::vector<size_t> nonResolus;
        for (size_t i = 0; i < unReseau.taille(); ++i) {
            nonResolus.push_back(i);
        }

        std::vector<std::string> listeVilles(unReseau.taille(), "");
        std::vector<float> ponderations(unReseau.taille(), std::numeric_limits<float>::infinity());
        ponderations.at(unReseau.getNumeroSommet(origine)) = 0;

        while (!nonResolus.empty()) {

            auto courant = localiserSommetMinimal(nonResolus, ponderations);

            nonResolus.erase(std::find(nonResolus.begin(), nonResolus.end(),courant));
            for (auto voisin: unReseau.listerSommetsAdjacents(courant)) {
                relaxer(voisin, courant, ponderations, listeVilles, dureeCout);
            }
        }

        //Mise à jour de la liste des villes de la struct cheminTrouve
        cheminTrouve.listeVilles = construireChemin(origine, destination, listeVilles, cheminTrouve);

        //Mise à jour des floats dureeTotale et coutTotal
        if (cheminTrouve.reussi)
        {
            for (int i = 0; i < cheminTrouve.listeVilles.size() - 1; ++i) {
                if (dureeCout)
                {
                    cheminTrouve.dureeTotale += unReseau.getPonderationsArc(unReseau.getNumeroSommet(cheminTrouve.listeVilles[i]), unReseau.getNumeroSommet(cheminTrouve.listeVilles[i +1])).duree;
                }
                else{
                    cheminTrouve.coutTotal += unReseau.getPonderationsArc(unReseau.getNumeroSommet(cheminTrouve.listeVilles[i]), unReseau.getNumeroSommet(cheminTrouve.listeVilles[i +1])).cout;
                }
            }
        }

        return cheminTrouve;
    }


    /**
   * \fn std::vector<std::vector<std::string> > algorithmeKosaraju()
   * \brief Fonction servant à trouver et énumérer les composantes fortement connexes d'un graphe
   * \return Un vecteur de vecteur de string représentant les composantes fortement connexes et leurs villes respectives
   */

    std::vector<std::vector<std::string> > ReseauInterurbain::algorithmeKosaraju()
    {
        std::vector<std::vector<std::string> > composantes;
        std::stack<std::string> pile = exploreRecursifGrapheDFS(unReseau.grapheInverse()) ;

        InfoDFS data(unReseau) ;
        while (!pile.empty()) {
            std::string depart = pile.top() ;
            pile.pop() ;

            if (!data.visites.at(unReseau.getNumeroSommet(depart))) {
                auxExploreRecursifDFS(data, unReseau.getNumeroSommet(depart)) ;
                composantes.push_back(transfererPileVersVecteur(data.abandonnes)) ;
            }
        }

        return composantes ;
    }

    //À compléter au besoin par d'autres méthodes

    /**
    * \fn  size_t localiserSommetMinimal(std::vector<size_t>& nonResolus, std::vector<float>& ponderations) const
    * \brief Fonction auxiliaire utilisée dans Dijkstra.  Elle balaye les noeuds non-résolus à la recherche de celui qui a une
    * distance minimale.  Ce noeud est retiré des non-résolus: il sera le prochain noeud résolu.
    * \param[in] nonResolus le vecteur de sommets non résolus
    * \param[in] ponderations le vecteur contenant les pondérations
    * \return Le numéro du sommet non résolu ayant la plus petite pondération
    */
    size_t ReseauInterurbain::localiserSommetMinimal(std::vector<size_t>& nonResolus, std::vector<float>& ponderations) const {
        auto temp = std::numeric_limits<float>::infinity();
        size_t indexMin = *nonResolus.begin();

        for (auto cle: nonResolus)
            if (ponderations.at(cle) < temp)
            {
                indexMin = cle ;
                temp = ponderations.at(cle);
            }

        return indexMin ;
    }

    /**
    * \fn  void relaxer(size_t voisin, size_t courant, std::vector<float>& ponderations, std::vector<std::string>& listeVilles, bool dureeCout) const
    * \brief Fonction auxiliaire utilisée dans Dijkstra.  Elle relaxe le noeud voisin à partir du noeud courant
    * \param[in] voisin le sommet voisin à relaxer
    * \param[in] courant le sommet courant
    * \param[in] ponderations le vecteur contenant les pondérations
    * \param[in] listeVilles le vecteur contenant les villes précédant le voisin
    * \param[in] dureeCout le booléen décidant si la pondération à utiliser est la distance ou le coût
    */
    void ReseauInterurbain::relaxer(size_t voisin, size_t courant, std::vector<float>& ponderations, std::vector<std::string>& listeVilles, bool dureeCout) const{
        float temp;

        if(dureeCout)
        {
            temp = ponderations.at(courant) + unReseau.getPonderationsArc(courant, voisin).duree ;
        }
        else
        {
            temp = ponderations.at(courant) + unReseau.getPonderationsArc(courant, voisin).cout ;
        }

        if (temp < ponderations.at(voisin)) {
            ponderations.at(voisin) = temp ;
            listeVilles.at(voisin) = unReseau.getNomSommet(courant);
        }
    }

    /**
    * \fn  std::stack<std::string> exploreRecursifGrapheDFS(const Graphe &graphe)
    * \brief Effectue une visite en profondeur d'un objet graphe
    * \param[in] graphe Le graphe à visiter
    * \return Une pile contenant les noeuds dans l'ordre où ils ont été abandonnés.  Donc le dernier noeud abandonné sera le
    * premier à sortir de la pile.
    */
    std::stack<std::string> ReseauInterurbain::exploreRecursifGrapheDFS(const Graphe &graphe) {
        InfoDFS donneesDfs(graphe) ;

        for (size_t depart = 0; depart < graphe.taille(); ++depart)
            auxExploreRecursifDFS(donneesDfs, depart) ;

        return donneesDfs.abandonnes ;
    }

    /**
    * \fn  void auxExploreRecursifDFS(InfoDFS& donneesDFS, size_t depart)
    * \brief Explore un graphe en profondeur à partir d'une sommet donné.
    * \param[in] donneesDFS struct InfoDFS contenant le graphe à explorer, une pile qui recevra les noeuds abandonnées et un vecteur
    * de bool indiquant quels noeuds ont été visités.  Voir Graphe_algorithme.h pour une description complète.
    * \param[in] depart Le numéro du sommet de départ à explorer.
    */
    void ReseauInterurbain::auxExploreRecursifDFS(InfoDFS& donneesDFS, size_t depart) {
        if (donneesDFS.visites.at(depart)) return ;

        donneesDFS.visites.at(depart) = true ;
        for (const auto& voisin: donneesDFS.graphe.listerSommetsAdjacents(depart))
            if (!donneesDFS.visites.at(voisin)) auxExploreRecursifDFS(donneesDFS, voisin) ;

        donneesDFS.abandonnes.push(unReseau.getNomSommet(depart)) ;

    }

    /**
    * \fn  void auxExploreRecursifDFS(InfoDFS& donneesDFS, size_t depart)
    * \brief Transfère le contenu d'une pile dans un vecteur, en vidant la pile.
    * \param[in] pile La pile à transférer
    * \return std::vector<std::string> Le vecteur contenant tous les éléments de la pile.
    */
    std::vector<std::string> ReseauInterurbain::transfererPileVersVecteur(std::stack<std::string> &pile) {
        std::vector<std::string> vecteurRetour ;

        while (!pile.empty()) {
            vecteurRetour.push_back(pile.top()) ;
            pile.pop() ;
        }

        return vecteurRetour ;
    }

    /**
     * \fn td::vector<std::string> construireChemin(const std::string& source, const std::string& destination, std::vector<std::string> predecesseurs, Chemin& cheminTrouve) const
     * \brief Construit le chemin entre deux villes à partir d'un vecteur de std::string contenant les prédécesseurs des sommets.
     * \param[in] source le sommet de départ du trajet. Sert à déterminer si le chemin est possible ou non
     * \param[in] destination le sommet de destination
     * \param[in] predecesseurs le vecteur de std::string contenant les prédécesseurs
     * \param[in] cheminTrouve la struct Chemin afin de mettre à jour son booléen réussi si le chemin a été trouvé
     * \return std::vector<std::string> le vecteur contenant les villes du trajet
     */
    std::vector<std::string> ReseauInterurbain::construireChemin(const std::string& source, const std::string& destination, std::vector<std::string> predecesseurs, Chemin& cheminTrouve) const {

        std::vector<std::string> trajet;

        auto courant = unReseau.getNumeroSommet(destination);
        while(predecesseurs[courant] != "")
        {
            trajet.push_back(unReseau.getNomSommet(courant));
            courant = unReseau.getNumeroSommet(predecesseurs[courant]);
            if (unReseau.getNomSommet(courant) == source)
            {
                cheminTrouve.reussi = true;
                trajet.push_back(source);
            }
        }

        std::reverse(trajet.begin(), trajet.end());
        return trajet;
    }

}//Fin du namespace
