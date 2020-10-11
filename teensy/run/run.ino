#include <AccelStepper.h>

#include "Animation_Data.h"

// ----------------------------- Audio -----------------------------
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

AudioPlaySdWav           playWav1;
AudioAmplifier           amp1;
AudioOutputI2S           audioOutput;
AudioConnection          patchCord1(playWav1, 0, amp1, 0);
AudioConnection          patchCord2(amp1, 0, audioOutput, 0);
AudioControlSGTL5000     sgtl5000_1;

// Pins für die SD-Karte
#define SDCARD_CS_PIN    10
#define SDCARD_MOSI_PIN  7
#define SDCARD_SCK_PIN   14

bool Play_Audio = false;

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

// Bewegunsbereiche Festlegen
const int L_Augenbraue_Min = -100, L_Augenbraue_Max = 10;        // --------- ÄNDERN
const int R_Augenbraue_Min = 100, R_Augenbraue_Max = -10;         // --------- ÄNDERN
const int Kiefer_Min = -35, Kiefer_Max = 15;                   // --------- ÄNDERN

// Zeit-Variablen
int Counter = 0;
int Interval = 1000 / FPS;
unsigned long previousMillis = 0;

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
  L_Augenbraue.setAcceleration(5000);   // GLÄTTUNG --------- ÄNDERN
  R_Augenbraue.setAcceleration(5000);   // GLÄTTUNG --------- ÄNDERN
  Kiefer.setAcceleration(5000);         // GLÄTTUNG --------- ÄNDERN

  // --------------------------- Audio -----------------------------
  Serial.begin(9600);

  AudioMemory(8);

  // Audio - Lautstärke
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.5); // Kopfhörer Lautstärke??
  amp1.gain(0.50);
  //sgtl5000_1.lineOutLevel(27);

  // SD-Karten - Kommunikation initiieren
  SPI.setMOSI(SDCARD_MOSI_PIN);
  SPI.setSCK(SDCARD_SCK_PIN);
  if (!(SD.begin(SDCARD_CS_PIN))) {
    // Hier das Programm beenden und nur noch diese Nachricht ausgeben
    while (1)
    {
      Serial.println("Die SD-Karte ist doof!");
      digitalWrite(LED, HIGH);
      delay(500);
      digitalWrite(LED, LOW);
      delay(500);
    }
  }
}

// ----------------------------- LOOP ------------------------------
void loop()
{
  if (!digitalRead(Switch))
  {
    // Animation und Audio in Schleife Abspielen, wenn der Schalter an ist
    if (!Play_Audio && getAnimationTime() == 1)
    {
      Play_Audio = true;
      playWav1.play("ATMEN_01.WAV");
    }
    // Das Audio neustarten, wenn Animation vorbei ist
    if (Play_Audio && getAnimationTime() == Animation_Length - 1)
    {
      Play_Audio = false;
      playWav1.stop();
    }
    digitalWrite(LED, LOW);
    runAnimation();
  }
  else
  {
    // Alles auf 0 zurücksetzten und Audio stoppen
    toZero();
    setAnimationTime(0);
    if (Play_Audio)
    {
      Play_Audio = false;
      digitalWrite(LED, HIGH);
      playWav1.stop();
    }
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
void setAnimationTime(int Frames)
{
  Counter = Frames;
}
void runAnimation()
{
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= Interval)
  {
    previousMillis = currentMillis;

    Serial.println("KieferS: " + String(map(Animation_Sequence[Counter][2], 0, 255, Kiefer_Min, Kiefer_Max)) +
                   "\tKieferI: " + String(Kiefer.currentPosition()) +
                   "\tFrame: " + String(Counter) +
                   "\tAudio: " + String(float(playWav1.positionMillis()) / 1000.0f) +
                   "\tSekunden: " + String(float(millis()) / 1000.0f));

    if (Counter < Animation_Length)
    {
      Counter++;
    }
    else
    {
      Counter = 0;
    }

    int L_Augenbraue_Target = map(Animation_Sequence[Counter][0], 0, 255, L_Augenbraue_Min, L_Augenbraue_Max);
    int R_Augenbraue_Target = map(Animation_Sequence[Counter][1], 0, 255, R_Augenbraue_Min, R_Augenbraue_Max);
    int Kiefer_Target = map(Animation_Sequence[Counter][2], 0, 255, Kiefer_Min, Kiefer_Max);
    L_Augenbraue.setMaxSpeed(norm(L_Augenbraue_Target - L_Augenbraue.currentPosition()) * FPS);
    R_Augenbraue.setMaxSpeed(norm(R_Augenbraue_Target - R_Augenbraue.currentPosition()) * FPS);
    Kiefer.setMaxSpeed(norm(Kiefer_Target - Kiefer.currentPosition()) * FPS);
    L_Augenbraue.moveTo(L_Augenbraue_Target);
    R_Augenbraue.moveTo(R_Augenbraue_Target);
    Kiefer.moveTo(Kiefer_Target);
  }

  L_Augenbraue.run();
  R_Augenbraue.run();
  Kiefer.run();
}

int getAnimationTime()
{
  return Counter;
}

int norm(int i)
{
  if (i < 0)
    return i * (-1);
  else
    return i;
}
