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
    string fichier_test = "./data/facebook_combined.txt";
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