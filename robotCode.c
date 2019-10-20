#pragma config(Sensor, dgtl1,  centerIRout,    sensorDigitalOut)
#pragma config(Sensor, dgtl2,  centerIR,       sensorDigitalIn)
#pragma config(Sensor, dgtl3,  leftIRout,      sensorDigitalOut)
#pragma config(Sensor, dgtl4,  leftIR,         sensorDigitalIn)
#pragma config(Sensor, dgtl5,  rightIRout,     sensorDigitalOut)
#pragma config(Sensor, dgtl6,  rightIR,        sensorDigitalIn)
#pragma config(Sensor, dgtl7,  LimitSwitch,    sensorTouch)
#pragma config(Motor,  port2,           leftMotor,     tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port4,           wireMotor,     tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port5,           servoAuto,     tmotorServoStandard, openLoop)
#pragma config(Motor,  port7,           powerMotor,    tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port9,           rightMotor,    tmotorVex393_MC29, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "JoystickDriver.c"

/*----------------------------------------------------------------------------------------------------*\

|*                                - BEST Robotics off the grid code -                                 *|

|*                                      ROBOTC on VEX 2.0 Cortex				  														*|

|*                                          By Turnt Ducky  				   																*|

|*												    																																				*|

|*    This program is the code for autonomous delivery, lowering and raising the wire arm, and        *|

|*    dropping off thye conduit pipes. Also this code uses basic joystick controlls to controll       *|

|*																						the robot																								*|

|*												      																																			*|

|*                                        ROBOT CONFIGURATION                                         *|

|*                                                                                                    *|

|*    MOTORS & SENSORS:                                                                               *|

|*    [I/O Port]        [Name]          [Type]                               [Description]            *|

|*    Motor - Port 2   leftMotor    VEX 393 Motor                         Left drive motor            *|

|*    Motor - Port 4   wireMotor    VEX 393 Motor                         Control wire motor          *|

|*    Motor - Port 5   servoAuto    3-wire Servo											    Automated dropper           *|

|*    Motor - Port 7   powerMotor   VEX 393 Motor                         Control power motor         *|

|*    Motor - Port 9   RightMotor   VEX 393 Motor                         Right drive motor           *|

|*    Digital - Port 1 centerIRout  Digital out                           Center sensor output        *|

|*    Digital - Port 2 centerIR		  Digital in                            Center sensor input         *|

|*    Digital - Port 3 leftIRout    Digital out                           Left sensor output          *|

|*    Digital - Port 4 leftIR		    Digital in                            Left sensor input           *|

|*    Digital - Port 5 rightIRout   Digital out                           Right sensor output         *|

|*    Digital - Port 6 rightIR		  Digital in                            Right sensor input          *|

|*    Digital - Port 7 LimitSwitch	Touch                                 Limit Switch for auto       *|

\*----------------------------------------------------------------------------------------------------*/


void robotReady() //Create a void function to be called in task main to make sure everything is set and ready
{
	setServo(servoAuto, -127); //Set the servo motor for the Autonomous delivery to -127 to keep it closed

	return; //return the void statement to say that everything in the void statement is complete
}

int getIrReading(tSensors irReceiverPin) //create a int for IR sensor reading
{
	// IRLED 38 kHz for at least 2 ms
	wait1Msec(1); // Wait 1 ms

	int ir = SensorValue[irReceiverPin]; // IR receiver -> ir variable

	wait1Msec(1); // Down time before recheck

	return ir; //returns ir value
}

