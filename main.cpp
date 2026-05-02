#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>

#include "dsatur.hpp"
#include "glouton.hpp"

using namespace std;

// --- FONCTIONS DE LECTURE ---
vector<vector<int>> lireGrapheSNAP(string nomFichier)
{
    ifstream fichier(nomFichier);
    vector<vector<int>> adj;
    if (!fichier.is_open())
        return adj;
    string ligne;
    int max_sommet = -1;
    vector<pair<int, int>> aretes;
    while (getline(fichier, ligne))
    {
        if (ligne.empty() || ligne[0] == '#' || ligne[0] == '%' || ligne[0] == 'c')
            continue;
        stringstream ss(ligne);
        int u, v;
        if (ss >> u >> v)
        {
            aretes.push_back({u, v});
            max_sommet = max({max_sommet, u, v});
        }
    }
    if (max_sommet >= 0)
    {
        adj.resize(max_sommet + 1);
        for (auto arete : aretes)
        {
            adj[arete.first].push_back(arete.second);
            adj[arete.second].push_back(arete.first);
        }
    }
    return adj;
}

vector<vector<int>> lireGrapheDIMACS(string nomFichier)
{
    ifstream fichier(nomFichier);
    vector<vector<int>> adj;

    if (!fichier.is_open())
    {
        cerr << "Erreur : Impossible d'ouvrir le fichier " << nomFichier << " !" << endl;
        return adj;
    }

    string type_ligne;
    int n = 0; // Nombre de sommets

    // On lit le premier mot de chaque ligne
    while (fichier >> type_ligne)
    {
        if (type_ligne == "c")
        {
            // C'est un commentaire : on lit tout le reste de la ligne et on l'ignore
            string commentaire;
            getline(fichier, commentaire);
        }
        else if (type_ligne == "p")
        {
            // C'est la définition du graphe : p edge n m
            string mot_edge;
            int m; // nombre d'arêtes (on ne s'en sert pas vraiment ici, mais il faut le lire)
            fichier >> mot_edge >> n >> m;

            // On connaît maintenant la taille du graphe, on redimensionne la liste
            adj.resize(n);
        }
        else if (type_ligne == "e")
        {
            // C'est une arête : e u v
            int u, v;
            fichier >> u >> v;

            // Attention : DIMACS commence à 1, nos tableaux commencent à 0 !
            // On soustrait donc 1 à chaque sommet.
            adj[u - 1].push_back(v - 1);
            adj[v - 1].push_back(u - 1);
        }
    }

    fichier.close();
    return adj;
}

double calculerRatioRho(const vector<int> &valeurs_ni, int n)
{
    if (valeurs_ni.empty() || n == 0)
        return 0.0;
    int n_k = valeurs_ni.back();
    return (double)n_k / n;
}

// --- PROGRAMME PRINCIPAL ---
int main()
{
    string fichier_test = "./data/graph_test_example_l.txt";
    vector<vector<int>> graphe = lireGrapheSNAP(fichier_test);

    if (graphe.empty())
    {
        cerr << "Impossible de charger le graphe." << endl;
        return 1;
    }

    int n = graphe.size();
    cout << "--- ANALYSE DU GRAPHE (" << n << " sommets) ---" << endl;

    // Appel à glouton.cpp
    vector<int> ni_nat = gloutonOrdreNaturel(graphe);
    cout << "\n[GLOUTON - Ordre Naturel]" << endl;
    cout << "Couleurs utilisees (k) : " << ni_nat.size() << endl;
    cout << "Ratio rho : " << calculerRatioRho(ni_nat, n) << endl;

    // Appel à glouton.cpp
    vector<int> ni_deg = gloutonDegreDecroissant(graphe);
    cout << "\n[GLOUTON - Degre Decroissant]" << endl;
    cout << "Couleurs utilisees (k) : " << ni_deg.size() << endl;
    cout << "Ratio rho : " << calculerRatioRho(ni_deg, n) << endl;

    // Appel à dsatur.cpp
    cout << "\n[DSATUR]" << endl;
    vector<int> ni_dsatur = calculerNi(graphe);
    cout << "Couleurs utilisees (k) : " << ni_dsatur.size() << endl;
    cout << "Ratio rho : " << calculerRatioRho(ni_dsatur, n) << endl;

    return 0;
}