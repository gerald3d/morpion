#ifndef DEF_ia
#define DEF_ia

#include "Define.h"
#include "ihm/src/logs.h"

void ia(Case *plateau, Joueur Joueur, CaracteristiqueJeu *Jeu, DimPlateau DimJeu, t_logs *logs);
void SimuleCoup(int niveau, CaracteristiqueJeu Jeu, Case *plateau,Joueur Joueur, Couronne InfoZoneDeRecherche[], DimPlateau DimJeu,int *TabDeCoef,int *AlignementRealise, t_logs *logs);

#endif
