/**
 * @file pwm.c
 * @author Mohamed Hashem
 * @date 13 févr. 2026
 * @brief Pilote périphérique pour la génération de signal PWM via le TIMER2.
 * @details Ce module gère la fréquence (hauteur de la note) et le rapport cyclique
 * (volume) du buzzer. Il utilise une échelle logarithmique pour le volume afin
 * de mieux correspondre à la perception auditive humaine.
 */

#include "pwm.h"
#include "sys/adc.h"

/** * @brief Nombre de divisions pour l'échelle de volume logarithmique.
 */
#define LOG_STEPS 12

/** * @brief Sauvegarde de la fréquence d'horloge d'entrée du Timer 2.
 */
uint32_t clock_tim2 = 0;

/* ==========================================================================
 * FONCTIONS DE CONTRÔLE MUSICAL
 * ========================================================================== */

/**
 * @brief Met à jour la fréquence du signal (Hauteur de la note).
 * @details Modifie le registre Auto-Reload (ARR) du Timer. Grâce au bit ARPE
 * activé lors de l'initialisation, le changement ne prend effet qu'au cycle suivant.
 * @param frequency Fréquence cible en Hertz.
 */
void update_pwm_frequency(uint32_t frequency) {
    // Calcul de la valeur de débordement (ARR)
    uint32_t new_arr = (uint32_t)(clock_tim2 / frequency) - 1;
    TIM2.ARR = new_arr;

    // Génère un événement de mise à jour pour forcer le nouveau réglage si nécessaire
    TIM2.EGR |= (1 << 0);
}

/**
 * @brief Met à jour le volume du signal (Rapport cyclique).
 * @details Le volume est traité sur une échelle logarithmique par décalage de bits (bit-shift).
 *
 * * @note Correspondance approximative :
 * - 100% : Division par 2 (Signal carré pur)
 * - 50%  : Division par 128
 * - 0%   : Registre à 0 (Silence)
 * * @param current_vol_percent Volume souhaité en pourcentage (0-100).
 */
void update_pwm_volume(uint8_t current_vol_percent){
	if (current_vol_percent == 0) {
		TIM2.CCR2 = 0;
	} else {
		// Calcul du palier logarithmique
		uint8_t step = (100 - current_vol_percent) * LOG_STEPS / 100;
		// On décale ARR pour obtenir le rapport cyclique (Duty Cycle)
		TIM2.CCR2 = TIM2.ARR >> (step + 1);
	}
}

/**
 * @brief Lit la valeur actuelle du potentiomètre.
 * @return uint32_t Valeur brute issue du convertisseur ADC.
 */
uint32_t mesure_potentiometre(){
	return ADC_convert_single_channel(&ADC1);
}

/* ==========================================================================
 * INITIALISATION MATÉRIELLE
 * ========================================================================== */

/**
 * @brief Configure la broche PB9 en mode Fonction Alternative (AF1) pour le Timer 2.
 * @details Redirige la sortie du Timer 2 Channel 2 vers le Buzzer physique.
 */
void init_Buzzer_AF(){
	// Mode Fonction Alternative sur PB9
	GPIOB.MODER &= ~(0x3 << (PIN_BUZZER_B << 1));
	GPIOB.MODER |= (0x2 << (PIN_BUZZER_B << 1));

	// Sélection de AF1 dans le registre AFRH
	GPIOB.AFRH &= ~(0xf << 4);
	GPIOB.AFRH |= (0x1 << 4);
}

/**
 * @brief Initialise le Timer 2 en mode PWM.
 * @details
 * - Active l'horloge sur le bus APB1.
 * - Configure le Channel 2 en PWM mode 1.
 * - **Preload activé** sur ARR (fréquence) et CCR2 (volume) pour garantir
 * des transitions fluides sans "glitches" audio.
 * @param clock Fréquence de l'horloge système alimentant le Timer.
 */
void init_TIM2_CH2(uint32_t clock){
    clock_tim2 = clock;

    // Activation de l'horloge du périphérique TIM2
    RCC.APB1ENR |= (1 << 0);

    TIM2.PSC = 0;    // Pas de diviseur de fréquence
    TIM2.ARR = 1000; // Valeur par défaut
    TIM2.CCR2 = 0;   // Volume initial nul

    /* --- CONFIGURATION CCMR1 --- */
    // Mode PWM 1 (OC2M = 110)
    TIM2.CCMR1 &= ~(0x7 << 12);
    TIM2.CCMR1 |= (0x6 << 12);

    // OC2PE : Output Compare 2 Preload Enable
    // Permet de synchroniser le changement de volume avec le début d'une période
    TIM2.CCMR1 |= (1 << 11);

    /* --- CONFIGURATION CR1 --- */
    // ARPE : Auto-reload Preload Enable
    // Permet de synchroniser le changement de note avec le début d'une période
    TIM2.CR1 |= (1 << 7);

    // Activation de la sortie sur le canal 2
    TIM2.CCER |= (1 << 4);

    // Lancement du compteur
    TIM2.CR1 |= (1 << 0);
}

/**
 * @brief Configure les broches et le convertisseur ADC pour le potentiomètre.
 * @details Utilise le canal 8 de l'ADC1 avec un temps d'échantillonnage maximal.
 */
void init_potentiometre(){
	// Configuration de la broche en mode Analogique
	GPIOB.MODER |= 0x3;

	// Initialisation du convertisseur
	ADC_init_single_channel(&ADC1, 8);
	ADC_set_sampling_time(&ADC1, 8, 0x7);
}
