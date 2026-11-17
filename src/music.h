/**
 * @file music.h
 * @author Mohamed Hashem
 * @date 13 févr. 2026
 * @brief Header définissant les structures de données musicales et les prototypes associés.
 * * @details Ce module permet d'encapsuler une partition complète (notes, BPM, signature rythmique)
 * et de gérer une bibliothèque de morceaux via la structure @ref Tab_Music.
 */

#ifndef SRC_MUSIC_H_
#define SRC_MUSIC_H_

#include <stdint.h>
#include "notes.h"

/** * @brief Taille maximale du nom d'une partition pour l'affichage (LCD/Console).
 */
#define MAX_STR_NAME 32

/* ==========================================================================
 * TYPES ET STRUCTURES
 * ========================================================================== */

/**
 * @struct Partition
 * @brief Structure représentant un morceau de musique complet.
 * * @details Elle contient toutes les métadonnées nécessaires au moteur de lecture
 * pour calculer la durée réelle des notes en fonction du tempo.
 */
typedef struct {
    char name[MAX_STR_NAME]; /**< Nom du morceau (ex: "Star Wars"). */
    uint8_t size;            /**< Nombre total de notes dans le tableau notes. */
    uint8_t bpm;             /**< Battements par minute (Tempo). */
    uint8_t num_beat;        /**< Numérateur de la signature (ex: 3 pour une valse). */
    uint8_t denom_beat;      /**< Dénominateur de la signature (ex: 4 pour une noire). */
    Note *notes;             /**< Pointeur vers le tableau de structures @ref Note. */
} Partition;

/**
 * @struct Tab_Music
 * @brief Conteneur pour la bibliothèque musicale globale.
 * * @details Cette structure permet d'itérer facilement sur l'ensemble des musiques
 * disponibles dans le système (playlist).
 */
typedef struct {
    uint8_t size;    /**< Nombre de partitions présentes dans le tableau. */
    Partition **tab; /**< Tableau de pointeurs vers les structures @ref Partition. */
} Tab_Music;



/* ==========================================================================
 * PROTOTYPES DES FONCTIONS
 * ========================================================================== */

/**
 * @brief Calcule la durée réelle d'une note en millisecondes.
 * * @details Convertit une valeur rythmique (NOIRE, CROCHE, etc.) en temps absolu
 * en tenant compte du tempo (BPM) et de la signature temporelle.
 * * @formula La durée est généralement calculée via :
 * $$T_{ms} = \frac{60000}{BPM \times \frac{valeur\_note}{denom\_beat}}$$
 * * @param val         Type de note (valeur_t : NOIRE, BLANCHE, etc.).
 * @param bpm         Tempo du morceau.
 * @param denom_beat  Dénominateur de la mesure (ex: 4).
 * @return uint32_t   La durée en millisecondes à transmettre aux fonctions de délai.
 */
uint32_t get_duration_ms(valeur_t val, uint16_t bpm, uint8_t denom_beat);

/* ==========================================================================
 * VARIABLES GLOBALES EXTERNES
 * ========================================================================== */

/** * @brief Instance globale de la bibliothèque musicale.
 * @details Définie dans @file music.c, elle regroupe tous les morceaux compilés.
 */
extern Tab_Music musics;

#endif /* SRC_MUSIC_H */
