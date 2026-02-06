#pragma once

struct Strings
{
    static inline const char* title = "Space invader";                      // titolo del gioco
    static inline const char* quit = "Quit";                                // bottone per uscire dalla partia
    static inline const char* restart = "Restart";                          // bottone per restart
    static inline const char* resume = "Resume";                            // bottone per start
    static inline const char* quitGame = "Quit the Game";                   // bottone per uscire dal gioco
    static inline const char* fullscreen = "Fullscreen: %s";                // bottone fullscreen %s può essere: ON/OFF

    static inline const char* ready = "Be ready!";                          // label che compare quando riprendi il gioco
    static inline const char* startSinglePlayer = "Start single player";    // bottone single player
    static inline const char* startMultiPlayer = "Start multi player";      // bottone multi player

    static inline const char* difficulty = "Difficulty: %s";                // bottone difficoltà, %s può essere: easy/normal/hard
    static inline const char* score = "Score: %d";                          // label del punteggio (la vedo difficile per questa)

    // stringhe della difficoltà di sopra
    static inline const char* easy = "Easy";
    static inline const char* normal = "Normal";
    static inline const char* hard = "Hard";

    // compare sopra al posto dei cuori quando sono finiti
    static inline const char* danger = "Danger!";

    static inline const char* audioSettings = "Audio";                      // bottone del menù di mid game (quando premi esc) dell'audio
    static inline const char* generalVolume = "General Volume";             // label per lo slider del volume generale
    static inline const char* musicVolume = "Music Volume";                 // ...
    static inline const char* laserVolume = "Lasers Volume";                // ...
    static inline const char* meteorVolume = "Meteors Volume";              // ...
    static inline const char* explosionVolume = "Explosions Volume";        // ...
    static inline const char* modifierVolume = "Modifiers Volume";          // ...

    static inline const char* control = "Controls";                         // bottone (sempre dell'esc) per i controlli
    static inline const char* moveUp = "UP: %s";                            // bottone che ti dice che bind c'è per il movimento verso l'alto
    static inline const char* moveDown = "DOWN: %s";                        // basso
    static inline const char* moveLeft = "LEFT: %s";                        // sinistra
    static inline const char* moveRight = "RIGHT: %s";                      // destra
    static inline const char* shoot = "SHOOT: %s";                          // sparo
    static inline const char* dash = "DASH: %s";                            // dash
    static inline const char* reset = "Reset bindings";                     // bottone per il reset dei bindings

    static inline const char* waitingKey = "Press a key...";                // il testo del bottone che compare quando devi premere una key per fare il bind
    static inline const char* notFound = "Res not found.";                  // debug, non ti serve
};
