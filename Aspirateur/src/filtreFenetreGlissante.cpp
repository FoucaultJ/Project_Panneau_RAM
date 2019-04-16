#include "filtreFenetreGlissante.hpp"

TFiltreFenetreGlissante::TFiltreFenetreGlissante(int fenetre)
    {//tableau pour contenir les valeurs (dimention = fenetre
	vTab.resize(fenetre);
    }

TFiltreFenetreGlissante::~TFiltreFenetreGlissante()
    {//delete si dynamiquement

    }

double TFiltreFenetreGlissante::filtre(double val)
    {//on filtre on entre val et on sort la plus ancienne.
/*	pTab[0]=pTab[1];
	pTab[1]=pTab[2];
	pTab[2]=val;



	for(int i=0; i<3;i++){
		val+=pTab[i];
	}
	val= val/3;*/
	vTab.erase(vTab.begin());
	vTab.push_back(val);
	val= accumulate(vTab.begin(), vTab.end(), 0.0)/vTab.size();

    return val;//moyenne
    }
