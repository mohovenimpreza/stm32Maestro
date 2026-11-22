/**
 * @file music.c
 * @brief Définition de la bibliothèque musicale et des partitions.
 * @author Mohamed Hashem
 * @date 13 fév. 2026
 * * @details Ce fichier contient les tableaux de structures 'Note' représentant
 * les morceaux. Chaque partition est encapsulée dans une structure 'Partition'
 * incluant les métadonnées de lecture (BPM, signature rythmique).
 */

#include "music.h"

/* ==========================================================================
 * PARTITIONS (DATA)
 * ========================================================================== */

/**
 * @brief Partition : Au Clair de la Lune.
 * @details Mélodie simple en 4/4 utilisée pour le test initial du système.
 */
static Note notes_acl[] = {
    // Phrase 1
    {DO,4,NOIRE}, {DO,4,NOIRE}, {DO,4,NOIRE}, {RE,4,NOIRE},
    {MI,4,BLANCHE},{RE,4,BLANCHE},
    {DO,4,NOIRE}, {MI,4,NOIRE}, {RE,4,NOIRE}, {RE,4,NOIRE},
    {DO,4,BLANCHE},{SILENCE,0,BLANCHE},
    // Phrase 2 (identique)
    {DO,4,NOIRE}, {DO,4,NOIRE}, {DO,4,NOIRE}, {RE,4,NOIRE},
    {MI,4,BLANCHE},{RE,4,BLANCHE},
    {DO,4,NOIRE}, {MI,4,NOIRE}, {RE,4,NOIRE}, {RE,4,NOIRE},
    {DO,4,BLANCHE},{SILENCE,0,BLANCHE},
    // Phrase 3
    {RE,4,NOIRE}, {RE,4,NOIRE}, {RE,4,NOIRE}, {RE,4,NOIRE},
    {LA,3,BLANCHE}, {LA,3,BLANCHE},
    {RE,4,NOIRE}, {DO,4,NOIRE}, {SI,3,NOIRE}, {LA,3,NOIRE},
    {SOL,3,RONDE},
    // Phrase finale
    {DO,4,NOIRE}, {DO,4,NOIRE}, {DO,4,NOIRE}, {RE,4,NOIRE},
    {MI,4,BLANCHE},{RE,4,BLANCHE},
    {DO,4,NOIRE}, {MI,4,NOIRE}, {RE,4,NOIRE}, {RE,4,NOIRE},
    {DO,4,BLANCHE}, {SILENCE,0,BLANCHE},
};

Partition au_clair_de_la_lune = {
    .size = sizeof(notes_acl)/sizeof(Note),
    .bpm = 120,
    .num_beat = 4.0,
    .denom_beat = 4.0,
    .notes = notes_acl,
    .name = "Au Clair de la Lune"
};

/**
 * @brief Partition : Berceuse de Brahms.
 * @note Signature : 3/4 (Valse).
 * @details Idéal pour tester l'enveloppe INSTR_PIANO avec un tempo lent.
 */
static Note berceuse_brahms[] = {
    {SILENCE, 0, BLANCHE}, {MI, 4, CROCHE}, {MI, 4, CROCHE},
    {SOL, 4, NOIRE_POINTE}, {MI, 4, CROCHE}, {MI, 4, NOIRE},
    {SOL, 4, BLANCHE}, {MI, 4, CROCHE}, {SOL, 4, CROCHE},
    {DO, 5, NOIRE}, {SI, 4, NOIRE_POINTE}, {LA, 4, CROCHE},
    {LA,4,NOIRE}, {SOL,4,NOIRE}, {RE,4,CROCHE}, {MI,4,CROCHE},
    {FA,4,NOIRE}, {RE,4,NOIRE}, {RE,4,CROCHE}, {MI,4,CROCHE},
    {FA,4,BLANCHE}, {RE,4,CROCHE}, {FA,4,CROCHE},
    {SI,4,CROCHE}, {LA,4,CROCHE}, {SOL,4,NOIRE}, {SI,4,NOIRE},
    {DO,5,BLANCHE}, {DO,4,CROCHE}, {DO,4,CROCHE},
    {DO,5,BLANCHE}, {LA,4,CROCHE}, {FA,4,CROCHE},
    {SOL,4,BLANCHE}, {MI,4,CROCHE}, {DO,4,CROCHE},
    {FA,4,NOIRE},{SOL,4,NOIRE},{LA,4,NOIRE},
    {SOL,4,BLANCHE}, {DO,4,CROCHE}, {DO,4,CROCHE},
    {DO,5,BLANCHE}, {LA,4,CROCHE}, {FA,4,CROCHE},
    {SOL,4,BLANCHE}, {MI,4,CROCHE}, {DO,4,CROCHE},
    {FA,4,NOIRE}, {MI,4,NOIRE}, {RE,4,NOIRE},
    {DO,4,BLANCHE}
};

