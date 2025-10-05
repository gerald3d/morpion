#ifndef __LOGS_H__
#define __LOGS_H__

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

typedef enum {LOG_STANDARD, LOG_ERROR} TYPE_OF_LOGS;

typedef struct {
  char *fichier_standard,  *fichier_erreur;
  FILE *descripteur_de_fichier_standard;
  FILE *descripteur_de_fichier_erreur;
  TYPE_OF_LOGS log_type;
} t_logs;

/* Initialise une donnée dûment remplie.
 * si un des deux ou les deux paramètres sont à NULL ils prennent les valeurs
 * suivantes par défaut : standard_logs.txt et error_logs.txt.
 * Si les deux noms sont identiques l'opération échoue et renvoie NULL.
 * Si une erreur survient la fonction renvoie NULL.
 * Pour toute erreur un message en console est affiché. */
t_logs *logs_init (const char *fichier_standard, const char *fichier_erreur);

/* Libération mémoire.
 * Si logs vaut NULL la fonction est sans effet */
void logs_liberation (t_logs *logs);

/* Écriture dans le fichier log désigné par log_type.
 * Si la sauvegarde échoue un message est affiché en console et
 * la fonction renvoie false.
 *
 * Remarque : en cas d'échec le pointeur logs n'est pas libéré. */
/* bool logs_write (t_logs *logs, const char *message, TYPE_OF_LOGS log_type); */

/* Récupération du descripteur de fichier.
 * Si une erreur survient un message d'erreur est affiché en console et
 * la fonction renvoie NULL */
FILE *logs_descripteur_fichier (t_logs *logs, TYPE_OF_LOGS log_type);

/* Ajoute un texte dans le descripteur choisi.*/
void logs_save (t_logs *logs, const char *text, TYPE_OF_LOGS log_type);
#endif
