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
