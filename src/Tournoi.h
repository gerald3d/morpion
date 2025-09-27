#ifndef DEF_Tournoi
#define DEF_Tournoi

#include "Define.h"
#include "morp_sdl.h"

void initTournoi(t_morp_sdl *morp_sdl, MancheTournoi *Tournoi, int MaxMancheTournoi,
		 SDL_Surface **texte,SDL_Rect *positionTitreColonne,
		 SDL_Rect *PositionZonesCliquables, SDL_Rect *PositionZonesTournoiAAfficher,
		 SDL_Rect *PositionZonesTournoiASelectionner);

void AfficheMancheTournoi(t_morp_sdl *morp_sdl, MancheTournoi *Tournoi,int MaxMancheTournoi,
			  Joueur Joueur, DimPlateau DimJeu, SDL_Surface **TitreColonne,
			  SDL_Rect *positionTitreColonne,SDL_Rect *PositionZonesTournoiAAfficher,
			  SDL_Rect *PositionZonesTournoiASelectionner); //a supprimer

void ModifMancheTournoi(t_morp_sdl *morp_sdl, MancheTournoi *Tournoi, int *MaxMancheTournoi,
			Joueur Joueur, DimPlateau DimJeu, SDL_Surface **TitreColonne,
			SDL_Rect *positionTitreColonne, SDL_Rect *PositionZonesCliquables,
			SDL_Rect *PositionZonesTournoiAAfficher,SDL_Rect *PositionZonesTournoiASelectionner); //a supprimer

void ModifPositionZonesTournoiASelectionner(int AncienMaxMancheTournoi, int NouveauMaxMancheTournoi,
					    SDL_Rect *PositionZonesTournoiASelectionner);

void AfficheVainqueur(t_morp_sdl *morp_sdl, IdentiteJoueur *joueurGagnant); //Affiche la Coupegrande avec O ou X sur le socle selon le joueur gagnant
//void CopieStruct(MancheTournoi *AncienTournoi,MancheTournoi *NouveauTournoi); //copie AncienTournoi dans NouveauTournoi



#endif
