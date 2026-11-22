/**
 * @file main.c
 * @author Mohamed Hashem
 * @date 28 fév. 2026
 * @brief Point d'entrée du synthétiseur musical interactif "Maestro STM32".
 * * @details Ce fichier orchestre la lecture de partitions et la synthèse sonore
 * en temps réel. Il gère la machine à états complexe permettant de basculer
 * entre la lecture automatique et le mode "Musicien" (jeu libre).
 * * L'interface utilisateur est déportée sur un terminal série utilisant des
 * séquences ANSI pour un affichage dynamique et coloré.
 */

#include <stdio.h>
#include <string.h>
#include <math.h>

/* Drivers système */
#include "sys/cm4.h"
#include "sys/devices.h"
#include "sys/init.h"
#include "sys/clock.h"
#include "sys/systick.h"
#include "sys/button.h"
#include "sys/serial_io.h"

/* Modules applicatifs */
#include "pwm.h"
#include "music.h"
#include "delays.h"
#include "instrument.h"

/**
 * @enum mode_t
 * @brief Énumération des modes de fonctionnement basés sur les switches du shield.
 * @details Les valeurs correspondent à un masquage de bits (Bitmask) :
 * - Bits 0-1 : Modulateurs (Vol, BPM, Pitch)
 * - Bit 2    : Activation du Timbre (Instrument)
 * - Bit 3    : Activation du mode Musicien (Prioritaire)
 */
typedef enum {
    NORMAL = 0,             /**< Lecture standard sans modification. */
    VOLUME_MODULATOR = 1,   /**< Potentiomètre contrôle le volume. */
    BPM_MODULATOR = 2,      /**< Potentiomètre contrôle le tempo. */
    PITCH_MODULATOR = 3,    /**< Potentiomètre contrôle l'octave. */
    TIMBRE_MODULATOR = 4,   /**< Sélection de l'instrument. */
    MUSICIEN = 8            /**< Mode jeu libre (Potentiomètre = Note). */
} mode_t;

/**
 * @name Paramètres de Synthèse
 * @brief Variables définissant le rendu sonore de la note actuelle.
 * @{
 */
uint8_t mode;               /**< Mode de fonctionnement extrait des switches. */
uint8_t music_index = 0;    /**< Index du morceau dans la playlist @ref musics. */
uint32_t pot_mesure;        /**< Valeur brute de l'ADC (0-4095). */
uint8_t volume;             /**< Volume cible (0-100%). */
uint8_t pitch;              /**< Octave de rendu. */
uint16_t bpm;               /**< Tempo en BPM. */
uint8_t note;               /**< Note MIDI à jouer. */
uint16_t duree;             /**< Durée rythmique (@ref valeur_t). */
uint8_t quadrature;         /**< Dénominateur temporel (ex: 4 pour 4/4). */
instrument_t timbre;        /**< Enveloppe ADSR sélectionnée (@ref instrument_t). */
/** @} */

/**
 * @name Contexte d'Interruption
 * @brief Variables volatiles modifiées par le matériel (ISR).
 * @{
 */
volatile uint32_t count = 0;    /**< Horloge système (ms) via @ref SysTick_Handler. */
volatile uint8_t progress = 0;  /**< Curseur de position dans la partition. */
volatile uint8_t playing = 0;   /**< Flag d'état : 1 = Play, 0 = Pause. */
/** @} */



/* * Prototypes des fonctions de service
 */

/**
 * @brief Convertit l'entrée ADC en pourcentage de volume.
 * @param val_pot Valeur 12 bits (0-4095).
 * @return Volume (0-100).
 */
uint8_t convert_2_volume(uint32_t val_pot);

/**
 * @brief Convertit l'entrée ADC en tempo avec courbe hybride (linéaire/quadratique).
 * @param val_pot Valeur 12 bits.
 * @return Tempo en BPM (50 - 2500).
 */
uint16_t convert_2_bpm(uint32_t val_pot);

/**
 * @brief Convertit l'entrée ADC en octave relative.
 * @param val_pot Valeur 12 bits.
 * @return Pitch (0-3).
 */
uint8_t convert_2_pitch(uint32_t val_pot);

/**
 * @brief Convertit l'entrée ADC en index de note (0-11).
 * @param val_pot Valeur 12 bits.
 * @return Index de note (@ref notes_t).
 */
uint8_t convert_2_note(uint32_t val_pot);

/**
 * @brief Récupère le nom formaté du mode actuel pour l'affichage.
 * @param mode Valeur brute du mode.
 * @return Chaîne de caractères descriptive.
 */
