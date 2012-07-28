#ifndef CONSTANTES

    #define CONSTANTES
    
    // ROBOT
    #define SPEED 100
    #define PIN_SWITCH_TEAM 1 // switch du pin 1
    
    // FOURCHE
    #define FOURCHE_PIN 2  //correspond aux pins 5 et 6
    
    #define STEP 200 /*NB*/   // Nombre de pas dans le moteur
    
    #define H_MAX 200 /*NB*/
    #define H_INTER 100
    #define H_MIN 0 /*NB*/
    #define SPEED_STEPPER 10
    
    #define TEMPS_PAS 10000
    
    // PORTE
    #define SERVOMOTOR_LEFT 9
    #define SERVOMOTOR_RIGHT 10
    
    #define POS_INIT 90
    #define POS_FIN  190
    
    #define DELAI 1000
    
    enum {BEGIN, LOCKED, POS_OK, PORTE_OUVERTE, POS_VERRE_OK, VERRE_0_OK, VERRE_REPERE, VERRE_1_OK, VERRE_2_OK, VERRE_3_OK, GO_BASE, IN_BASE, VERRE_FULL};
    enum {FULL, EMPTY};
    
#endif
