/**
 * @file instrument.h
 * @author Mohamed Hashem
 * @date 15 févr. 2026
 * @brief Définitions des types et prototypes pour la synthèse d'instruments.
 * * @details Ce module définit l'énumération des instruments supportés par le système.
 * Le rendu sonore repose sur une simulation d'enveloppe de volume permettant
 * de passer d'un simple signal carré (Buzzer) à des sons plus complexes.
 */

#ifndef SRC_INSTRUMENT_H_
#define SRC_INSTRUMENT_H_

#include <stdint.h>

/**
 * @enum instrument_t
 * @brief Énumération des instruments disponibles.
 * * @details Chaque instrument possède une enveloppe temporelle spécifique
 * (profil de volume) implémentée dans la fonction @ref jouer_note_style.
 */
typedef enum {
    INSTR_BUZZER = 0,       /**< @brief Signal carré pur (100% volume constant). */
    INSTR_PIANO = 1,        /**< @brief Percussif : Attaque immédiate, déclin (Decay) rapide. */
    INSTR_TROMPETTE = 2,    /**< @brief Cuivres : Attaque "Brass" et sustain stable (souffle). */
    INSTR_GUITARE_ELEC = 3, /**< @brief Électrique : Sustain compressé avec micro-vibrations. */
    INSTR_GUITARE = 4       /**< @brief Acoustique : Pincement sec et résonance de caisse. */
} instrument_t;



/**
 * @brief Convertit l'énumération de l'instrument en une chaîne de caractères lisible.
 * * @param instru L'identifiant de l'instrument à traduire.
 * @return const char* Un pointeur vers le nom de l'instrument (ex: "PIANO", "E-GUITARE").
 */
const char *get_instrument_name(instrument_t instru);

/**
 * @brief Fonction maîtresse de génération sonore avec gestion d'enveloppe.
 * * @details Calcule et applique l'enveloppe de volume en temps réel via le PWM.
 * La fonction itère milliseconde par milliseconde pour sculpter le son.
 * * @param note        La note à jouer (constantes définies dans @file notes.h).
 * @param pitch       L'octave de la note (ex: 3, 4, 5).
 * @param duration_ms Durée totale de la note en millisecondes.
 * @param vol_max     Volume de référence (amplitude maximale) pour le calcul.
 * @param instr       Le modèle d'instrument (@ref instrument_t) à simuler.
 * * @note Cette fonction est bloquante pour la durée de la note, mais reste
 * interruptible grâce à l'utilisation interne de @ref delay_NBC.
 */
void jouer_note_style(uint8_t note, uint8_t pitch, uint32_t duration_ms, uint8_t vol_max, instrument_t instr);

#endif /* SRC_INSTRUMENT_H_ */
