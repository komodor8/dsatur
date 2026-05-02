#ifndef GLOUTON_HPP
#define GLOUTON_HPP

#include <vector>

// On annonce nos deux fonctions gloutonnes
std::vector<int> gloutonOrdreNaturel(const std::vector<std::vector<int>> &adj);
std::vector<int> gloutonDegreDecroissant(const std::vector<std::vector<int>> &adj);

#endif