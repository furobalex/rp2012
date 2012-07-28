/* 
   Code gerant la fourche du robot
   Auteurs : Jeremy et Aurélien
   Derniere modif : 22/07/2012
*/

#include <arduino.h> // Pour la fonction micros()

#include "AFMotor.h" // bibliotheque pour le moteur pas à pas
#include "fourche.h"
#include "constantes.h"

static int x = 1;
static double hauteurFourche = H_MIN;
unsigned long tpsPrec = 0;


// monte la fourche
void monterFourche ()
{
  if ((micros() - tpsPrec) > TEMPS_PAS)
  {
        if (hauteurFourche < H_MAX)
        {   
            //actualisation de la hauteur
            hauteurFourche ++;
            x = 1;
            // verifier le calcul
        }
        else
        {
           x = 2;
        }
  }
}

//baisse la fourche
int baisserFourche (int h)
{
     if (hauteurFourche > h)
    {
        //actualisation de la hauteur
        hauteurFourche --;
        x = 0;
        // verifier le calcul
    }
    
    return hauteurFourche; 
}
