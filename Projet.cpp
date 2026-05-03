#include <iostream>
#include <vector>
#include <set>
#include <fstream> 

using namespace std;




//=================QUESTION 1=================
void afficherGraphe(vector<vector<int>>& voisins) {
    cout << "Voici le graphe :" << endl;
    cout << "---" << endl;
    
    for (int i = 0; i < voisins.size(); i++) {
        cout << "Le sommet " << i << " est relie a : ";
        
        for (int j = 0; j < voisins[i].size(); j++) {
            cout << voisins[i][j] << " ";
        }
        cout << endl;
    }
}









//=================QUESTION 2=================

//Fonction de cacul DSAT :

//Utilisation PseudoCode du cours
int calculerDSAT(int sommet, const vector<vector<int>>& voisins, const vector<int>& couleur) { 
    bool aucun_voisin_colorie = true;
    set<int> couleurs_differentes;
    // On parcourt tout le voisinage du sommet
    for (int i = 0; i < voisins[sommet].size(); i++) {
        int le_voisin = voisins[sommet][i];
        // On vérifie si ce voisin a une couleur (différente de -1)
        if (couleur[le_voisin] != -1) {
            aucun_voisin_colorie = false; //On a trouvé au moins un voisin colorié
            couleurs_differentes.insert(couleur[le_voisin]); // On note sa couleur
        }
    }

    //Si aucun voisin de v n’est colorié alors DSAT(v) = 0
    if (aucun_voisin_colorie == true) {
        return 0;
    }
    //Sinon DSAT(v) = le nombre de couleurs différentes utilisées par le voisinage de v
    else {
        return couleurs_differentes.size();
    }
}

int choisirProchainSommet(int n, const vector<int>& couleur, const vector<int>& degre, const vector<vector<int>>& voisins) {
    int sommet_choisi = -1;
    int max_dsat = -1;
    int max_degre = -1;

    for (int S = 0; S < n; S++) {

        // On ne regarde QUE les sommets non coloriés
        if (couleur[S] == -1) {
            int dsat_S = calculerDSAT(S, voisins, couleur);
            
            // --- REGLE 1 : Le plus grand degré de saturation ---
            if (dsat_S > max_dsat) {
                sommet_choisi = S;
                max_dsat = dsat_S;
                max_degre = degre[S];
            }

            // --- REGLE 2 : En cas d'égalité sur la Règle 1 ---
            else if (dsat_S == max_dsat) {
                
                // On regarde le plus grand degré
                if (degre[S] > max_degre) {
                    sommet_choisi = S;
                    max_degre = degre[S]; 
                }
                // --- REGLE 3 : En cas d'égalité sur la Règle 1 ET la Règle 2 ---
                else if (degre[S] == max_degre) {
                    
                    // On regarde le plus petit numéro. 
                    // (Le "sommet_choisi == -1" est là juste pour le tout premier passage)
                    if (S < sommet_choisi || sommet_choisi == -1) {
                        sommet_choisi = S;
                    }
                }
            }
        }
    }
    
    // À la fin de la boucle, on renvoie le grand gagnant !
    return sommet_choisi;
}



