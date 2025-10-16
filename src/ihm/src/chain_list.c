#include "chain_list.h"

t_liste*
liste_ajout_fin (t_liste *liste, void *donnee) {
  t_liste *nouvel_element = malloc (sizeof (t_liste));

  if (nouvel_element == NULL) {
    fprintf (stderr, "Erreur dans %s(); : erreur d'allocation mémoire d'un nouvel élément.\n", __func__);
    return liste;
  }

  /* Si c'est une nouvelle liste */
  if (liste == NULL) {
    liste = nouvel_element;
    nouvel_element->precedent = NULL;
  } else {
    /* Recherche du dernier élément de la liste */
    t_liste *liste_tmp = liste;
    while (liste_tmp->suivant)
      liste_tmp = liste_tmp->suivant;

    nouvel_element->precedent = liste_tmp;
    liste_tmp->suivant = nouvel_element;
  }

  nouvel_element->suivant = NULL;

  nouvel_element->donnee = donnee;

  return liste;
}

t_liste*
list_ajout_debut (t_liste *liste, void *donnee) {
  t_liste *nouvel_element = malloc (sizeof (t_liste));

  if (nouvel_element == NULL) {
    fprintf (stderr, "Erreur dans %s(); : erreur d'allocation mémoire d'un nouvel élément.\n", __func__);
    return liste;
  }

  /* Recherche du premier élément de la liste */
  t_liste *liste_tmp = liste;
  while (liste_tmp->precedent)
    liste_tmp = liste_tmp->precedent;

  /* Si c'est une nouvelle liste */
  if (liste == NULL) {
    nouvel_element->suivant = NULL;
  }
  else {
    liste_tmp->precedent = nouvel_element;
    nouvel_element->suivant = liste_tmp;
  }

  nouvel_element->precedent = NULL;

  nouvel_element->donnee = donnee;

  return nouvel_element;
}

t_liste*
liste_enleve_un_maillon (t_liste *liste, void *donnee) {
	if (liste == NULL) {
    fprintf (stderr, "Erreur dans %s(); : **liste ne doit pas être NULL.\n", __func__);
    return NULL;
  }

  if (donnee == NULL)
    return NULL;

	/* Recherche de la donnée dans la liste et suppression du maillon correspondant*/
  t_liste *list = liste;
  t_liste *element_a_supprimer = NULL;
  t_liste *nouvelle_liste = NULL;
  while (list) {
		if (list->donnee == donnee) {
			t_liste *maillon_precedent = list->precedent;
			t_liste *maillon_suivant = list->suivant;

			/* Si c'est le premier élément de la liste à supprimer -> mémorisation de l'élément suivant comme nouveau pointeur pour la liste */
			if (list == liste)
				nouvelle_liste = maillon_suivant;
			else
				nouvelle_liste = liste;

			element_a_supprimer = list;

			/* Attachement du maillon précédent au maillon suivant */
			if (maillon_precedent)
				maillon_precedent->suivant = maillon_suivant;
			/* Attachement du maillon suivant au maillon précédent */
			if (maillon_suivant)
				maillon_suivant->precedent = maillon_precedent;

			/* Destruction du maillon courant */
			free (element_a_supprimer);

			return nouvelle_liste;
		}

		list = list->suivant;
  }

  return liste;
}

void
liste_liberation (t_liste **liste) {
  if (liste == NULL) {
    fprintf (stderr, "Erreur dans %s(); : **liste ne doit pas être NULL.\n", __func__);
    return;
  }

  if (*liste == NULL)  {
    fprintf (stderr, "Erreur dans %s(); : liste ne doit pas être NULL.\n", __func__);
    return;
  }

  t_liste *liste_tmp = *liste;
  t_liste *liste_suivant_tmp;

  while (liste_tmp != NULL) {
    liste_suivant_tmp = liste_tmp->suivant;
    free (liste_tmp);
    liste_tmp = liste_suivant_tmp;
  }

  *liste = NULL;
}
