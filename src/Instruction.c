#include "Main.h"

/* extern FILE* fichierErreur;//extern car la variable globale est deja initialisee dans main.c */

/*********************************************/
void Instruction(t_morp_sdl *morp_sdl, Joueur Joueur, CaracteristiqueJeu Jeu, DimPlateau DimJeu)
{
    int i;
    SDL_Event event;
    SDL_Rect clic= {0, 0, EcranLarg, EcranHaut};//position du clic (x, y) et dimension de l'ecran (w, h)
    int AideFinie=NON;
    SDL_Rect positionTexte= {30, 30, 0, 0};
    SDL_Rect positionFond= {0, 0, EcranLarg, EcranHaut};
    zone ZoneCliquee = MARGE;
    int PositionTexteY[11]= {30,60,90,140,170,220,250,290,320,350,380}; //pour déterminer l'ordonnée des lignes à écrire
//    SDL_SetRenderDrawColor(Renderer, couleurImageBlanc,255);//selectionne de la couleurImageBlanc pour le fond (renderer), AVEC Alpha=255
//    SDL_RenderClear(Renderer);//nettoie le fond

    AfficheRectPlein(morp_sdl, positionFond, couleurImageMarron);//recouvre la marge avec la couleur de fond
    AfficheBoutons(morp_sdl,Joueur, DimJeu, EcranInstruction);

    for (i=0; i<11; i++)
    {
        positionTexte.y = PositionTexteY[i];
        positionTexte.w = morp_sdl->jeton.DimensionTexteMessage[i].w;
        positionTexte.h = morp_sdl->jeton.DimensionTexteMessage[i].h;
        SDL_RenderCopy(morp_sdl->renderer, morp_sdl->jeton.TexteMessage[i],NULL,&positionTexte); // Affiche le Titre
    }

    SDL_RenderPresent(morp_sdl->renderer);//Actualisation

    while(AideFinie==NON)
    {
        while(SDL_PollEvent(&event)) //Traiter les évènements
        {
            switch(event.type)
            {
            case SDL_QUIT: //Si on a cliqué sur la croix
                AideFinie=OUI;
                break;
            case SDL_KEYUP: //Si touche du clavier enfoncée
                switch (event.key.keysym.sym) //selon la touche enfoncée
                {
                case SDLK_ESCAPE: // Appui sur la touche Echap, on va arrêter le jeu
                case SDLK_a: //appui sur la touche a pour quitter
                case SDLK_v: //appui sur la touche v pour valider et quitter
                    AideFinie=OUI;
                    break;
                default : //sinon rien pour eviter message d'alerte lors de la compilation
                    break;
                }
                break;
            case SDL_MOUSEBUTTONUP: //Si clic souris
                clic.x=event.button.x;
                clic.y=event.button.y;
                ZoneCliquee=QuelleZoneCliquee(DimJeu,clic, Joueur,Jeu);
                if((ZoneCliquee==QUITTER)||(ZoneCliquee==VALIDER))
                    AideFinie=OUI;
                break;
            default:  //Sinon on ne fait rien
                break;
            }
        }
    }
}

/*****************************/











