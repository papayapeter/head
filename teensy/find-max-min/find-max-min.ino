#include <AccelStepper.h>

// ---------------------------- Motoren ----------------------------
// Pins zur Motorsteuerung
#define L_Augenbraue_Step 26
#define L_Augenbraue_Dir  31
#define R_Augenbraue_Step 28
#define R_Augenbraue_Dir  29
#define Kiefer_Step       27
#define Kiefer_Dir        30
#define Microstepping     32
#define Switch            25

#define LED               13

// Motoren
AccelStepper L_Augenbraue(1, L_Augenbraue_Step, L_Augenbraue_Dir);
AccelStepper R_Augenbraue(1, R_Augenbraue_Step, R_Augenbraue_Dir);
AccelStepper Kiefer(1, Kiefer_Step, Kiefer_Dir);

// Bewegunsbereiche Festlegen                                     ---!!!---
const int L_Augenbraue_Min = -120, L_Augenbraue_Max = 0;        // ÄNDERN
const int R_Augenbraue_Min = 120, R_Augenbraue_Max = 0;        // ÄNDERN
const int Kiefer_Min = -150, Kiefer_Max = 70;                    // ÄNDERN

boolean L_Augenbraue_Positiv = false, R_Augenbraue_Positiv = false, Kiefer_Positiv = false;

// ----------------------------- SETUP -----------------------------
void setup()
{
  // -------------------------- Motoren ----------------------------
  // Pinmodes definieren
  pinMode(Microstepping, OUTPUT);
  pinMode(Switch, INPUT_PULLUP);
  pinMode(LED, OUTPUT);

  // Microstepping einschalten
  digitalWrite(Microstepping, HIGH);
  
  // Beschleunigung hoch setzten
  L_Augenbraue.setAcceleration(10000);
  R_Augenbraue.setAcceleration(10000);
  Kiefer.setAcceleration(10000);
}

// ----------------------------- LOOP ------------------------------
void loop()
{
  if (!digitalRead(Switch))
  {
    L_Augenbraue.setMaxSpeed(50);
    R_Augenbraue.setMaxSpeed(50);
    Kiefer.setMaxSpeed(50);
    L_Augenbraue.moveTo(L_Augenbraue_Min);
    R_Augenbraue.moveTo(R_Augenbraue_Min);
    Kiefer.moveTo(Kiefer_Min);
    L_Augenbraue.run();
    R_Augenbraue.run();
    Kiefer.run();
//    runAnimation();
    digitalWrite(LED, LOW);
  }
  else
  {
    toZero();
    digitalWrite(LED, HIGH);
  }
}

// Funktionen
void toZero()
{
  L_Augenbraue.setMaxSpeed(200);
  R_Augenbraue.setMaxSpeed(200);
  Kiefer.setMaxSpeed(200);
  L_Augenbraue.moveTo(0);
  R_Augenbraue.moveTo(0);
  Kiefer.moveTo(0);

  L_Augenbraue.run();
  R_Augenbraue.run();
  Kiefer.run();
}
void runAnimation()
{
  L_Augenbraue.setMaxSpeed(50);
  R_Augenbraue.setMaxSpeed(50);
  Kiefer.setMaxSpeed(50);

  // linke Augenbraue
  if (L_Augenbraue.currentPosition() == 0)
  {
    if (L_Augenbraue_Positiv)
    {
      L_Augenbraue.moveTo(L_Augenbraue_Max);
    }
    else
    {
      L_Augenbraue.moveTo(L_Augenbraue_Min);
    }
  }
  else if (L_Augenbraue.currentPosition() == L_Augenbraue_Min)
  {
    L_Augenbraue_Positiv = true;
    L_Augenbraue.moveTo(0);
  }
  else if (L_Augenbraue.currentPosition() == L_Augenbraue_Max)
  {
    L_Augenbraue_Positiv = false;
    L_Augenbraue.moveTo(0);
  }
  // rechte Augenbraue
  if (R_Augenbraue.currentPosition() == 0)
  {
    if (R_Augenbraue_Positiv)
    {
      R_Augenbraue.moveTo(R_Augenbraue_Max);
    }
    else
    {
      R_Augenbraue.moveTo(R_Augenbraue_Min);
    }
  }
  else if (R_Augenbraue.currentPosition() == R_Augenbraue_Min)
  {
    R_Augenbraue_Positiv = true;
    R_Augenbraue.moveTo(0);
  }
  else if (R_Augenbraue.currentPosition() == R_Augenbraue_Max)
  {
    R_Augenbraue_Positiv = false;
    R_Augenbraue.moveTo(0);
  }
  // Kiefer
  if (Kiefer.currentPosition() == 0)
  {
    if (Kiefer_Positiv)
    {
      Kiefer.moveTo(Kiefer_Max);
    }
    else
    {
      Kiefer.moveTo(Kiefer_Min);
    }
  }
  else if (Kiefer.currentPosition() == Kiefer_Min)
  {
    Kiefer_Positiv = true;
    Kiefer.moveTo(0);
  }
  else if (Kiefer.currentPosition() == Kiefer_Max)
  {
    Kiefer_Positiv = false;
    Kiefer.moveTo(0);
  }

  L_Augenbraue.run();
  R_Augenbraue.run();
  Kiefer.run();
}

int norm(int i)
{
  if (i < 0)
    return i * (-1);
  else
    return i;
}
