//Использование выводов
/*
  ===PWM==============================================================
  Моторы
  4 ---> правый мотор PWM1 (+ земля)
  5 ---> левый мотор PWM2
  ===DIGITAL==========================================================
  Моторы
  52 ---> Direction-пин правого мотора DIR1
  53 ---> Direction-пин левого мотора DIR2
  ===ANALOG==========================================================
  нет
*/
//#include <Kalman.h>
//#include <Metro.h>
#include <PID_v1.h>
#include <math.h>
#include <TimerOne.h> // http://www.arduino.cc/playground/Code/Timer1
#include <Wire.h>
//#include <VL53L0X.h>
//#include <LiquidCrystal.h>
// #include <Check_times.cpp>

//#include <TroykaIMU.h>
#include <Wire.h>
#include <stdlib.h>
#include <stdio.h>

#include "PIDtuner.h"  // GyverPID
PIDtuner tuner;  // rele-mod

#include "PIDtuner2.h"  // GyverPID
// #include "configs/blue.h"
#include "configs/orange.h"

//PIDtuner2 tuner;  // Cohen-Coon mod

// MegaADK DIGITAL PINS USABLE FOR INTERRUPTS 2, 3, 18, 19, 20, 21
//                                                 I2C pins 20, 21

//Encoder variables

const byte encoderRpinA = ENCODER_R_A;                              //A pin -> the interrupt pin (2)
const byte encoderRpinB = ENCODER_R_B;                              //B pin -> the digital pin (16)
const byte encoderLpinA = ENCODER_L_A;                              //A pin -> the interrupt pin (3)
const byte encoderLpinB = ENCODER_L_B;                              //B pin -> the digital pin (17)


byte encoderRPinALast;
byte encoderLPinALast;
double wheelSpeedR = 0;  // Скорость правого колеса с энкодера
double wheelSpeedL = 0;  // Скорость левого колеса с энкодера
unsigned long wheelImpR = 0; // число импульсов с энкодера правого колеса
unsigned long wheelImpL = 0; // число импульсов с энкодера левого колеса

long long prevWheelImpR = 0; // число импульсов с энкодера правого колеса
long long prevWheelImpL = 0; // число импульсов с энкодера левого колеса

//PID variables
double Motor_2[3] = {P, I, D}; // {0.1,3,0};                //PID parameters [P,I,D]
double Setpoint1, Input1, Output1;                 //PID input&output values for Motor1
double Setpoint2, Input2, Output2;                 //PID input&output values for Motor2

PID myPID1(&Input1, &Output1, &Setpoint1, Motor_2[0], Motor_2[1], Motor_2[2], DIRECT);
PID myPID2(&Input2, &Output2, &Setpoint2, Motor_2[0], Motor_2[1], Motor_2[2], DIRECT);

// Timer variables
const long Timer1Interval = 100000;                              // 100 ms = 10 times per sec - Timer interrupt interval
double dT = double(Timer1Interval) / 1000000;         // период счета

//Motor control variables
const int MotorRdir = MOTOR_R_DIR;  // 52    //Right motor Direction Control pin
const int MotorLdir = MOTOR_L_DIR;  // 53    //Left motor Direction Control pin
const int MotorRpwm = MOTOR_R_PWM;  // 4     //Right motor PWM Speed Control pin
const int MotorLpwm = MOTOR_L_PWM;  // 5     //Left motor PWM Speed Control pin
double SetSpeedR = 0;   //Wish Speed of right motor
double SetSpeedL = 0;   //Wish Speed of left motor
bool DirectionR = 0;     //Right Motor Direction
bool DirectionL = 0;     //Left Motor Direction

//-------------------------События------------------------------------------------------
bool settingSpeed = false; // команда установки скорости

//------------------------------------------------
#include <string.h>
char buffer[256];
double LinearVelocity, AngularVelocity;
double readV, readW;
double wheelLeftS = 0;
double wheelRightS = 0;
double wheelLeftV = 0;
double wheelRightV = 0;
double omegaRight = 0;
double omegaLeft = 0;
double R = WHEEL_RADIUS;
double L = BASE_WIDTH;
double V = 0;
double omega = 0;
double Vl = 0;
double Vr = 0;
double SetV = 0;
double SetW = 0;
double maxSpeed = MAX_SPEED;  // максимальная линейная скорость при скважности 100%, в м/с
// int    PPR = 1450;             // импульсов энкодера за оборот