Partition brahms_berceuse = {
    .size = sizeof(berceuse_brahms)/sizeof(Note),
    .bpm = 80,
    .num_beat = 3.0,
    .denom_beat = 4.0,
    .notes = berceuse_brahms,
    .name = "La Berceuse de Brahms"
};

/**
 * @brief Partition : Thème de Super Mario Bros.
 * @details Rythme syncopé rapide. Demande une gestion précise des silences courts.
 */
static Note mario_theme[] = {
    // Jingle d'intro
    {MI, 4, CROCHE}, {MI, 4, CROCHE}, {SILENCE, 0, CROCHE}, {MI, 4, CROCHE},
    {SILENCE, 0, CROCHE}, {DO, 4, CROCHE}, {MI, 4, NOIRE},
    {SOL, 4, NOIRE}, {SILENCE, 0, NOIRE}, {SOL, 3, NOIRE}, {SILENCE, 0, NOIRE},
    // Thème principal
    {DO, 4, NOIRE}, {SILENCE, 0, CROCHE}, {SOL, 3, CROCHE}, {SILENCE, 0, CROCHE}, {MI, 3, NOIRE},
    {LA, 3, NOIRE}, {SI, 3, NOIRE}, {LA_DIESE, 3, CROCHE}, {LA, 3, NOIRE},
    {SOL, 3, CROCHE}, {MI, 4, CROCHE}, {SOL, 4, CROCHE}, {LA, 4, NOIRE}, {FA, 4, CROCHE}, {SOL, 4, CROCHE},
    {SILENCE, 0, CROCHE}, {MI, 4, NOIRE}, {DO, 4, CROCHE}, {RE, 4, CROCHE}, {SI, 3, NOIRE},
    // Section 2
    {SILENCE, 0, NOIRE}, {SOL, 4, CROCHE}, {FA_DIESE, 4, CROCHE}, {FA, 4, CROCHE}, {RE_DIESE, 4, NOIRE}, {MI, 4, CROCHE},
    {SILENCE, 0, CROCHE}, {SOL_DIESE, 3, CROCHE}, {LA, 3, CROCHE}, {DO, 4, CROCHE}, {SILENCE, 0, CROCHE}, {LA, 3, CROCHE}, {DO, 4, CROCHE}, {RE, 4, CROCHE},
    {SILENCE, 0, NOIRE}, {SOL, 4, CROCHE}, {FA_DIESE, 4, CROCHE}, {FA, 4, CROCHE}, {RE_DIESE, 4, NOIRE}, {MI, 4, CROCHE},
    {SILENCE, 0, CROCHE}, {DO, 5, CROCHE}, {SILENCE, 0, CROCHE}, {DO, 5, CROCHE}, {DO, 5, NOIRE}, {SILENCE, 0, NOIRE},
    // Section 3
    {SILENCE, 0, NOIRE}, {SOL, 4, CROCHE}, {FA_DIESE, 4, CROCHE}, {FA, 4, CROCHE}, {RE_DIESE, 4, NOIRE}, {MI, 4, CROCHE},
    {SILENCE, 0, CROCHE}, {SOL_DIESE, 3, CROCHE}, {LA, 3, CROCHE}, {DO, 4, CROCHE}, {SILENCE, 0, CROCHE}, {LA, 3, CROCHE}, {DO, 4, CROCHE}, {RE, 4, CROCHE},
    {SILENCE, 0, NOIRE}, {RE_DIESE, 4, NOIRE}, {SILENCE, 0, CROCHE}, {RE, 4, NOIRE},
    {DO, 4, BLANCHE}, {SILENCE, 0, BLANCHE},
};