const char* get_mode_name(uint8_t mode);

/**
 * @brief Met à jour le Dashboard série (affichage ANSI).
 * @param mode Mode actuel.
 * @param music Partition en cours.
 * @param instru Instrument utilisé.
 * @param volume Volume actuel.
 * @param progress Note actuelle.
 * @param playing État du lecteur.
 */
void display_music_status(uint8_t mode, Partition *music, instrument_t instru, uint8_t volume, uint8_t progress, uint8_t playing);

/**
 * @brief Point d'entrée principal de l'application.
 * * @details Cette fonction orchestre l'ensemble du système suivant un cycle de vie précis :
 * 1. **Initialisation matérielle** : Activation des horloges (GPIO, TIM, ADC),
 * configuration des interruptions (Boutons, Switches) et du Systick (1ms).
 * 2. **Initialisation logicielle** : Reset des variables d'état (volume, bpm, progress).
 * 3. **Boucle de contrôle (Super-loop)** :
 * - Lecture des entrées physiques (Potentiomètre, Switches).
 * - Résolution de la machine à états pour déterminer les paramètres audio (@ref note, @ref volume, @ref timbre).
 * - Rafraîchissement intelligent de l'affichage série (uniquement sur changement d'état).
 * - Génération du signal sonore via @ref jouer_note_style.
 * - Incrémentation du curseur de partition (@ref progress).
 * * @return int Code de sortie (théoriquement jamais atteint dans un système embarqué).
 */
int main() {
  /* --- Initialisation des périphériques (Horloges) --- */
  enable_GPIOB();
  enable_GPIOC();
  enable_TIM2();
  enable_ADC1();

  /* --- Configuration des Entrées/Sorties --- */
  init_IRQ_BP_CARTE();   /**< Bouton bleu de la carte Nucleo */
  init_IRQ_BP_SHIELD();  /**< Bouton poussoir du shield Arduino */
  init_switch();         /**< Configuration des DIP-switches de mode */

  /* --- Audio & Mesure --- */
  init_Buzzer_AF();      /**< Configuration du Buzzer en Alternate Function */
  init_TIM2_CH2(get_APB1TIMCLK());
  init_potentiometre();  /**< Initialisation de l'ADC1 pour le potentiomètre */

  /* --- Système & Communication --- */
  SysTick_init(1000);    /**< Base de temps de 1ms pour @ref count */
  serial_io_init();      /**< Initialisation de l'USART2 (Dashboard ANSI) */


  /* --- État Initial du Lecteur --- */
  update_pwm_volume(0);  /**< Silence au démarrage */
  progress = 0;

  // Initialisation des constantes par défaut
  volume = 50; bpm = 50; note = DO; pitch = 4; duree = NOIRE; quadrature = 4;

  uint8_t last_progress = -1;
  uint8_t last_playing = playing ^ 1;



  /* --- Boucle Infinie --- */
  while (1) {
      Partition *p = musics.tab[music_index];

      pot_mesure = mesure_potentiometre();
      mode = get_switch_status();

      /** * @section Mode_Logic_Section Machine à états des modes
       * @details La logique est hiérarchisée :
       * - **Mode MUSICIEN (@ref MUSICIEN)** : Le potentiomètre contrôle la note ou un modificateur en temps réel.
       * - **Mode TIMBRE (@ref TIMBRE_MODULATOR)** : Sélection forcée de l'instrument sur la lecture auto.
       * - **Modes Modulateurs** : Ajustement de la partition par l'utilisateur.
       */
      if (mode & MUSICIEN) {
          if ((mode & ~0x8) == VOLUME_MODULATOR) {
              volume = convert_2_volume(pot_mesure);
          } else if ((mode & ~0x8) == BPM_MODULATOR) {
              bpm = convert_2_bpm(pot_mesure);
          } else if ((mode & ~0x8) == PITCH_MODULATOR) {
              pitch = (convert_2_pitch(pot_mesure) - 1) + 3;
          } else if (mode & TIMBRE_MODULATOR) {
              timbre = mode & ~0xc;
          } else {
              note = convert_2_note(pot_mesure);
          }
          duree = NOIRE;
          quadrature = 4;

      } else if (mode & TIMBRE_MODULATOR) {
          timbre = mode & ~0xC;
          bpm = p->bpm;
          pitch = p->notes[progress].octave;
          note = p->notes[progress].note;
          duree = p->notes[progress].valeur;
          quadrature = p->denom_beat;

      } else {
          // Gestion des modes modulateurs simples ou mode NORMAL
          if (mode == VOLUME_MODULATOR) {
              volume = convert_2_volume(pot_mesure);
              note = p->notes[progress].note;
              duree = p->notes[progress].valeur;
              quadrature = p->denom_beat;
          } else if (mode == BPM_MODULATOR) {
              bpm = convert_2_bpm(pot_mesure);
              note = p->notes[progress].note;
              duree = p->notes[progress].valeur;
              quadrature = p->denom_beat;
          } else if (mode == PITCH_MODULATOR) {
              note = p->notes[progress].note;
              pitch = convert_2_pitch(pot_mesure) - 1 + p->notes[progress].octave;
              duree = p->notes[progress].valeur;
              quadrature = p->denom_beat;
          } else {
              bpm = p->bpm;
              pitch = p->notes[progress].octave;
              note = p->notes[progress].note;
              duree = p->notes[progress].valeur;
              quadrature = p->denom_beat;
          }
      }

      /* --- Gestion de l'affichage série --- */
      if (playing) {
          if (progress != last_progress) {
              display_music_status(mode, p, timbre, volume, progress, 1);
              last_progress = progress;
              last_playing = 1;
          }
      } else {
          if (last_playing != 0) {
              display_music_status(mode, p, timbre, volume, progress, 0);
              last_playing = 0;
              last_progress = 0xff;
          }
      }

      /* --- Rendu Audio --- */
      jouer_note_style(note, pitch, get_duration_ms(duree, bpm, quadrature), volume, timbre);

      /** @note Petit délai de garde pour détacher les notes entre elles (Articulations) */
      delay_BC(20);

      /* --- Mise à jour du curseur de lecture --- */
      progress = (progress < p->size - 1) ? progress + 1 : 0;
  }

  return 0;
}

