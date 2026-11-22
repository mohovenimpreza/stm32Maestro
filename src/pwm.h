/**
 * @file pwm.h
 * @author Mohamed Hashem
 * @date 13 fév. 2026
 * @brief Header pour la gestion du signal PWM et des périphériques associés.
 * * @details Ce module définit les fonctions nécessaires à l'initialisation du
 * Timer 2 pour la génération sonore, ainsi qu'à la lecture du potentiomètre
 * via l'ADC pour le contrôle utilisateur.
 */

#ifndef SRC_PWM_H_
#define SRC_PWM_H_

#include <stdint.h>

/** * @brief Index de la broche utilisée pour le Buzzer sur le PORT B.
 * @details PB9 correspond généralement au canal 2 du Timer 2 (TIM2_CH2).
 */
#define PIN_BUZZER_B 9

/* ==========================================================================
 * INITIALISATION ET CONFIGURATION
 * ========================================================================== */

/**
 * @brief Configure la broche physique en mode Fonction Alternative.
 * @details Bascule la broche @ref PIN_BUZZER_B du mode GPIO standard vers
 * la sortie du Timer (AF1).
 */
void init_Buzzer_AF();

/**
 * @brief Initialise le périphérique Timer 2 pour le mode PWM.
 * @param clock Fréquence du bus APB (en Hz) alimentant le Timer.
 */
void init_TIM2_CH2(uint32_t clock);

/**
 * @brief Initialise le convertisseur analogique-numérique (ADC).
 * @details Prépare la lecture du potentiomètre pour le contrôle du volume
 * ou du tempo.
 */
void init_potentiometre();

/* ==========================================================================
 * CONTRÔLE TEMPS RÉEL
 * ========================================================================== */



/**
 * @brief Modifie la fréquence de sortie (hauteur de la note).
 * @param frequency Valeur de la fréquence cible en Hertz.
 */
void update_pwm_frequency(uint32_t frequency);

/**
 * @brief Modifie le rapport cyclique (intensité sonore).
 * @details Utilise une conversion interne pour transformer le pourcentage
 * en une valeur de registre logarithmique.
 * @param current_vol_percent Volume cible de 0 à 100%.
 */
void update_pwm_volume(uint8_t current_vol_percent);

/**
 * @brief Récupère la valeur actuelle lue sur le potentiomètre.
 * @return uint32_t Valeur numérisée (0 à 4095 pour un ADC 12 bits).
 */
uint32_t mesure_potentiometre();

#endif /* SRC_PWM_H_ */
