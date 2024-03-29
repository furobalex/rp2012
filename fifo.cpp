/*
 * fifo.c
 *
 *  Created on: 16 janv. 2011
 *	  Author: HoHen
 */

#include "fifo.h"
#include <math.h>
#include "include_arduino.h"



Fifo goals;

void initGoals(){
	goals.goal = (Goal*)malloc(sizeof(Goal)*SIZE);
	goals.in = 0;
	goals.out = 0;

        for(int i=0;i<10;i++){
          pushGoalPosition(NO_ID, (double)400 * (ENC_MM_TO_TICKS), (double)0 * (ENC_MM_TO_TICKS), (double)250);
          pushGoalPosition(NO_ID, (double)200 * (ENC_MM_TO_TICKS), (double)0 * (ENC_MM_TO_TICKS), (double)250);
        }/*
        for(int i=0;i<10;i++){
            pushGoalOrientation(NO_ID, 3.14/2, 100); //angle,vitesse
            pushGoalOrientation(NO_ID, 0, 100); //angle,vitesse
          }  
        //pushGoalOrientation(NO_ID, 3.14/2, 100); //angle,vitesse
        /*pushGoalPosition(NO_ID, (double)100 * (ENC_MM_TO_TICKS), (double)100 * (ENC_MM_TO_TICKS), (double)255);
        pushGoalPosition(NO_ID, (double)0 * (ENC_MM_TO_TICKS), (double)100 * (ENC_MM_TO_TICKS), (double)255);
        pushGoalPosition(NO_ID, (double)0 * (ENC_MM_TO_TICKS), (double)0 * (ENC_MM_TO_TICKS), (double)255);*/

	//pushGoalAutoCalibration(0,0);
	/*pour tester le robot*/
	//pushGoalOrientation(3.14,255); //angle,vitesse
	//pushGoalPosition(NO_ID, 10000, 9000, 255); //x,y,vitesse
	//pushGoalSpeed(NO_ID, 255, 10000); //vitesse,periode
	
	//pushGoalOrientation(3.14,255);
	/*pushGoalDelay(1000);
	pushGoalOrientation(3.14,255);
	pushGoalDelay(1000);
	pushGoalOrientation(3*3.14/2,255);
	pushGoalDelay(1000);
	pushGoalOrientation(0,255);*/
	//pushGoalOrientation(1.414,200);


}

// oriente le robot
void pushGoalOrientation(int id, double angle, double speed){
	if((goals.in+1)%SIZE != goals.out){
		Goal* incGoal = goals.goal+goals.in;
		incGoal->type = TYPE_ANGLE;
		incGoal->id = id;
		incGoal->data_1 = angle;
		incGoal->data_2 = speed;
		goals.in = (goals.in+1)%SIZE;
	}
}

// bouge le robot vers une position
void pushGoalPosition(int id, double x, double y, double speed){
	if((goals.in+1)%SIZE != goals.out){
		Goal* incGoal = goals.goal+goals.in;
		incGoal->type = TYPE_POSITION;
		incGoal->id = id;
		incGoal->data_1 = x;
		incGoal->data_2 = y;
		incGoal->data_3 = speed;
		goals.in = (goals.in+1)%SIZE;
	}
}

// fait foncer le robot en ligne droite
void pushGoalSpeed(int id, double speed, double period){
	if((goals.in+1)%SIZE != goals.out){
		Goal* incGoal = goals.goal+goals.in;
		incGoal->type = TYPE_SPEED;
		incGoal->id = id;
		incGoal->data_1 = speed;
		incGoal->data_2 = period;
		goals.in = (goals.in+1)%SIZE;
	}
}

// donne une valeur prédéfinie a chacun des moteurs
void pushGoalPwm(int id, double left, double right, double period){
	if((goals.in+1)%SIZE != goals.out){
		Goal* incGoal = goals.goal+goals.in;
		incGoal->type = TYPE_PWM;
		incGoal->id = id;
		incGoal->data_1 = left;
		incGoal->data_2 = right;
		incGoal->data_3 = period;
		goals.in = (goals.in+1)%SIZE;
	}
}

// enleve une position
void popGoal(){
	if(goals.in!=goals.out){
		current_goal.isReached = false;
		current_goal.isCanceled = false;
		current_goal.isMessageSent = false;
		current_goal.phase = PHASE_1;
		Goal* outGoal = goals.goal+goals.out;
		current_goal.type = outGoal->type;
		current_goal.id = outGoal->id;
		switch (outGoal->type) {
		case TYPE_SPEED:
			current_goal.speed = outGoal->data_1;
			current_goal.period = outGoal->data_2;
			break;
		case TYPE_ANGLE:
			current_goal.angle = outGoal->data_1;
			current_goal.speed = outGoal->data_2;
			break;
		case TYPE_POSITION:
			current_goal.x = outGoal->data_1;
			current_goal.y = outGoal->data_2;
			current_goal.speed = outGoal->data_3;
			break;
		case TYPE_PWM:
			current_goal.x = outGoal->data_1;
			current_goal.y = outGoal->data_2;
			current_goal.period = outGoal->data_3;
			break;
		default:
			break;
		}
		goals.out = (goals.out+1)%SIZE;
	}
}

// vide la file des objectifs
void clearGoals(){
	goals.in = 0;
	goals.out = 0;
}

bool fifoIsEmpty(){
	return goals.in==goals.out;
}