Partition mario = {
    .size = sizeof(mario_theme)/sizeof(Note),
    .bpm = 160,
    .num_beat = 4.0,
    .denom_beat = 4.0,
    .notes = mario_theme,
    .name = "Mario"
};

/**
 * @brief Partition : Megalovania (Sans Theme).
 * @details Tempo très élevé (180 BPM) avec un riff rapide en RE mineur.
 */
static Note megalovania_theme[] = {
    {RE, 4, CROCHE}, {RE, 4, CROCHE}, {RE, 5, NOIRE}, {LA, 4, NOIRE_POINTE},
    {SOL_DIESE, 4, NOIRE}, {SOL, 4, NOIRE}, {FA, 4, NOIRE},
    {RE, 4, CROCHE}, {FA, 4, CROCHE}, {SOL, 4, CROCHE},
    {DO, 4, CROCHE}, {DO, 4, CROCHE}, {RE, 5, NOIRE}, {LA, 4, NOIRE_POINTE},
    {SOL_DIESE, 4, NOIRE}, {SOL, 4, NOIRE}, {FA, 4, NOIRE},
    {RE, 4, CROCHE}, {FA, 4, CROCHE}, {SOL, 4, CROCHE},
    {SI, 3, CROCHE}, {SI, 3, CROCHE}, {RE, 5, NOIRE}, {LA, 4, NOIRE_POINTE},
    {SOL_DIESE, 4, NOIRE}, {SOL, 4, NOIRE}, {FA, 4, NOIRE},
    {RE, 4, CROCHE}, {FA, 4, CROCHE}, {SOL, 4, CROCHE},
    {LA_DIESE, 3, CROCHE}, {LA_DIESE, 3, CROCHE}, {RE, 5, NOIRE}, {LA, 4, NOIRE_POINTE},
    {SOL_DIESE, 4, NOIRE}, {SOL, 4, NOIRE}, {FA, 4, NOIRE},
    {RE, 4, CROCHE}, {FA, 4, CROCHE}, {SOL, 4, CROCHE},
    {FA, 4, CROCHE}, {FA, 4, CROCHE}, {FA, 4, NOIRE}, {FA, 4, NOIRE},
    {FA, 4, CROCHE}, {FA, 4, CROCHE}, {RE, 4, NOIRE}, {RE, 4, NOIRE},
    {FA, 4, CROCHE}, {FA, 4, CROCHE}, {FA, 4, NOIRE}, {SOL, 4, NOIRE},
    {SOL_DIESE, 4, NOIRE}, {SOL, 4, CROCHE}, {FA, 4, CROCHE}, {RE, 4, NOIRE},
    {FA, 4, CROCHE}, {SOL, 4, CROCHE}, {SOL_DIESE, 4, NOIRE}, {LA, 4, NOIRE},
    {DO, 5, NOIRE}, {LA, 4, NOIRE}, {RE, 5, BLANCHE},
    {RE, 5, CROCHE}, {RE, 5, CROCHE}, {RE, 5, CROCHE}, {LA, 4, NOIRE}, {RE, 5, NOIRE},
    {DO, 5, BLANCHE}, {SILENCE, 0, NOIRE}
};

Partition megalovania = {
    .size = sizeof(megalovania_theme)/sizeof(Note),
    .bpm = 180,
    .num_beat = 4.0,
    .denom_beat = 4.0,
    .notes = megalovania_theme,
    .name = "Megalovania"
};

/**
 * @brief Partition : Marche Impériale (Star Wars).
 * @note Utilise le rythme "pointé" pour l'aspect martial.
 */
