#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>

#include "dsatur.hpp"
#include "glouton.hpp"

using namespace std;

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
    int n = 0;

    while (fichier >> type_ligne)
    {
        if (type_ligne == "c")
        {
            string commentaire;
            getline(fichier, commentaire);
        }
        else if (type_ligne == "p")
        {
            string mot_edge;
            int m;
            fichier >> mot_edge >> n >> m;

            adj.resize(n);
        }
        else if (type_ligne == "e")
        {
            int u, v;
            fichier >> u >> v;

            adj[u - 1].push_back(v - 1);
            adj[v - 1].push_back(u - 1);
        }
    }

    fichier.close();
    return adj;
}

double calculerRatio(const vector<int> &valeurs_ni, int n)
{
    if (valeurs_ni.empty() || n == 0)
        return 0.0;
    int n_k = valeurs_ni.back();
    return (double)n_k / n;
}

void afficherResultats(string nomAlgo, const vector<int> &valeurs_ni, int n)
{
    cout << "\n[" << nomAlgo << "]" << endl;

    if (valeurs_ni.empty())
    {
        cout << "Erreur : Aucun sommet colorie." << endl;
        return;
    }

    int k = valeurs_ni.size();
    int n_k = valeurs_ni.back();
    double rho = calculerRatio(valeurs_ni, n);

    cout << "  - Nombre total de sommets (n)           : " << n << endl;
    cout << "  - Couleurs utilisees (k)                : " << k << endl;
    cout << "  - Sommet de la derniere couleur (n_k)   : " << n_k << endl;
    cout << "  - Ratio d'avancement (rho)              : " << rho << " (soit " << rho * 100 << " %)" << endl;

    cout << "  - Detail de l'ouverture des couleurs (n_i) :" << endl;
    for (int i = 0; i < k; ++i)
    {
        cout << "      n_" << (i + 1) << " = " << valeurs_ni[i] << endl;
    }
    cout << "------------------------------------------------" << endl;
}

int main()
{
    string fichier_test = "./data/le450_15b.col";
    vector<vector<int>> graphe = lireGrapheDIMACS(fichier_test);

    if (graphe.empty())
    {
        cerr << "Impossible de charger le graphe." << endl;
        return 1;
    }

    int n = graphe.size();
    cout << "================================================" << endl;
    cout << "          ANALYSE DU GRAPHE (" << n << " sommets)    " << endl;
    cout << "================================================" << endl;

    vector<int> ni_nat = calculerNiGloutonOrdreNaturel(graphe);
    afficherResultats("GLOUTON - Ordre Naturel", ni_nat, n);

    vector<int> ni_deg = calculerNiGloutonDegreDecroissant(graphe);
    afficherResultats("GLOUTON - Degre Decroissant", ni_deg, n);

    vector<int> ni_dsatur = calculerNiDsatur(graphe);
    afficherResultats("DSATUR", ni_dsatur, n);

    return 0;
}