// double CL = 1;  // корректирующие коеффициенты для радиусов колес
// double CR = 1;

double yaw = 0;
double x = 0;
double y = 0;

bool printflag = false;
bool isTuningMode = false;
bool is_connected = false;

unsigned long numRightPulse = 0;
unsigned long numLeftPulse = 0;

bool testFinishR = true;
bool testFinishL = true;

void printValue(double val, const char* namVal = NULL, bool withSignAndDouble = true);

void setup() {
  Init();
}
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Главный цикл ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void loop() {
  // --------------- Чтение порта --------------------
  get_messages_from_SERIAL();
  // --------------- Смена уставки скорости ----------
  Motor();
  // -------------------------------------------------
}
//loop ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Motor() {
  CheckSettingsSpeed();
  PIDMovement(SetSpeedR, SetSpeedL); // передается линейная скорость колес, в м/сек
}
void SetSpeed(double LinearVelocity, double AngularVelocity) {
  SetSpeedR = (((2 * LinearVelocity) + (AngularVelocity * L)) / (2 * R)) * R; //M/S - линейная скорость колеса
  SetSpeedL = (((2 * LinearVelocity) - (AngularVelocity * L)) / (2 * R)) * R;
}
void CheckSettingsSpeed() {
  if (settingSpeed) {
    settingSpeed = false;
    SetSpeed(LinearVelocity, AngularVelocity);
  }
}
// --------------- Чтение порта --------------------

void reset_var() {
  x = 0;
  y = 0;
  yaw = 0;
  V = 0;
  SetSpeedR = 0;   //M/S - линейная скорость колеса
  SetSpeedL = 0;
}

void Init() {
  Wire.begin();
  SERIAL.begin(57600);  // 57600  //Initialize the SERIAL1 port
  while (!SERIAL) ; // while the SERIAL stream is not open, do nothing
  MotorsInit();
  EncoderInit();//Initialize encoder
  PIDInit();
}

void PIDInit() {
  myPID1.SetMode(AUTOMATIC);
  myPID2.SetMode(AUTOMATIC);
  myPID1.SetOutputLimits(0, 255);
  myPID2.SetOutputLimits(0, 255);

  tuner.setParameters(NORMAL, 130, 40, 2000, 3, 2000, dT * 1000);
//  tuner.setParameters(NORMAL, 100, 200, 2000, 3, dT * 1000);
}
void MotorsInit() { //Initialize motors variables
  DirectionR = LOW;
  DirectionL = LOW;
  SetSpeedR = 0;
  SetSpeedL = 0;

  pinMode(MotorRdir, OUTPUT);
  pinMode(MotorLdir, OUTPUT);
  pinMode(MotorRpwm, OUTPUT);
  pinMode(MotorLpwm, OUTPUT);

  digitalWrite (MotorRdir, DirectionR);
  digitalWrite (MotorLdir, DirectionL);
  analogWrite (MotorRpwm, SetSpeedR);
  analogWrite (MotorLpwm, SetSpeedL);
}
void EncoderInit() { //Initialize encoder interruption

  pinMode(encoderRpinA, INPUT); // Right weel
  pinMode(encoderRpinB, INPUT);
  pinMode(encoderLpinA, INPUT); // Left weel
  pinMode(encoderLpinB, INPUT);

  // Привязка прерывания по импульсу энкодера
  attachInterrupt(digitalPinToInterrupt(encoderRpinA), WheelPulseR, RISING ); // вызов процедуры по прерыванию. Параметры: номер прерывания (не ножки), имя процедуры, состояние сигнала
  attachInterrupt(digitalPinToInterrupt(encoderLpinA), WheelPulseL, RISING );  // ЗАМЕНА, была ссылка на DecodeSpeedL

  // Настройка таймера
  Timer1.initialize(Timer1Interval);
  Timer1.attachInterrupt(Timer_finish);

}
void WheelPulseR() {   // Счетчик спиц правого колеса
  wheelImpR++;

  if (!testFinishR) {
    if (wheelImpR > numRightPulse) {
      SetSpeedR = 0;
      testFinishR = true;
    }
  }
}

