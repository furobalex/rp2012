/* 
   Fonctions gérant l'ouverture et la fermeture du robot principal
   Auteurs : Jeremy et Aurélien
   Derniere modif : 22/07/2012
*/

#include <arduino.h> // pour la fonction micros()
#include <Servo.h>

#include "porte.h"
#include "constantes.h"

static unsigned long tpsPrec = 0;

void ouvrirPorte(int pos)//la fonction qui ouvre les portes.
{
    pos = POS_FIN;
}

void fermerPorte(int pos)//la fonction qui ferme les portes.
{
   pos = POS_INIT;
}

bool fini(unsigned long tpsPrec)
{
    if (micros() - tpsPrec < DELAI)
    {
        return false;
    }
    
    else
    {  
        return true;
    }
}
