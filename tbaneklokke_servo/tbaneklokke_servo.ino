// Dette programmet får en kontinuerlig rotasjonsservo til å rotere
// omtrent 24° (én «posisjon») og deretter stoppe. 
// Etter 6 sekunder (360/24 blir 15, dette skal da svare til minuttene som i dette eksempelet er 6 sekunder)
// Maskinvare: Kontinuerlig rotasjonsservo koblet til digital pinne 9
// Justering: speedValue  – hastighet/dreieretning (>90 = CW, <90 = CCW)
// moveTime er millisekunder motoren får fart, vi har prøvd å kallibrere dette til å være 24 grader,
// men dette er veldig unøyaktig når man bruker en liten billig servo
// delayBetweenSteps – pause mellom hver bevegelse (ms)

#include <Servo.h>

Servo countdownServo; // håndterer signalet til servoen


const int SERVO_PIN = 9;           // PWM‑kapabel pin


const int moveTime = 200;          // ms motoren roterer ≈ 24 grader
const int delayBetweenSteps = 6000; // 6 000 ms pause (6 sekunder)
const int speedValue = 120;        // Så lenge denne er over 90 skal det i teorien få servoen til å bevege seg,
// men vi måtte skru den ganske mye opp for å få det til å skjer

void setup() {
  countdownServo.attach(SERVO_PIN); // bind Servo‑objekt til pinnen
  countdownServo.write(90);         // stopp (nøytral puls)
  delay(1000);                      // stabiliser før start
}

// Denne skal få servoen til å rotere en gang
void rotateChunk() {
  countdownServo.write(speedValue); 
  delay(moveTime);                  // la den rotere i moveTime ms
  countdownServo.write(90);         // stopp
}

void loop() {
  rotateChunk();                    // ta ett «hakk» fremover
  delay(delayBetweenSteps);         // vent 6 s før neste hakk
}
