#include "dsatur.hpp" // On inclut le menu
#include <vector>
#include <set>

using namespace std;

vector<int> calculerNi(const vector<vector<int>> &adj)
{
    int n = adj.size();
    vector<int> couleur(n, 0);
    vector<int> degre(n);
    vector<int> dsat(n, 0);
    vector<set<int>> couleurs_voisines(n);
    vector<int> valeurs_ni;

    for (int i = 0; i < n; i++)
        degre[i] = adj[i].size();

    int sommets_colories = 0;
    int max_couleur_utilisee = 0;

    while (sommets_colories < n)
    {
        int u = -1, max_dsat = -1, max_deg = -1;

        for (int i = 0; i < n; i++)
        {
            if (couleur[i] == 0)
            {
                if (dsat[i] > max_dsat)
                {
                    max_dsat = dsat[i];
                    max_deg = degre[i];
                    u = i;
                }
                else if (dsat[i] == max_dsat)
                {
                    if (degre[i] > max_deg)
                    {
                        max_deg = degre[i];
                        u = i;
                    }
                }
            }
        }

        int c_choisie = 1;
        while (couleurs_voisines[u].count(c_choisie))
            c_choisie++;

        if (c_choisie > max_couleur_utilisee)
        {
            max_couleur_utilisee = c_choisie;
            valeurs_ni.push_back(sommets_colories + 1);
        }

        couleur[u] = c_choisie;
        sommets_colories++;

        for (int voisin : adj[u])
        {
            if (couleur[voisin] == 0)
            {
                couleurs_voisines[voisin].insert(c_choisie);
                dsat[voisin] = couleurs_voisines[voisin].size();
            }
        }
    }
    return valeurs_ni;
}