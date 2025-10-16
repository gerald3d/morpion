#ifndef __LIST__
#define __LIST__

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

/*! \class t_liste
    \brief Chaîne doublement chaînée.

    Pour l'instant elle permet seulement d'ajouter en début ou en fin de liste un nouvel élément.
*/

/*!
 * \private Structure privée.
 * L'utilisateur final ne doit pas accéder à un de ses membres directement.
 */
typedef struct t_liste t_liste;

/*!
 * \private Structure privée.
 * L'utilisateur final ne doit pas accéder à un de ses membres directement.
 */
struct t_liste {
  t_liste *precedent;
  t_liste *suivant;
  void *donnee;
};

/*!
 * \brief Ajoute un élément à la fin de la liste.
 * @param liste Pointeur sur la liste à modifier.
 * @param donnee Pointeur sur la donnée à ajouter à la liste.
 *
 * Si liste vaut NULL une nouvelle liste est créée.
 */
t_liste *liste_ajout_fin (t_liste *liste, void *donnee);

/*!
 * \brief Ajoute un élément au début de la liste.
 * @param liste Pointeur sur la liste à modifier.
 * @param donnee Pointeur sur la donnée à ajouter à la liste.
 *
 * Si liste vaut NULL une nouvelle liste est créée.
 */
t_liste *list_ajout_debut (t_liste *liste, void *donnee);

/*!
 * \brief Enlève un élément de la liste.
 * @param liste Pointeur sur la liste à modifier.
 * @param donnee Pointeur sur la donnée à enlever.
 *
 * L'élément enlevé n'est pas désalloué durant l'opération.
 *
 * Si donnee vaut NULL l'opération est sans effet.
 *
 * Si liste vaut NULL un mesage est envoié à la console.
 */
t_liste *liste_enleve_un_maillon (t_liste *liste, void *donnee);

/*!
 * \brief Destructeur de l'objet t_ihm_sdl.
 * @param liste Liste à détruire
 *
 * Le pointeur vaut NULL au retour de la fonction.
 *
 * IMPORTANT : les données de chaque maillon ne sont pas détruits lors de l'opération.
 */
void liste_liberation (t_liste **liste);

#endif
