/* Mainloop du robot principal 2012
   Auteur : Aurélien Duval
   Derniere modif : 27/07/2012
*/

#include <Servo.h>
#include <Stepper.h>

#include "constantes.h"
#include "porte.h"
#include "fourche.h"
#include "AFMotor.h"
#include "fifo.h"
#include "verre.h"
#include "robotstate.h"


bool tab[4] = {false, false, false, false};
AF_Stepper stepper(STEP, 2);
Servo servoLeft;
Servo servoRight;
int pos = 0;
int x;
unsigned long tpsPrecStepper = 0;
unsigned long tpsPrecServo = 0;
Objet verre[11];

void setup()
{
   bool redTeam = true;
   
   pinMode(PIN_SWITCH_TEAM, INPUT);
   digitalWrite(PIN_SWITCH_TEAM, HIGH);
   
   redTeam = setTeam();
   initGoals();
   initPosVerre(verre, redTeam);
   stepper.setSpeed(SPEED_STEPPER);
   servoLeft.attach(SERVOMOTOR_LEFT); 
   servoRight.attach(SERVOMOTOR_RIGHT);
   
   Serial.begin(9600);
}

void loop()
{
    int state = VERRE_0_OK;
    int hauteur = 0;
    Objet cibleActu;
    bool continuer = true;
    int i;
   
  while (continuer == true)
  {  
    switch (state)
    {
        case BEGIN: // recherche d'un verre plein en vue de le mettre en haut de la pile. S'il n'y en a plus, le robot ramassera un verre vide
           cibleActu = rechercheVerrePlein(verre);
           break;
           
        case LOCKED: // si un verre plein a été trouvé
          pushGoalPosition(NO_ID, cibleActu.x, cibleActu.y, SPEED); //rouler vers le verre plein
          if (robot_get_x () == cibleActu.x && robot_get_y () == cibleActu.y)
          {
            state = POS_OK;
          }
          break;
         
        case POS_OK: // si le robot est en place
          ouvrirPorte(pos);
          clearGoals();
          if (fini(tpsPrecServo) == true)
          {  
            state = PORTE_OUVERTE;
          }
          break;
          
        case PORTE_OUVERTE: // quand les portes sont ouvertes
          pushGoalPosition(NO_ID, cibleActu.x, cibleActu.y, SPEED);//avancer
          if (robot_get_x () == cibleActu.x && robot_get_y () == cibleActu.y)
          {
            state = POS_VERRE_OK;
          }
          break;
          
        case POS_VERRE_OK: // quand le verre est en place
          monterFourche();
          state = VERRE_0_OK;
          break;
          
        case VERRE_0_OK: // quand la fourche est en haut et que le verre est en place
          tab[0] = true;
          cibleActu = rechercheVerreVide(verre);
          clearGoals();
          if (cibleActu.x == 0 && cibleActu.y == 0 && cibleActu.type == EMPTY && cibleActu.isAlreadyHere == true)
          {
            state = GO_BASE; // on rentre a la base car il n'y a plus de verres
          }
          
          else 
          {
            state = VERRE_REPERE; // on a repéré un verre
          }
            break;
          
        case VERRE_REPERE: // si un verre est repéré
          pushGoalPosition(NO_ID, cibleActu.x, cibleActu.y, SPEED);
          if (robot_get_x () == cibleActu.x && robot_get_y () == cibleActu.y)
          {
            if (tab[1] == false)
            {
              state = VERRE_2_OK;
            }
            
            else if (tab[2] == false)
            {
              state = VERRE_3_OK;
            }
            
            else if (tab[3] == false)
            {
              state = GO_BASE;
            }
          }
          break;
          
        case VERRE_1_OK: // si le verre un est dans le robot
          tab[1] = true;
          fermerPorte(pos);
          clearGoals();
          state = checkVerre(tab);
          break;
          
        case VERRE_2_OK: // si le verre 2 est dans le robot
          tab[2] = true;
          fermerPorte(pos);
          clearGoals();
          state = checkVerre(tab);
          break;
          
        case VERRE_3_OK: // si le verre 3 est dans le robot
          tab[3] = true;
          fermerPorte(pos);
          state = checkVerre(tab);
          break;
          
        case VERRE_FULL: // si le robot est plein
          state = GO_BASE;
          break;
          
        case GO_BASE: // retour a la base
          pushGoalPosition(NO_ID, cibleActu.x, cibleActu.y, SPEED);
          if (robot_get_x () == cibleActu.x && robot_get_y () == cibleActu.y)
          {
            state = IN_BASE;
          }
          break;
          
        case IN_BASE: // quand le robot est dans la base
          ouvrirPorte(pos);
          hauteur = baisserFourche(H_INTER);
          if (fini (tpsPrecServo) == true && hauteur <= H_INTER)
          {
              pushGoalPosition(NO_ID, cibleActu.x, cibleActu.y, SPEED);
              fermerPorte(pos);
              if (robot_get_x () == cibleActu.x && robot_get_y () == cibleActu.y && fini (tpsPrecServo) == true)
              {
                state = BEGIN; // retour au début
                clearGoals();
                
                for (i = 0 ; verre[i].isAlreadyHere != true ; i++)
                {
                  if (verre[i].isAlreadyHere == true)
                  {
                      continuer = true;
                  }
                        
                  else 
                  {
                      continuer = false;
                  }
                }
              }
          }
          break;
          
    }
    
    update();
  }
}