static Note imperial_march_theme[] = {
    {LA, 3, NOIRE}, {LA, 3, NOIRE}, {LA, 3, NOIRE},
    {FA, 3, CROCHE_POINTE}, {DO, 4, DOUBLE_CROCHE},
    {LA, 3, NOIRE}, {FA, 3, CROCHE_POINTE}, {DO, 4, DOUBLE_CROCHE}, {LA, 3, BLANCHE},
    {MI, 4, NOIRE}, {MI, 4, NOIRE}, {MI, 4, NOIRE},
    {FA, 4, CROCHE_POINTE}, {DO, 4, DOUBLE_CROCHE},
    {LA_DIESE, 3, NOIRE}, {FA, 3, CROCHE_POINTE}, {DO, 4, DOUBLE_CROCHE}, {LA, 3, BLANCHE},
    {LA, 4, NOIRE}, {LA, 3, CROCHE}, {LA, 3, CROCHE}, {LA, 4, NOIRE},
    {SOL_DIESE, 4, CROCHE_POINTE}, {SOL, 4, DOUBLE_CROCHE},
    {FA_DIESE, 4, DOUBLE_CROCHE}, {FA, 4, DOUBLE_CROCHE}, {FA_DIESE, 4, CROCHE},
    {SILENCE, 0, CROCHE}, {LA_DIESE, 3, CROCHE}, {RE_DIESE, 4, NOIRE},
    {RE, 4, CROCHE_POINTE}, {DO_DIESE, 4, DOUBLE_CROCHE},
    {DO, 4, DOUBLE_CROCHE}, {SI, 3, DOUBLE_CROCHE}, {DO, 4, CROCHE},
    {SILENCE, 0, CROCHE}, {FA, 3, CROCHE}, {SOL_DIESE, 3, NOIRE},
    {FA, 3, CROCHE_POINTE}, {LA, 3, DOUBLE_CROCHE},
    {DO, 4, NOIRE}, {LA, 3, CROCHE_POINTE}, {DO, 4, DOUBLE_CROCHE}, {MI, 4, BLANCHE},
};

Partition imperial_march = {
    .size = sizeof(imperial_march_theme)/sizeof(Note),
    .bpm = 100,
    .num_beat = 4.0,
    .denom_beat = 4.0,
    .notes = imperial_march_theme,
    .name = "La Marche Imperiale - Star Wars"
};

/**
 * @brief Partition : Thème de Tetris.
 * @details Mélodie folklorique russe Korobeiniki. Rythme 4/4 vif.
 */
static Note tetris_theme[] = {
    {MI, 5, NOIRE}, {SI, 4, CROCHE}, {DO, 5, CROCHE}, {RE, 5, NOIRE}, {DO, 5, CROCHE}, {SI, 4, CROCHE},
    {LA, 4, NOIRE}, {LA, 4, CROCHE}, {DO, 5, CROCHE}, {MI, 5, NOIRE}, {RE, 5, CROCHE}, {DO, 5, CROCHE},
    {SI, 4, NOIRE_POINTE}, {DO, 5, CROCHE}, {RE, 5, NOIRE}, {MI, 5, NOIRE},
    {DO, 5, NOIRE}, {LA, 4, NOIRE}, {LA, 4, NOIRE}, {SILENCE, 0, NOIRE},
    {RE, 5, NOIRE_POINTE}, {FA, 5, CROCHE}, {LA, 5, NOIRE}, {SOL, 5, CROCHE}, {FA, 5, CROCHE},
    {MI, 5, NOIRE_POINTE}, {DO, 5, CROCHE}, {MI, 5, NOIRE}, {RE, 5, CROCHE}, {DO, 5, CROCHE},
    {SI, 4, NOIRE}, {SI, 4, CROCHE}, {DO, 5, CROCHE}, {RE, 5, NOIRE}, {MI, 5, NOIRE},
    {DO, 5, NOIRE}, {LA, 4, NOIRE}, {LA, 4, NOIRE}, {SILENCE, 0, NOIRE},
    {MI, 4, BLANCHE}, {DO, 4, BLANCHE},
    {RE, 4, BLANCHE}, {SI, 3, BLANCHE},
    {DO, 4, BLANCHE}, {LA, 3, BLANCHE},
    {SOL_DIESE, 3, BLANCHE_POINTE}, {SILENCE, 0, NOIRE},
    {MI, 4, BLANCHE}, {DO, 4, BLANCHE},
    {RE, 4, BLANCHE}, {SI, 3, BLANCHE},
    {MI, 4, NOIRE}, {DO, 5, NOIRE}, {LA, 4, NOIRE},
    {SOL_DIESE, 4, BLANCHE_POINTE}, {SILENCE, 0, NOIRE},
    {MI, 5, BLANCHE}, {DO, 5, BLANCHE},
    {RE, 5, BLANCHE}, {SI, 4, BLANCHE},
    {DO, 5, NOIRE}, {RE, 5, NOIRE}, {MI, 5, NOIRE}, {DO, 5, NOIRE},
    {SI, 4, NOIRE}, {LA, 4, NOIRE}, {LA, 4, BLANCHE}, {SILENCE, 0, NOIRE}
};

