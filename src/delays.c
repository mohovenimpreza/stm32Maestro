/**
 * @file delays.c
 * @brief Gestion des temporisations bloquantes et non-bloquantes.
 *
 * @details Ce module fournit des fonctions de délai basées sur un compteur global
 * incrémenté par une interruption timer (IT). Il permet de gérer le rythme des
 * notes tout en restant réactif aux commandes de l'utilisateur (Stop/Play).
 * * @author Mohamed Hashem
 * @date 15 fév. 2026
 */

#include "delays.h"

/**
 * @brief Temporisation bloquante stricte (Blocking Delay).
 * * @details Cette fonction suspend l'exécution du thread principal en attendant
 * activement que le compteur matériel atteigne la valeur cible.
 * @warning À utiliser avec précaution : aucune autre tâche de fond (hors IT)
 * ne pourra s'exécuter pendant ce délai.
 * * @param t Durée du délai (unités dépendantes de la fréquence d'incrémentation de count).
 */
void delay_BC(uint32_t t){
    count = 0;              /**< Réinitialisation du compteur global partagé */
    while (count < t);      /**< Boucle d'attente active (Polling) */
}

/**
 * @brief Temporisation non-bloquante conditionnelle (Non-Blocking Delay).
 * * @details Cette fonction est conçue spécifiquement pour la lecture musicale.
 * Contrairement à delay_BC, elle surveille l'état du système à chaque itération.
 * Elle permet de sortir instantanément du délai si l'utilisateur arrête la musique,
 * évitant ainsi un "gel" de l'interface entre deux notes.
 * *
 * * @note La sortie anticipée se produit si :
 * - @ref playing devient faux (Musique mise en pause ou arrêtée).
 * - @ref progress est réinitialisé à 0 (Changement de morceau).
 * * @param t Durée maximum du délai.
 */
void delay_NBC(uint32_t t){
    count = 0;              /**< Réinitialisation du compteur global */

    /**
     * @details La boucle de surveillance vérifie trois conditions critiques :
     * 1. Le temps écoulé (count < t)
     * 2. L'autorisation de lecture (playing)
     * 3. L'intégrité du curseur de lecture (progress != 0)
     */
    while (playing && progress != 0 && count < t);
}