/**
 * @brief Convertit la valeur brute de l'ADC en pourcentage de volume.
 * @param val_pot Valeur 12-bits (0-4095).
 * @return uint8_t Volume mappé de 0 à 100%.
 */
uint8_t convert_2_volume(uint32_t val_pot){
	return (val_pot * 100) >> 12;
}

/**
 * @brief Calcule le tempo (BPM) avec une réponse hybride.
 * @details La courbe est divisée en deux phases :
 * 1. **Phase Linéaire** (0 à 2048) : de 50 à 200 BPM pour un réglage précis.
 * 2. **Phase Exponentielle** (2048 à 4095) : montée rapide jusqu'à 2500 BPM (Speedcore).
 * @param val_pot Valeur brute de l'ADC.
 * @return uint16_t Tempo en Battements Par Minute.
 */
uint16_t convert_2_bpm(uint32_t val_pot) {
    const uint32_t MID = 2048;
    const uint16_t BPM_MIN = 50;
    const uint16_t BPM_MID = 200;
    const uint16_t BPM_MAX = 2500;

    if (val_pot <= MID) {
        return BPM_MIN + ((val_pot * 150) >> 11);
    } else {
        uint32_t excess = val_pot - MID;
        uint32_t curve = (excess * excess) >> 10;
        uint32_t result = BPM_MID + curve;
        return (result > BPM_MAX) ? BPM_MAX : (uint16_t)result;
    }
}

/**
 * @brief Mappe le potentiomètre sur 4 octaves.
 * @param val_pot Valeur ADC 12-bits.
 * @return uint8_t Index d'octave (0-3).
 */
uint8_t convert_2_pitch(uint32_t val_pot){
	return (val_pot << 2) >> 12;
}

/**
 * @brief Convertit la valeur ADC en index de note (Gamme chromatique).
 * @param val_pot Valeur ADC 12-bits.
 * @return uint8_t Index de la note (0-11).
 */
uint8_t convert_2_note(uint32_t val_pot){
	return (val_pot*12) >> 12;
}

/**
 * @brief Traduit le mode binaire en libellé textuel pour l'affichage.
 * @param mode Registre de mode actuel.
 * @return const char* Nom du mode formaté.
 */
const char* get_mode_name(uint8_t mode) {
    if (mode & MUSICIEN) {
        switch (mode & 0x07) {
            case VOLUME_MODULATOR: return "MUSICIEN-VOLUME";
            case BPM_MODULATOR:    return "MUSICIEN-BPM";
            case PITCH_MODULATOR:  return "MUSICIEN-OCTAVE";
            default:               return "MUSICIEN";
        }
    }
    switch (mode & 0x07) {
        case VOLUME_MODULATOR: return "VOLUME";
        case BPM_MODULATOR:    return "BPM";
        case PITCH_MODULATOR:  return "OCTAVE";
        default:               return "NORMAL";
    }
}