Partition tetris = {
    .size = sizeof(tetris_theme)/sizeof(Note),
    .bpm = 150,
    .num_beat = 4.0,
    .denom_beat = 4.0,
    .notes = tetris_theme,
    .name = "Tetris"
};

/**
 * @brief Partition : Pirates des Caraïbes.
 * @note Signature : 3/4.
 * @details Mélodie épique et galopante.
 */
static Note pirates_theme[] = {
    {LA, 3, CROCHE}, {DO, 4, CROCHE}, {RE, 4, NOIRE}, {RE, 4, NOIRE},
    {RE, 4, CROCHE}, {MI, 4, CROCHE}, {FA, 4, NOIRE}, {FA, 4, NOIRE},
    {FA, 4, CROCHE}, {SOL, 4, CROCHE}, {MI, 4, NOIRE}, {MI, 4, NOIRE},
    {RE, 4, CROCHE}, {DO, 4, CROCHE}, {DO, 4, CROCHE}, {RE, 4, NOIRE_POINTE},
    {LA, 3, CROCHE}, {DO, 4, CROCHE}, {RE, 4, NOIRE}, {RE, 4, NOIRE},
    {RE, 4, CROCHE}, {MI, 4, CROCHE}, {FA, 4, NOIRE}, {FA, 4, NOIRE},
    {FA, 4, CROCHE}, {SOL, 4, CROCHE}, {MI, 4, NOIRE}, {MI, 4, NOIRE},
    {RE, 4, CROCHE}, {DO, 4, CROCHE}, {RE, 4, NOIRE}, {SILENCE, 0, NOIRE},
    {LA, 3, CROCHE}, {DO, 4, CROCHE}, {RE, 4, NOIRE}, {RE, 4, CROCHE}, {RE, 4, CROCHE},
    {RE, 4, CROCHE}, {FA, 4, CROCHE}, {SOL, 4, NOIRE}, {SOL, 4, CROCHE}, {SOL, 4, CROCHE},
    {SOL, 4, CROCHE}, {LA, 4, CROCHE}, {SI_BEMOL, 4, NOIRE}, {SI_BEMOL, 4, CROCHE}, {LA, 4, CROCHE},
    {SOL, 4, CROCHE}, {LA, 4, CROCHE}, {RE, 4, NOIRE}, {SILENCE, 0, CROCHE}, {RE, 4, CROCHE},
    {RE, 4, CROCHE}, {MI, 4, CROCHE}, {FA, 4, NOIRE}, {FA, 4, NOIRE},
    {SOL, 4, NOIRE}, {LA, 4, NOIRE}, {RE, 4, NOIRE}, {SILENCE, 0, CROCHE}, {RE, 4, CROCHE},
    {FA, 4, CROCHE}, {MI, 4, CROCHE}, {MI, 4, CROCHE}, {RE, 4, NOIRE}, {DO, 4, CROCHE},
    {RE, 4, BLANCHE_POINTE}, {SILENCE, 0, BLANCHE}
};

