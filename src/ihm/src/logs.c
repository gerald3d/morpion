#include "logs.h"

t_logs*
logs_init (const char *fichier_standard, const char *fichier_erreur) {
  t_logs *logs = malloc (sizeof(t_logs));

  if (logs == NULL) {
    fprintf (stderr, "Erreur d'allocation mémoire dans la fonction %s\n", __func__);
    return NULL;
  }

  logs->fichier_standard = NULL;
  logs->fichier_erreur = NULL;
  logs->descripteur_de_fichier_standard = NULL;
  logs->descripteur_de_fichier_erreur = NULL;

  /* Initialisation des noms des fichiers de logs */
  if (fichier_standard == NULL)
    logs->fichier_standard = strdup ("standard_logs.txt");
  else
    logs->fichier_standard = strdup (fichier_standard);
  if (fichier_erreur == NULL)
    logs->fichier_erreur = strdup ("error_logs.txt");
  else
    logs->fichier_erreur = strdup (fichier_erreur);

  if (strcmp(logs->fichier_standard, logs->fichier_erreur)==0) {
    fprintf (stderr, "Erreur : les deux fichiers de log portent le même nom !\n");
    logs_liberation (logs);
    return NULL;
  }

  /* Ouverture (en mode écriture) du fichier log pour les réussites */
  logs->descripteur_de_fichier_standard = fopen(logs->fichier_standard, "w");
  if (logs->descripteur_de_fichier_standard) {
    fprintf (logs->descripteur_de_fichier_standard, "%s : Ouverture du fichier : %s : OK\n", __TIME__, logs->fichier_standard);
  } else {
    fprintf (logs->descripteur_de_fichier_erreur,"Erreur d'ouverture du fichier : %s\n", logs->fichier_standard);
    fprintf (logs->descripteur_de_fichier_erreur, "errno : %d\n", errno);

    logs_liberation (logs);

    return NULL;
  }

  /* Ouverture (en mode ecriture) du fichier log pour les erreurs */
  logs->descripteur_de_fichier_erreur = fopen(logs->fichier_erreur, "w");
  if (logs->descripteur_de_fichier_erreur) {
    fprintf (logs->descripteur_de_fichier_standard, "Ouverture du fichier : %s : OK\n", logs->fichier_erreur);
  }  else {
    fprintf (logs->descripteur_de_fichier_erreur,"Erreur d'ouverture du fichier : %s\n", logs->fichier_erreur);
    fprintf (logs->descripteur_de_fichier_erreur, "errno : %d\n", errno);

    logs_liberation (logs);

    return NULL;
  }

  return logs;
}

void logs_liberation (t_logs *logs) {
  if (logs == NULL)
      return;

    if (logs->fichier_standard)
      free (logs->fichier_standard);

    if (logs->fichier_erreur)
      free (logs->fichier_erreur);

    if (logs->descripteur_de_fichier_standard)
      fclose (logs->descripteur_de_fichier_standard);

    if (logs->descripteur_de_fichier_erreur)
      fclose (logs->descripteur_de_fichier_erreur);

    free (logs);
}

FILE*
logs_descripteur_fichier (t_logs *logs, TYPE_OF_LOGS log_type) {
  if (logs == NULL) {
    fprintf (stderr, "logs vaut NULL dans %s();\n", __func__);
    return NULL;
  }

  if (log_type == LOG_STANDARD) {
    return logs->descripteur_de_fichier_standard;
  } else if (log_type == LOG_ERROR) {
    return logs->descripteur_de_fichier_erreur;
  }

  fprintf (logs->descripteur_de_fichier_erreur, "log_type doit prendre une des deux valeurs suivantes : LOG_STANDARD ou LOG_ERROR dans %s();\n", __func__);

  return NULL;
}

void
logs_save (t_logs *logs, const char *text, TYPE_OF_LOGS log_type) {
  if (logs == NULL) {
    fprintf (stderr, "logs vaut NULL dans %s();\n", __func__);
    return;
  }

  if (text == NULL)
  	return;

  if (log_type == LOG_STANDARD) {
    fprintf (logs->descripteur_de_fichier_standard, "%s : %s",  __TIME__, text);
    return;
  } else if (log_type == LOG_ERROR) {
    fprintf (logs->descripteur_de_fichier_erreur, "%s : %s",  __TIME__, text);
    return;
  }

  fprintf (logs->descripteur_de_fichier_erreur, "log_type doit prendre une des deux valeurs suivantes : LOG_STANDARD ou LOG_ERROR dans %s();\n", __func__);
}
