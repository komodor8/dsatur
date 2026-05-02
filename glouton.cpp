#include "glouton.hpp"
#include <vector>
#include <set>
#include <algorithm>

using namespace std;

vector<int> gloutonOrdreNaturel(const vector<vector<int>> &adj)
{
    int n = adj.size();
    vector<int> couleur(n, 0);
    vector<int> valeurs_ni;
    int max_couleur_utilisee = 0;
    int sommets_colories = 0;

    for (int u = 0; u < n; ++u)
    {
        set<int> couleurs_voisines;
        for (int voisin : adj[u])
        {
            if (couleur[voisin] != 0)
                couleurs_voisines.insert(couleur[voisin]);
        }

        int c_choisie = 1;
        while (couleurs_voisines.count(c_choisie))
            c_choisie++;

        couleur[u] = c_choisie;
        sommets_colories++;
        if (c_choisie > max_couleur_utilisee)
        {
            max_couleur_utilisee = c_choisie;
            valeurs_ni.push_back(sommets_colories);
        }
    }
    return valeurs_ni;
}

vector<int> gloutonDegreDecroissant(const vector<vector<int>> &adj)
{
    int n = adj.size();
    vector<int> couleur(n, 0);
    vector<int> valeurs_ni;
    int max_couleur_utilisee = 0;

    vector<pair<int, int>> degres_sommets(n);
    for (int i = 0; i < n; ++i)
        degres_sommets[i] = {adj[i].size(), i};

    sort(degres_sommets.begin(), degres_sommets.end(), [](const pair<int, int> &a, const pair<int, int> &b)
         {
        if (a.first != b.first) return a.first > b.first;
        return a.second < b.second; });

    int sommets_colories = 0;

    for (auto p : degres_sommets)
    {
        int u = p.second;
        set<int> couleurs_voisines;
        for (int voisin : adj[u])
        {
            if (couleur[voisin] != 0)
                couleurs_voisines.insert(couleur[voisin]);
        }

        int c_choisie = 1;
        while (couleurs_voisines.count(c_choisie))
            c_choisie++;

        couleur[u] = c_choisie;
        sommets_colories++;
        if (c_choisie > max_couleur_utilisee)
        {
            max_couleur_utilisee = c_choisie;
            valeurs_ni.push_back(sommets_colories);
        }
    }
    return valeurs_ni;
}