Partition pirates = {
    .size = sizeof(pirates_theme)/sizeof(Note),
    .bpm = 145,
    .num_beat = 3.0,
    .denom_beat = 4.0,
    .notes = pirates_theme,
    .name = "Pirates des Caraibes"
};

/**
 * @brief Partition : Harry Potter (Hedwig's Theme).
 * @note Signature : 3/4.
 * @details Utilise beaucoup d'altérations pour l'ambiance mystérieuse.
 */
static Note harry_potter_theme[] = {
    {SI, 3, NOIRE},
    {MI, 4, NOIRE_POINTE}, {SOL, 4, CROCHE}, {FA_DIESE, 4, NOIRE},
    {MI, 4, BLANCHE}, {SI, 4, NOIRE},
    {LA, 4, BLANCHE_POINTE},
    {FA_DIESE, 4, BLANCHE_POINTE},
    {MI, 4, NOIRE_POINTE}, {SOL, 4, CROCHE}, {FA_DIESE, 4, NOIRE},
    {RE_DIESE, 4, BLANCHE}, {FA, 4, NOIRE},
    {SI, 3, BLANCHE_POINTE}, {SILENCE, 0, NOIRE},
    {SI, 3, NOIRE},
    {MI, 4, NOIRE_POINTE}, {SOL, 4, CROCHE}, {FA_DIESE, 4, NOIRE},
    {MI, 4, BLANCHE}, {SI, 4, NOIRE},
    {RE, 5, BLANCHE}, {DO_DIESE, 5, NOIRE},
    {DO, 5, BLANCHE}, {SOL_DIESE, 4, NOIRE},
    {DO, 5, NOIRE_POINTE}, {SI, 4, CROCHE}, {LA_DIESE, 4, NOIRE},
    {SI, 3, NOIRE}, {SOL, 4, NOIRE},
    {MI, 4, BLANCHE_POINTE}, {SILENCE, 0, BLANCHE}
};

Partition harry_potter = {
    .size = sizeof(harry_potter_theme)/sizeof(Note),
    .bpm = 130,
    .num_beat = 3.0,
    .denom_beat = 4.0,
    .notes = harry_potter_theme,
    .name = "Harry Potter"
};

/**
 * @brief Partition : Smoke on the Water.
 * @details Le riff rock par excellence.
 */
static Note riff_smoke[] = {
    {SOL, 3, NOIRE}, {SI_BEMOL, 3, NOIRE}, {DO, 4, NOIRE_POINTE},
    {SOL, 3, NOIRE}, {SI_BEMOL, 3, NOIRE}, {RE_BEMOL, 4, CROCHE}, {DO, 4, BLANCHE},
    {SOL, 3, NOIRE}, {SI_BEMOL, 3, NOIRE}, {DO, 4, NOIRE_POINTE},
    {SI_BEMOL, 3, NOIRE}, {SOL, 3, BLANCHE}
};

Partition smoke_on_the_water = {
    .name = "Smoke On Water",
    .bpm = 112,
    .size = 12,
    .notes = riff_smoke,
    .denom_beat = 4.0,
    .num_beat = 4.0,
};

/* ==========================================================================
 * GESTIONNAIRE DE PLAYLIST
 * ========================================================================== */



/**
 * @brief Tableau de toutes les partitions disponibles.
 */
Partition *tab_music[] = {
    &au_clair_de_la_lune,
    &brahms_berceuse,
    &mario,
    &tetris,
    &megalovania,
    &imperial_march,
    &pirates,
    &harry_potter,
    &smoke_on_the_water
};

/**
 * @brief Objet global regroupant la liste des musiques et sa taille.
 */
Tab_Music musics = {
    .size = sizeof(tab_music) / sizeof(Partition*),
    .tab = tab_music
};

