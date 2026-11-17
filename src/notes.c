/**
 * @file notes.c
 * @author Mohamed Hashem
 * @date 14 févr. 2026
 * @brief Moteur de calcul des fréquences et des durées musicales.
 * * @details Ce fichier contient les fonctions permettant de traduire les structures
 * musicales en signaux temporels. Il inclut un système de correction de fréquence
 * pour compenser les dérives de l'horloge système ou du buzzer.
 */

#include "notes.h"

/* ==========================================================================
 * TABLES DE CORRESPONDANCE ET CONSTANTES
 * ========================================================================== */

/** * @brief Noms courts des notes pour l'affichage (LCD/UART).
 */
const char* note_name[] = {
    "DO ", "DO#", "RE ", "RE#", "MI ", "FA ", "FA#", "SOL", "SOL#", "LA ", "LA#", "SI "
};

/** * @brief Coefficient de correction empirique des fréquences.
 * @details Résulte de la moyenne des ratios entre les fréquences théoriques et
 * les mesures réelles effectuées à l'oscilloscope.
 * @note Ratio moyen mesuré : 0.9845694f
 */
const float correction_freq = 0.984569438495132f;

/** * @brief Fréquences de base de l'octave 0 (Échelle Chromatique).
 * @details Ces valeurs servent de base pour le calcul de toutes les octaves supérieures.
 */
const float CHROMATIC_SCALE[12] = {
    16.35, 17.32, 18.35, 19.45, 20.60, 21.83, 23.12, 24.50, 25.96, 27.50, 29.14, 30.87
};



/* ==========================================================================
 * FONCTIONS D'INTERFACE ET DE CALCUL
 * ========================================================================== */

/**
 * @brief Récupère le nom textuel d'une note.
 * @param note Index de la note (0-11).
 * @return const char* Chaîne de caractères (ex: "DO#") ou "SILENCE".
 */
const char* get_note_name(uint8_t note) {
    if (note >= 0 && note < 12) {
        return note_name[note];
    }
    return "SILENCE";
}

/**
 * @brief Récupère le nom textuel d'une durée.
 * @param duration Valeur brute de la durée (définie dans notes.h).
 * @return const char* Libellé lisible (ex: "NOIRE P.").
 */
const char* get_duration_name(uint16_t duration) {
    switch (duration) {
        case RONDE:          return "RONDE";
        case BLANCHE_POINTE: return "BLANCHE P.";
        case BLANCHE:        return "BLANCHE";
        case NOIRE_POINTE:   return "NOIRE P.";
        case NOIRE:          return "NOIRE";
        case CROCHE_POINTE:  return "CROCHE P.";
        case CROCHE:         return "CROCHE";
        case DOUBLE_CROCHE:  return "D-CROCHE";
        default:             return "---";
    }
}

/**
 * @brief Calcule la fréquence absolue d'une note à une octave donnée.
 * @details La formule utilisée est :
 * $$f = f_{base} \times 2^{pitch} \times k_{correction}$$
 * * @param note  Index de la note (@ref notes_t).
 * @param pitch Octave souhaitée.
 * @return uint32_t Fréquence calculée en Hertz.
 */
uint32_t get_frequency(notes_t note, uint8_t pitch){
	float target_freq = CHROMATIC_SCALE[note] * (1 << pitch);
	target_freq *= correction_freq; // Application du coefficient correcteur
	return (uint32_t) target_freq;
}

/**
 * @brief Calcule la durée d'une note en millisecondes.
 * @details La fonction prend en compte le tempo (BPM) et la signature rythmique.
 * * @formula La durée est déterminée par la base de temps d'une mesure entière (ronde) :
 * $$T_{ms} = \frac{240000}{BPM \times denom\_beat} \times \frac{val}{100}$$
 * * @param val        Coefficient multiplicateur de la note (ex: 100 pour une NOIRE).
 * @param bpm        Tempo en battements par minute.
 * @param denom_beat Dénominateur de la signature (ex: 4 pour un temps en noire).
 * @return uint32_t  Durée d'exécution en millisecondes.
 */
uint32_t get_duration_ms(valeur_t val, uint16_t bpm, uint8_t denom_beat) {
    // Calcul de la durée de référence d'une unité relative
    uint32_t ms_per_unit = (240000) / (bpm * denom_beat);

    // Retourne la durée finale ajustée par la valeur de la note
    return (ms_per_unit * val) / 100;
}



