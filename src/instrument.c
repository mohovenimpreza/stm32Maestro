/**
 * @file instrument.c
 * @author Mohamed Hashem
 * @date 15 févr. 2026
 * @brief Moteur de synthèse sonore et gestion des enveloppes d'instruments.
 * * @details Ce fichier contient la logique de sculpture du signal PWM. En modifiant
 * le rapport cyclique (volume) milliseconde par milliseconde, on simule la
 * réponse physique de différents instruments (percussion, souffle, corde).
 */

#include "instrument.h"
#include "pwm.h"
#include "notes.h"
#include "delays.h"

/** * @brief Table de correspondance pour l'affichage textuel des instruments.
 */
const char *instrument_name[] = {"BUZZER", "PIANO", "TROMPETTE", "E-GUITARE"};



/**
 * @brief Joue une note avec une enveloppe de volume spécifique à l'instrument.
 * * @details Cette fonction est le cœur du rendu sonore. Elle utilise une boucle
 * temporelle fine pour modifier le volume en temps réel.
 * * @param note        Index de la note (définie dans notes.h, ex: DO, RE...).
 * @param pitch       Octave de la note (typiquement entre 3 et 5).
 * @param duration_ms Durée totale de la note en millisecondes.
 * @param vol_max     Amplitude maximale du signal (0-100%).
 * @param instr       Type d'instrument à simuler (@ref instrument_t).
 * * @note La fonction utilise @ref delay_NBC(1) à chaque itération, permettant
 * d'interrompre la note instantanément si l'utilisateur appuie sur "Stop".
 */
void jouer_note_style(uint8_t note, uint8_t pitch, uint32_t duration_ms, uint8_t vol_max, instrument_t instr) {

    /* --- Gestion du silence --- */
    if (note == SILENCE){
        update_pwm_volume(0);
        delay_NBC(duration_ms);
        return;
    }

    /* --- Initialisation de la fréquence --- */
    uint32_t freq = get_frequency(note, pitch);
    update_pwm_frequency(freq);

    /**
     * @brief Boucle de synthèse granulaire (1ms).
     * @details On sculpte la courbe de volume 'v' en fonction du temps 't'.
     */
    for (uint32_t t = 0; t < duration_ms; t++) {
        uint8_t v = 0;

        switch(instr) {

            case INSTR_PIANO:
                /** @details Piano : Attaque percussive immédiate suivie d'un déclin constant. */
                if (t < 3) {
                    v = (t * vol_max) / 3;
                }
                else {
                    int32_t decay = (t * vol_max) / (duration_ms * 2);
                    int32_t calc_v = vol_max - decay;
                    v = (calc_v < (vol_max * 50 / 100)) ? (vol_max * 50 / 100) : (uint8_t)calc_v;
                }
                break;

            case INSTR_TROMPETTE:
                /** @details Trompette : Attaque plus lente (30ms) et sustain stable (souffle). */
                if (t < 30) {
                    v = (t * vol_max) / 30;
                }
                else if (t < 60) {
                    v = vol_max - ((t - 30) * (vol_max * 20 / 100) / 30);
                }
                else {
                    v = (vol_max * 80) / 100;
                    if (duration_ms - t < 15) { // Release final
                        v = (v * (duration_ms - t)) / 15;
                    }
                }
                break;

            case INSTR_GUITARE:
                /** @details Guitare Acoustique : "Twang" initial suivi d'une résonance longue. */
                if (t < 5) {
                    v = (t * vol_max) / 5;
                }
                else {
                    uint32_t decay_end = duration_ms / 4;
                    if (t < decay_end) {
                        v = vol_max - ((t - 5) * (vol_max * 40 / 100) / (decay_end - 5));
                    } else {
                        int32_t resonance = ( (t - decay_end) * (vol_max * 30 / 100) ) / (duration_ms - decay_end);
                        int32_t calc_v = (vol_max * 60 / 100) - resonance;
                        v = (calc_v < 0) ? 0 : (uint8_t)calc_v;
                    }
                }
                break;

            case INSTR_GUITARE_ELEC:
                /** @details Guitare Électrique : Sustain compressé et simulation de micro-distorsion. */
                if (t < 10) {
                    v = (t * vol_max) / 10;
                }
                else {
                    uint8_t base_v = (vol_max * 90) / 100;
                    v = (t % 4 < 2) ? base_v : (base_v - (vol_max * 8 / 100)); // Tremblement
                    v -= (t * (vol_max * 10 / 100)) / duration_ms; // Déclin léger
                }
                break;

            default:
                /** @details Buzzer (Défaut) : Volume constant sur toute la durée. */
                v = vol_max;
                break;
        }

        /* Mise à jour du PWM et vérification des conditions de sortie */
        update_pwm_volume(v);
        delay_NBC(1);
    }

    /* Arrêt propre du son pour éviter les résonances parasites */
    update_pwm_volume(0);
}

/**
 * @brief Récupère le nom de l'instrument sous forme de texte.
 * @param instru Identifiant de l'instrument.
 * @return const char* Nom lisible (ex: "PIANO").
 */
const char* get_instrument_name(instrument_t instru) {
    if (instru >= 0 && instru < 4) {
        return instrument_name[instru];
    }
    return "---";
}