/**
 * @brief Affiche le tableau de bord interactif sur la liaison série.
 * @details Utilise des séquences d'échappement ANSI pour le contrôle du curseur et les couleurs :
 * - `\033[H` : Home (curseur en haut à gauche).
 * - `\033[J` : Clear (efface le reste de l'écran).
 * - `\033[1;32m` : Vert gras (Statut PLAY).
 * - `\033[1;31m` : Rouge gras (Statut STOP).
 * * @param mode     Mode de fonctionnement actuel.
 * @param music    Pointeur vers la @ref Partition active.
 * @param instru   Instrument courant.
 * @param volume   Volume de sortie.
 * @param progress Index de la note dans la partition.
 * @param playing  Drapeau de lecture (1=Play, 0=Pause).
 */
void display_music_status(uint8_t mode, Partition *music, instrument_t instru, uint8_t volume, uint8_t progress, uint8_t playing){
    printf("\033[H"); // Home
    printf("==========================================\r\n");
    printf("        MAESTRO STM32 DASHBOARD           \r\n");
    printf("==========================================\r\n");

    printf("\033[J");
    if (playing) {
    	const char * mode_name = get_mode_name(mode);
        printf(" STATUS\t  : \033[1;32m\342\226\266 PLAYING\033[0m\r\n");
        printf(" MODE\t  : \033[1;31m%-16s\033[0m\r\n", mode_name);
        printf("------------------------------------------\r\n");

        // --- TITRE DE LA MUSIQUE (Couleur principale sur le texte) ---
        if (strstr(mode_name, "MUSIC") == NULL) {
            printf(" SONG\t  : \033[1;35m%s\033[0m\r\n", music->name);
        }else {
            printf(" SONG\t  : \033[1;35mJEU LIBRE\033[0m\r\n");
        }

        // --- GESTION NOTE / OCTAVE ---
        Note n = music->notes[progress];
        // Note en CYAN, mais l'Octave : parenthèses normales et chiffre en BLEU
        printf(" NOTE\t  : \033[1;36m%-3s\033[0m (Octave \033[1;36m%d\033[0m)\r\n", get_note_name(n.note), n.octave);

        // --- AFFICHAGE RYTHME ---
        printf(" RYTHME\t  : \033[0;32m%s\033[0m\r\n", get_duration_name(n.valeur));

        // --- PARAMÈTRES AUDIO ---
        // Le BPM et le symbole % en couleur
        printf(" TEMPO\t  : \033[0;37m%4d\033[0m BPM\r\n", music->bpm);
        printf(" VOLUME\t  : \033[0;37m%3d\033[0m %%\r\n", volume);

        // --- INSTRUMENT JOUE ---
        printf(" INSTR.\t  : \033[1;33m%s\033[0m\r\n", get_instrument_name(instru));

        // --- LOGIQUE ADAPTIVE ---
        if (strstr(mode_name, "MUSIC") == NULL) {
            printf(" PROGRESS : [%2d / %2d]\r\n", progress + 1, music->size);
        }

    } else {
        printf(" STATUS\t  : \033[1;31m\342\226\252 STOPPED / PAUSED\033[0m\r\n");
    }

    printf("==========================================\r\n");
    fflush(stdout);
}

/**
 * @brief Handler de l'horloge système (SysTick).
 * @details Incrémente la variable globale @ref count toutes les millisecondes.
 * Sert de base de temps pour les délais non-bloquants.
 */
void SysTick_Handler(){
	count++;
}

/**
 * @brief Handler EXTI pour le bouton du Shield (Broche 8).
 * @details Déclenche le passage au morceau suivant dans la bibliothèque @ref musics.
 * Réinitialise également le curseur de progression à 0.
 */
void EXTI9_5_Handler(){
	if (EXTI.PR & (1<<8)){
		music_index++;
		if (music_index >= musics.size) music_index = 0;
		progress=0;
		EXTI.PR = 1<<8;
	}
}

/**
 * @brief Handler EXTI pour le bouton bleu de la carte Nucleo (Broche 13).
 * @details Bascule l'état de lecture (@ref playing) entre Play et Pause.
 * Réinitialise la progression si nécessaire pour un redémarrage propre.
 */
void EXTI15_10_Handler(){
	if (EXTI.PR & (1<<13)){
		progress=0;
		playing^=1;
		EXTI.PR |= 1<<13;
	}
}