// Fonction complète DSATUR + calcul des n_i
vector<int> calculerNiDsatur(const vector<vector<int>>& voisins) {

    int n = voisins.size(); // Nombre total de sommets du graphe

    vector<int> couleur(n, -1);   // Tableau des couleurs (-1 = sommet non colorié)
    vector<int> degre(n);         // Tableau Degré de chaque sommet
    vector<int> ni;               // Les valeurs n_i demandées

    //  Calcul des degrés 
    for (int i = 0; i < n; i++) {
        degre[i] = voisins[i].size(); // Le degré d’un sommet = nombre de voisins
    }

    int sommets_colories = 0;
    int max_couleur_utilisee = 0;     // Plus grande couleur utilisée jusqu’à présent


    //  Boucle principale DSATUR
    while (sommets_colories < n) {

        // Choisir le prochain sommet à colorier selon DSATUR
        int u = choisirProchainSommet(n, couleur, degre, voisins);

        //  Recherche de la plus petite couleur valide 
        int c = 1;             // Val de la couleur      
        bool couleur_trouvee = false; 


        //-----------RECHERCHE DE LA PLUS PETITE COULEUR--------------
        while (!couleur_trouvee) {
            couleur_trouvee = true;  // On suppose que la couleur est valide

            // Vérifier tous les voisins du sommet u
            for (int i = 0; i < voisins[u].size(); i++) {
                int v = voisins[u][i]; // v est un voisin de u

                // Si un voisin a déjà la couleur c -> conflit
                if (couleur[v] == c) {
                    couleur_trouvee = false; // couleur interdite
                    break;                   // inutile de continuer à vérifier
                }
            }

            // Si la couleur n’est pas valide, on teste la suivante
            if (!couleur_trouvee) c++;
        }
        // -------------------------

        //  Gestion des n_i 
        // Si on utilise une nouvelle couleur jamais utilisée auparavant
        if (c > max_couleur_utilisee) {
            max_couleur_utilisee = c;         // Mise à jour de la couleur max
            ni.push_back(sommets_colories + 1); // On stocke le nombre de sommets déjà coloriés
        }

        //  Coloration du sommet 
        couleur[u] = c;       // On affecte la couleur c au sommet u
        sommets_colories++;   // On incrémente le nombre de sommets coloriés
    }

    //  Résultat 
    return ni; // On retourne le tableau contenant tous les n_i
}














//=================QUESTION 3=================

// Calcule le ratio ρ = n_k / n
double calculerRatio(const vector<int> &valeurs_ni, int n)
{
    if (valeurs_ni.empty() || n == 0)
        return 0.0;
    int n_k = valeurs_ni.back();
    return (double)n_k / n;
}

















//=================QUESTION 4=================

vector<int> calculerNiGloutonOrdreNaturel(const vector<vector<int>> &adj)
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

vector<int> calculerNiGloutonDegreDecroissant(const vector<vector<int>> &adj)
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


















//================= MAIN =================
int main() {
    int sommet1, sommet2;
    vector<vector<int>> voisins; 
    
    ifstream monFichier("graphe.txt");

    if (!monFichier.is_open()) {
        cout << "Erreur : Impossible d'ouvrir le fichier graphe.txt" << endl;
        return 1;
    }

    while (monFichier >> sommet1 >> sommet2) {
        //Taille dynamique du vecteur
        int max_sommet=max(sommet1,sommet2);

        if (max_sommet >=voisins.size()){
            voisins.resize(max_sommet +1);
        }

        voisins[sommet1].push_back(sommet2);
        voisins[sommet2].push_back(sommet1);
    }

    monFichier.close();
    int n = voisins.size();

    
    // Question 1:
    cout << "\n--- Question 1 ---" << endl;
    afficherGraphe(voisins);



    // Question 4:
cout << "\n--- Question 4 : Comparaison des ratios (plusieurs graphes) ---" << endl;

vector<string> fichiers = {
    "data/graph_test_example_l.txt",
    "data/graph_test_example_r.txt",
    "data/graph_test_snap.txt"
};

for (string nom : fichiers)
{
    cout << "\n=== Graphe : " << nom << " ===" << endl;

    vector<vector<int>> voisins;
    ifstream fichier(nom);

    int u, v;
    while (fichier >> u >> v)
    {
        int max_sommet = max(u, v);
        if (max_sommet >= voisins.size())
            voisins.resize(max_sommet + 1);

        voisins[u].push_back(v);
        voisins[v].push_back(u);
    }

    fichier.close();

    int n = voisins.size();

    vector<int> ni_nat = calculerNiGloutonOrdreNaturel(voisins);
    cout << "[GLOUTON - Ordre Naturel] ";
    cout << "k=" << ni_nat.size() << " ";
    cout << "ratio=" << calculerRatio(ni_nat, n) << endl;

    vector<int> ni_deg = calculerNiGloutonDegreDecroissant(voisins);
    cout << "[GLOUTON - Degre Decroissant] ";
    cout << "k=" << ni_deg.size() << " ";
    cout << "ratio=" << calculerRatio(ni_deg, n) << endl;

    vector<int> ni_dsatur = calculerNiDsatur(voisins);
    cout << "[DSATUR] ";
    cout << "k=" << ni_dsatur.size() << " ";
    cout << "ratio=" << calculerRatio(ni_dsatur, n) << endl;
}




    return 0;
}