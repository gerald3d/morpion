#include <float.h>
#include <stdbool.h>

//double max_entre_a_et_b (double a, double b) //Recherche la valeur maximale d'un tableau
//{
//  if (a>b)
//    return a;
//
//  return b;
//}
//
//double min_entre_a_et_b (double a, double b) //Recherche la valeur maximale d'un tableau
//{
//  if (a<b)
//    return a;
//
//  return b;
//}
//
//bool verifier_vainqueur(Case *plateau, int joueur) {
//  // Vérifier les lignes, colonnes et diagonales
//  /* return ( */
//  /* 	  any(all(case == joueur pour case dans ligne) pour ligne dans plateau) or */
//  /* 	  any(all(ligne[i] == joueur pour ligne dans plateau) pour i dans range(3)) or */
//  /* 	  all(plateau[i][i] == joueur pour i dans range(3)) or */
//  /* 	  all(plateau[i][2-i] == joueur pour i dans range(3)) */
//  /* 	  ) */
//    }
//
//double minimax(Case *plateau, unsigned char profondeur, int taille_plateau, bool est_maximisation) {
//  if (verifier_vainqueur(plateau, 'O'))
//    return 1;
//  if (verifier_vainqueur(plateau, 'X'))
//    return -1;
//  if (not any(' ' dans ligne pour ligne dans plateau))
//    return 0;
//
//  if (est_maximisation) {
//    double meilleur_score = -DBL_MAX;
//    double score = 0;
//
//    for (int i=0; i <taille_plateau; i++){
//      for (int j=0; j<taille_plateau; j++) {
//	if (plateau[i][j] == ' ') {
//	  plateau[i][j] = 'O';
//	  score = minimax(plateau, profondeur + 1, false);
//	  plateau[i][j] = ' ';
//	  meilleur_score = max_entre_a_et_b (score, meilleur_score);
//	}
//      }
//    }
//    return meilleur_score;
//  }
//  else {
//    double meilleur_score = DBL_MAX;
//    double score = 0;
//
//    for (int i=0; i<taille_plateau; i++) {
//      for (int j=0; j<taille_plateau; j++) {
//	if (plateau[i][j] == ' ') {
//	  plateau[i][j] = 'X';
//	  score = minimax(plateau, profondeur + 1, true);
//	  plateau[i][j] = ' ';
//	  meilleur_score = min_entre_a_et_b (score, meilleur_score);
//	}
//      }
//    }
//    return meilleur_score;
//  }
//}
//
//double coup_ia (Case *plateau, int taille_plateau) {
//  double meilleur_score = -DBL_MAX;
//  double meilleur_coup = 0;
//
//  for (int i=0; i<taille_plateau; i++) {
//    for (int j=0; j<taille_plateau; j++) {
//      if (plateau[i][j] == ' ') {
//	plateau[i][j] = 'O';
//	score = minimax(plateau, 0, false);
//	plateau[i][j] = ' ';
//
//	if (score > meilleur_score) {
//	  meilleur_score = score;
//	  meilleur_coup = (i, j);
//	}
//      }
//    }
//  }
//
//  return meilleur_coup;
//}