// update le tout
void update()
{
   if ((micros() - tpsPrecStepper) > TEMPS_PAS)
    {
      tpsPrecStepper = micros();
      if (x == 1)
      {
         stepper.onestep(FORWARD, SINGLE); // fait tourner le moteur de [val-valPrecedente] pas 
      }
      
      else if (x == 0)
      {
         stepper.onestep(BACKWARD, SINGLE); // fait tourner le moteur de [val-valPrecedente] pas 
      }
    } 
    
    tpsPrecServo = micros();
    servoLeft.write(pos-120);
    servoRight.write(180-pos+90);
    
    Serial.println("pos servo : ");
    Serial.println(pos - 120);
    Serial.println(180-pos+90);
    Serial.println();
    Serial.println("position du bot : ");
    Serial.println(robot_get_x());
    Serial.println(robot_get_y());
    Serial.println();
    Serial.println("Orientation du bot : ");
    Serial.println(robot_get_angle());
}

// Initialise la position des verres pour le robot
void initPosVerre (Objet verre[11], bool redTeam)
{
  int i;
  
  int redX[11] = {1383, 789, 1135, 962, 789, 1135, 1825, 2171, 1998, 1825, 2171};
  int redY[11] = {716, 400, 400, 716, 1033, 1033, 400, 400, 716, 1033, 1033};

  int blueX[11] = {1383, 789, 1135, 962, 789, 1135, 1825, 2171, 1998, 1825, 2171};
  int blueY[11] = {1516, 1200, 1200, 1516, 1833, 1833, 1200, 1200, 1516, 1833, 1833};
  
  int type[11] = {FULL, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY};
  bool isAlreadyHere[11] = {true, true, true, true, true, true, true, true, true, true, true};
  
  if (redTeam == true)
  {
    for (i = 0 ; i < 11 ; i++)
    {
      verre[i].x = redX[i];
      verre[i].y = redY[i];
      verre[i].type = type[i];
      verre[i].isAlreadyHere = isAlreadyHere[i];
    }
  }
  
  else
  {
    for (i = 0 ; i < 11 ; i++)
    {
      verre[i].x = blueX[i];
      verre[i].y = blueY[i];
      verre[i].type = type[i];
      verre[i].isAlreadyHere = isAlreadyHere[i];
    }
  }
  
}

// Indique l'equipe dans laquelle on est
bool setTeam()
{
    if (digitalRead(PIN_SWITCH_TEAM) == HIGH)
    {
      return true;
    }
    
    else
    {
      return false;
    }
}
