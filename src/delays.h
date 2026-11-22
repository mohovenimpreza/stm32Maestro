/**
 * @file delays.h
 * @author Mohamed Hashem
 * @date 15 févr. 2026
 * @brief Header pour la gestion des temporisations du système de synthèse musicale.
 * * @details Ce module définit les interfaces pour les délais bloquants et réactifs.
 * Il repose sur des variables globales volatiles mises à jour par des interruptions
 * matérielles (ISR), assurant la synchronisation entre le temps réel et la lecture.
 */

#ifndef SRC_DELAYS_H_
#define SRC_DELAYS_H_

#include <stdint.h>

/* ==========================================================================
 * VARIABLES GLOBALES EXTERNES
 * ========================================================================== */

/** * @brief État de lecture de la partition.
 * @details 1 si la musique est en cours de lecture, 0 si elle est en pause ou arrêtée.
 * Cette variable est scrutée par @ref delay_NBC pour permettre une sortie prématurée.
 */
extern volatile uint8_t playing;

/** * @brief Compteur temporel de référence.
 * @details Incrémenté périodiquement (ex: toutes les 1ms) par l'interruption du
 * System Tick (Systick) ou d'un Timer matériel.
 */
extern volatile uint32_t count;

/** * @brief Indicateur de progression du morceau.
 * @details Utilisé pour détecter un changement de morceau ou une réinitialisation.
 * Si cette valeur tombe à 0, @ref delay_NBC interrompt son attente.
 */
extern volatile uint8_t progress;


/* ==========================================================================
 * PROTOTYPES DES FONCTIONS
 * ========================================================================== */

/**
 * @brief Génère une temporisation bloquante (Blocking).
 * * @details Cette fonction réinitialise @ref count et boucle jusqu'à ce que
 * celui-ci atteigne la valeur cible. Elle ne tient pas compte de l'état du système.
 * * @param t Valeur cible à atteindre par le compteur (ex: nombre de ticks).
 */
void delay_BC(uint32_t t);

/**
 * @brief Génère une temporisation réactive et non-bloquante (Non-Blocking Context).
 * * @details C'est la fonction principale pour la lecture des notes. Elle permet de
 * respecter le tempo tout en restant à l'écoute des événements système.
 * * @note Sortie anticipée si :
 * - @ref playing passe à 0.
 * - @ref progress passe à 0.
 * * @param t Valeur cible maximale pour le délai.
 */
void delay_NBC(uint32_t t);

#endif /* SRC_DELAYS_H_ */