void WheelPulseL() {   // Счетчик спиц левого колеса
  wheelImpL++;

  if (!testFinishL) {
    if (wheelImpL > numLeftPulse) {
      SetSpeedL = 0;
      testFinishL = true;
    }
  }
}

void Timer_finish()  {
  wheelSpeedR = double((wheelImpR - prevWheelImpR) / dT); // число импульсов за сек
  wheelSpeedL = double((wheelImpL - prevWheelImpL) / dT); // число импульсов за сек

  prevWheelImpR = wheelImpR;
  prevWheelImpL = wheelImpL;

  if (isTuningMode) {
    tuner.setInput(wheelSpeedR); // передаём текущее значение с датчика
    tuner.compute(); // тут производятся вычисления по своему таймеру
    analogWrite(MotorRpwm, tuner.getOutput());
    digitalWrite(MotorRdir, DirectionR);
    analogWrite(MotorLpwm, tuner.getOutput());
    digitalWrite(MotorLdir, DirectionR);

//    tuner.debugText();

    if (tuner.getAccuracy() > 95) {
      analogWrite(MotorRpwm, 0);
      analogWrite (MotorLpwm, 0);

      isTuningMode = false;
    }
  }

  // пройденный колесом путь, м
  wheelRightS = ((wheelSpeedR / PPR) * 2 * 3.14 * R) * CR; // 663  // метры L = 2*PI*R*n/N
  wheelLeftS  = ((wheelSpeedL / PPR) * 2 * 3.14 * R) * CL; //*

  // линейная скорость колеса
  wheelRightV = wheelRightS / 1; // mетры за сек
  wheelLeftV  = wheelLeftS / 1;

  // угловая скорость колеса
  omegaRight = wheelRightV / R; // rad за сек
  omegaLeft  = wheelLeftV / R;

  // фактическая линейная скорость центра робота
  V     = (wheelRightV * (DirectionR ? -1 : 1) + wheelLeftV * (DirectionL ? -1 : 1)) / 2;//m/s
  // фактическая угловая скорость поворота робота
  omega = (wheelRightV * (DirectionR ? -1 : 1) - wheelLeftV * (DirectionL ? -1 : 1)) / L;

  yaw += (omega * dT);  // направление в рад
  x += V * cos(yaw) * dT; // в метрах
  y += V * sin(yaw) * dT;

  // проверка
  Vr = (((2 * V) + (omega * L)) / (2 * R)) * R; //M/S
  Vl = (((2 * V) - (omega * L)) / (2 * R)) * R;
}
void Movement(int a, int b) { //move
  if (a < 10) {
    a = 0;
  }
  if (b < 10) {
    b = 0;
  }
  analogWrite (MotorRpwm, a);     //motor1 move forward at speed a
  digitalWrite(MotorRdir, DirectionR);
  analogWrite (MotorLpwm, b);     //motor2 move forward at speed b
  digitalWrite(MotorLdir, DirectionL);
}

//PID modules
void PIDMovement(double a, double b) {
  // a, b - m/sec

  if (a < 0) {
    a = abs(a);
    DirectionR = true;
  }
  else {
    DirectionR = false;
  }

  if (b < 0) {
    b = abs(b);
    DirectionL = true;
  }
  else {
    DirectionL = false;
  }

  if (isTuningMode) {
    return;
  }

  Setpoint1 = (a * 255 / maxSpeed); // уставка скорости
  Setpoint2 = (b * 255 / maxSpeed);

  Input1 = wheelRightV * 255 / maxSpeed;          // обратная связь ПИД-регулятора, м/сек
  Input2 = wheelLeftV * 255 / maxSpeed;

  myPID1.Compute();
  myPID2.Compute();

  if (a < 0.01) {
    Output1 = 0;
  }

  if (b < 0.01) {
    Output2 = 0;
  }

  Movement (int (Output1), int(Output2));

  // Movement (int (a * 255 / maxSpeed), int(b * 255 / maxSpeed));
}

