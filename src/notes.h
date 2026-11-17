/**
 * @file notes.h
 * @author Mohamed Hashem
 * @date 13 fév. 2026
 * @brief Définitions des types fondamentaux pour la manipulation des notes et du rythme.
 * * @details Ce fichier contient les énumérations pour les hauteurs de notes (échelle chromatique)
 * et les durées relatives (solfège). Il définit également la structure @ref Note qui est
 * l'unité de base d'une partition.
 */

#ifndef SRC_NOTES_H_
#define SRC_NOTES_H_

#include <stdint.h>

/**
 * @enum notes_t
 * @brief Indexation des notes de l'échelle chromatique.
 * @details Utilise les noms français. Les alias pour les dièses (#) et bémols (b)
 * permettent une écriture plus naturelle des partitions.
 */
typedef enum {
    SILENCE = -1,           /**< Absence de son. */
    DO,                     /**< Do / C */
    DO_DIESE,               /**< Do# / C# */
    RE_BEMOL = DO_DIESE,    /**< Réb (Enharmonique de Do#) */
    RE,                     /**< Ré / D */
    RE_DIESE,               /**< Ré# / D# */
    MI_BEMOL = RE_DIESE,    /**< Mib (Enharmonique de Ré#) */
    MI,                     /**< Mi / E */
    FA,                     /**< Fa / F */
    FA_DIESE,               /**< Fa# / F# */
    SOL_BEMOL = FA_DIESE,   /**< Solb (Enharmonique de Fa#) */
    SOL,                    /**< Sol / G */
    SOL_DIESE,              /**< Sol# / G# */
    LA_BEMOL = SOL_DIESE,   /**< Lab (Enharmonique de Sol#) */
    LA,                     /**< La / A (440Hz à l'octave 4) */
    LA_DIESE,               /**< La# / A# */
    SI_BEMOL = LA_DIESE,    /**< Sib (Enharmonique de La#) */
    SI                      /**< Si / B */
} notes_t;

/**
 * @enum valeur_t
 * @brief Durées relatives des notes.
 * @details Les valeurs sont exprimées en centièmes de "temps" (base 100 pour une NOIRE)
 * afin d'éviter l'usage de flottants dans les calculs rapides.
 */
typedef enum {
    RONDE          = 400,   /**< 4 temps. */
    BLANCHE_POINTE = 300,   /**< 3 temps. */
    BLANCHE        = 200,   /**< 2 temps. */
    NOIRE_POINTE   = 150,   /**< 1.5 temps. */
    NOIRE          = 100,   /**< 1 temps (unité de référence). */
    CROCHE_POINTE  = 75,    /**< 0.75 temps. */
    CROCHE         = 50,    /**< 0.5 temps. */
    DOUBLE_CROCHE  = 25     /**< 0.25 temps. */
} valeur_t;



/**
 * @struct Note
 * @brief Structure représentant une note jouable.
 * @details Combine une fréquence (note/octave) et une durée (rythme).
 */
typedef struct {
    uint8_t note;    /**< Index de la note (@ref notes_t). */
    uint8_t octave;  /**< Octave (typiquement 0 à 8). */
    uint16_t valeur; /**< Durée relative (@ref valeur_t). */
} Note;

/* --- Variables Globales Externes --- */

/** @brief Fréquences de référence de l'octave 0. */
extern const float CHROMATIC_SCALE[12];

/** @brief Coefficient de correction pour compenser la dérive matérielle. */
extern const float correction_freq;

/* --- Prototypes des fonctions --- */

/**
 * @brief Retourne le nom textuel de la durée (ex: "NOIRE").
 * @param duration Valeur brute de la durée.
 */
const char* get_duration_name(uint16_t duration);

/**
 * @brief Retourne le nom de la note (ex: "DO#").
 * @param note Index de la note.
 */
const char* get_note_name(uint8_t note);

/**
 * @brief Calcule la fréquence en Hz pour une note et une octave.
 */
uint32_t get_frequency(notes_t note, uint8_t pitch);

/**
 * @brief Calcule la durée absolue en ms selon le BPM du morceau.
 */
uint32_t get_duration_ms(valeur_t val, uint16_t bpm, uint8_t denom_beat);

#endif /* SRC_NOTES_H_ */
