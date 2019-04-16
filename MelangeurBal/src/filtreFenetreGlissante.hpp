#ifndef FILTRE_FENETRE_GLISSANTE_HPP
#define FILTRE_FENETRE_GLISSANTE_HPP
#include <iostream>
#include <vector>
#include <numeric>
using namespace std;

class TFiltreFenetreGlissante
    {
    private:
	//double *pTab;
	vector<double> vTab;
    public:
	TFiltreFenetreGlissante(int fenetre = 3);
	~TFiltreFenetreGlissante();

	double filtre(double val);
    };

#endif // FILTRE_FENETRE_GLISSANTE_HPP