void get_messages_from_SERIAL()
{
  if (SERIAL.available() > 0)
  {
    // The first byte received is the instruction
    int order_received = SERIAL.read();

    if (order_received == 's')
    {
      // If the cards haven't say hello, check the connection
      if (!is_connected)
      {
        is_connected = true;
        SERIAL.print("r");
      }
    }
    else
    {
      switch (order_received)
      {
        case 'i':
        {
          printValue(Motor_2[0]);
          printValue(Motor_2[1]);
          printValue(Motor_2[2]);

          SERIAL.print("\n");

          break;
        }

        case 'v'://если v, то считываем уставку по скорости
          {

            String line = SERIAL.readStringUntil('\n');// считываем скорости для левого и правого колеса [40 50]
            line.toCharArray(buffer, 10); //переводим в char
            LinearVelocity = atof(strtok(buffer, " ")); //разделяем на скорости левого и правого колеса
            AngularVelocity = atof(strtok(NULL,  " "));

            //          printValue(LinearVelocity, "LinearVelocity");
            //          printValue(AngularVelocity, "AngularVelocity");

            settingSpeed = true;

            break;
          }

        case 'a':
          {
            isTuningMode = true;
            tuner.reset();

            break;
          }

        case 'k':
          {

            String line = SERIAL.readStringUntil('\n');
            line.toCharArray(buffer, line.length() + 1);

            double p = atof(strtok(buffer, " "));

            char* next = strchr(buffer, ' ');
            double i = atof(strtok(next, " "));

            next = strchr(next, ' ');
            double d = atof(strtok(next, " "));

            myPID1.SetTunings(p, i, d);
            myPID2.SetTunings(p, i, d);

            break;
          }

        case 'd'://если d, то печатаем текущие значения полно
          {
            printValue(V, "V");
            printValue(omega, "Omega");

            printValue(yaw, "Yaw");
            printValue(x, "x");
            printValue(y, "y");

            SERIAL.print("\n");

            break;
          }

        case 'o'://если o, то печатаем текущие значения сжато
          {
            printValue(V);  // linear velocity
            printValue(omega);  // angular velocity

            printValue(yaw);  // yaw angle
            printValue(x);  // x position
            printValue(y);  // y position

            SERIAL.print(testFinishR && testFinishL);

            if (SERIAL.read() == 't') {
//              printValue(tuner.getState());
              printValue(tuner.getAccuracy(), NULL, false);

              printValue(tuner.getPID_p());
              printValue(tuner.getPID_i());
              printValue(tuner.getPID_d());
              printValue(isTuningMode ? 0 : 1, NULL, false);
            }

            SERIAL.print("\n");

            break;
          }

        case 'p'://если p, то пауза
          {
            LinearVelocity = 0;
            AngularVelocity = 0;
            SetSpeedR = 0;
            SetSpeedL = 0;

            settingSpeed = true;

            //          SERIAL.print("Stop command");
            //          SERIAL.print("\n");

            break;
          }

        case 'n':
          {
            reset_var();
            break;
          }

        case 't':
          {
            while(SERIAL.available() == 0) {}
            char type = SERIAL.read();

            if (type == 'l') {
              String line = SERIAL.readStringUntil('\n');
              float targetLine = line.toFloat();

              numRightPulse = targetLine / (2 * 3.14 * R / PPR);
              numLeftPulse = targetLine / (2 * 3.14 * R / PPR);

              prevWheelImpR = prevWheelImpR - wheelImpR;
              prevWheelImpL = prevWheelImpL - wheelImpL;

              wheelImpR = 0;
              wheelImpL = 0;

              testFinishR = false;
              testFinishL = false;
            }

            if (type == 'a') {
              String line = SERIAL.readStringUntil('\n');
              float targetAngle = line.toFloat();

              numRightPulse = targetAngle * L / (2 * R) / (2 * 3.14) * PPR;
              numLeftPulse = targetAngle * L / (2 * R) / (2 * 3.14) * PPR;

              prevWheelImpR = prevWheelImpR - wheelImpR;
              prevWheelImpL = prevWheelImpL - wheelImpL;

              wheelImpL = 0;
              wheelImpR = 0;

              testFinishR = false;
              testFinishL = false;
            }

            SERIAL.print("\n");

            break;
          }

        // Unknown order
        default:
          printValue(order_received, "Unknown command", false);
          break;
      }
    }

    SERIAL.flush();
  }
}

void printValue(double val, const char* namVal, bool withSignAndDouble) {
  SERIAL.print(namVal == NULL ? "" : String(namVal) + ": ");

  if (withSignAndDouble) {
    SERIAL.print(val >= 0 ? "+" : "");
    SERIAL.print(val);
  } else {
    SERIAL.print(int(val));
  }

  SERIAL.print("; ");
}