task main()
{ //This is to get everything ready
	wait1Msec(2000); // The program waits for 2000 milliseconds before continuing.

	int i = 0; //Set the int i to equal 0 each time code is loaded so no errors happen

	robotReady(); //If the void has ran and set the servo motors to their set postions it will run

	while (true) //While task main is running do the following
	{ //All the following is the actual code for the robot
		//Conduit controls
		if (vexRT[Btn6U] == 1) //If button 6U is pressed
		{ //This is for conduit drop off
			motor[servoAuto] = 127; //Set the servo position to open
		}
		else if (vexRT[Btn6D] == 1) //If button 6D is pressed
		{
			setServo(servoAuto, -127); //Set the servo position to closed
		}
		//Control the wire motor for up and down motions
		if (vexRT[Btn8D] == 1) //If button 8D is pressed
		{ //This is for the user to be able to raise and lower the wire motor for wire hook up
			motor[wireMotor] = 127; //Raise wire motor
		}
		else if (vexRT[Btn8U] == 1) //If button 8U is pressed
		{
			motor[wireMotor] = -127; //Lower wire motor
		}
		else //If neither buttons are pressed
		{
			motor[wireMotor] = 0; //Motor is off
		}
		//Donut time
		if (vexRT[Btn7R] == 1) //If Btn 7R is pressed
		{ //Does a donut because racecar ...
			motor[leftMotor] = 127; //Left motor is max speed
			motor[rightMotor] = -127; //Right motor is max speed in negative direction
		}
		//Control the IR sensors
		if (vexRT[Btn7D]) //If button 7D is pressed
		{ //Sets the value of i to 1 so autonomous can start
			i = 1; //Set the i value as 1
			SensorValue(rightIRout) = 1; //Set the IR output to be on
			SensorValue(centerIRout) = 1; //Set the IR output to be on
			SensorValue(leftIRout) = 1; //Set the IR output to be on
		}
		else if (vexRT[Btn7U]) //If button 7U is pressed
		{ //Turns off IR in case of problems and lets the user drive and stops autonomous in case of problems
			i = 2; //Set the i values as 2
			SensorValue(rightIRout) = 0; //Set the IR output to be off
			SensorValue(centerIRout) = 0; //Set the IR output to be off
			SensorValue(leftIRout) = 0; //Set the IR output to be off
		}
		if (vexRT[Btn5U]) //If button 5U is pressed
		{ //Turns off IR in case of problems and lets the user drive
			i = 3; //Set the i value as 1
			SensorValue(rightIRout) = 0; //Set the IR output to be off
			SensorValue(centerIRout) = 0; //Set the IR output to be off
			SensorValue(leftIRout) = 0; //Set the IR output to be off
		}
		if (i == 1) //If the i value is 1 (7D is pressed)
		{ //Starts the autonomous delivery
			if ((getIrReading(rightIR) == 0) && (getIrReading(centerIR) == 0) &&(getIrReading(leftIR) == 0))  //If all sensors are over white
			{ //This is so the robot will go straight on autonomous
				startMotor(leftMotor, 63); //Left motor is at the value of 63
				startMotor(rightMotor, 63); //Right motor is at the value of 63
			}
			else if (getIrReading(rightIR) == 1) //If right IR is over black
			{ //This lets the autonomous correct if robot is over to much to the right
				startMotor(leftMotor, 63); //Left motor will stay at value 63
				startMotor(rightMotor, 0); //Right motor will stop to allow correction
			}
			else if (getIrReading(leftIR) == 1) //If left IR is over black
			{ //This lets the autonomous correct if robot is over to much to the left
				startMotor(leftMotor, 0); //Left motor will stop to allow correction
				startMotor(rightMotor, 63); //Right otor will stay at value 63
			}
			if(SensorValue [LimitSwitch] == 1) //If limit switch is pressed in
			{ //This is done for autonomous drop off
				stopMotor(leftMotor); //Left motor stops
				stopMotor(rightMotor); //Right motor stops
				setServo(servoAuto, 127); //Set the servo to open
				wait1Msec(500); //Waits 500 milliseconds
				startMotor(leftMotor, -127); //Left motor goes back at full speed
				startMotor(rightMotor, -127); //Right motor goes back at full speed
				wait1Msec(1000); //Waits 1000 milliseconds
				setServo(servoAuto, -127); //Set the servo to close
				wait1Msec(500); //Waits 500 milliseconds
				stopMotor(leftMotor); //Left motor stops
				stopMotor(rightMotor); //Right motor stops
				i = 3; //Set i equal to 3 so it can drive
			}
		}
		//Stops IR line follow
		else if(i == 2) //If the i value is set to 2 (Btn 7U is pressed)
		{ //Stops motors incase autonomous doesn't work
			stopMotor(leftMotor); //Left motor stops
			stopMotor(rightMotor); //Right motor stops
			i = 3; //Sets i to equal to 3 so it can drive
		}
		//Controls the right and left motors with joystick
		else if (i == 3) //If the i value is set to 3 (Btn 5U or 7U is pressed)
		{ //It needs to be done this way to stop jitter on autonomous
			motor[rightMotor] = (vexRT[Ch2] + vexRT[Ch1]) / 2; // (y + x)/2 [Ch2 Controlls right motor]
			motor[leftMotor] = (vexRT[Ch3] - vexRT[Ch4]) / 2; // (y - x)/2 [Ch3 Controlls left motor]
		}
	}
}
