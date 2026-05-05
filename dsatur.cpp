#include "dsatur.hpp"
#include <vector>
#include <set>

using namespace std;

vector<int> calculerNiDsatur(const vector<vector<int>> &adj)
{
    int n = adj.size();
    vector<int> couleur(n, 0);
    vector<int> degre(n);
    vector<int> dsat(n, 0);
    vector<set<int>> couleurs_voisines(n);
    vector<int> valeurs_ni;

    for (int i = 0; i < n; i++)   // on parcourt tous les sommets
        degre[i] = adj[i].size(); // on assigne le nombre de degré du sommet

    int sommets_colories = 0;
    int max_couleur_utilisee = 0;

    while (sommets_colories < n)
    {
        int u = -1, max_dsat = -1, max_deg = -1;

        for (int i = 0; i < n; i++) // choix du sommet en parcourant tous les sommets
        {
            if (couleur[i] == 0) // non colorié
            {
                if (dsat[i] > max_dsat) // si dsat est le plus élevé on prend le sommet
                {
                    max_dsat = dsat[i];
                    max_deg = degre[i];
                    u = i;
                }
                else if (dsat[i] == max_dsat)
                {
                    if (degre[i] > max_deg) // Sinon on prend le sommet avec le plus de degré
                    {
                        max_deg = degre[i];
                        u = i;
                    }
                }
            }
        }

        int c_choisie = 1;                            // couleur la plus basse
        while (couleurs_voisines[u].count(c_choisie)) // on verifie si la couleur est choisie
            c_choisie++;                              // on cherche la plus petite couleur non utilisée par ses voisins

        if (c_choisie > max_couleur_utilisee) // si nouvelle couleur on ajoute le nombre de sommets coloriés à n_i
        {
            max_couleur_utilisee = c_choisie;
            valeurs_ni.push_back(sommets_colories + 1);
        }

        couleur[u] = c_choisie; // on assigne la couleur au sommet
        sommets_colories++;

        for (int voisin : adj[u]) // On parcourt tous les voisins du sommet choisi
        {
            if (couleur[voisin] == 0) // si voisin du sommet u non colorié
            {
                couleurs_voisines[voisin].insert(c_choisie);     // on ajoute la couleur à la ligne du voisin
                dsat[voisin] = couleurs_voisines[voisin].size(); // on update le dsat du sommet
            }
        }
    }
    return valeurs_ni;
}