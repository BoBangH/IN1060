// Dette programmet kontrollerer en bipolar steppermotor med TB6612FNG driver
// Dette programmet roterer motoren 24,3 grader hvert 6. sekund 
// Dette er kun for demonstrasjon, den skal egentlig gå hvert minutt
// TB6612FNG har to H-bro kanaler, AIN1 og 2 styrer retning for den ene spolen. 
// BIN1 og 2 styrer andre veien. PWMA/PWMB styrer motstrøm via PWM

const int AIN1 = 6;
const int AIN2 = 5;
const int PWMA = 3;          // PWM-kapabel pin
const int BIN1 = 11;
const int BIN2 = 12;
const int PWMB = 10;         // PWM-kapabel pin
const int STBY = 8;          // når denne er HIGH er driver aktiv

// delayTime er pause mellom hvert halvsteg, dette kontrolerer hastighet
// 27 halvsteg er tilnærmet lik 24,3 grader, 
// dette er så nøyaktig vi får denne driveren til å gå da den ikke kan microsteppe
// waitTime er hvor lenge det er før stepper-motoren aktiverer (6s som egentlig skal være 60s)
const int delayTime = 5;              // ms mellom hvert halvsteg
const int stepsPerSegment = 27;       // 27 halvsteg ≈ 24.3 grader
const unsigned long waitTime = 6000; // 6 s hvile

int stepIndex = 0; // Viser nårværende halvsteg fra 0 til 7
unsigned long lastMoveTime = 0; // tid siden vi sist startet beveglse

// Under er sekvensen av halvstegene, dette er da hver rad (AIN1, AIN2, BIN1, BIN2) i rekkefølge
// 1 = HIGH, 0 = LOW
// Vi kjører a, b, a, b for jevnt dreiemoment og 8 delsteg per helsteg
const byte stepSequence[8][4] = {
  {1,0,1,0},{1,0,0,0},{1,0,0,1},{0,0,0,1},
  {0,1,0,1},{0,1,0,0},{0,1,1,0},{0,0,1,0}
};

void setup() {
  pinMode(7, INPUT_PULLUP);     // Denne pinnen på være deaktivert da vi klarte å brekke av en
  // liten bit metal fra en ledning som satt seg fast i pinnen
  pinMode(AIN1, OUTPUT); pinMode(AIN2, OUTPUT);
  pinMode(PWMA, OUTPUT); pinMode(PWMB, OUTPUT);
  pinMode(BIN1, OUTPUT); pinMode(BIN2, OUTPUT);
  pinMode(STBY, OUTPUT);
  digitalWrite(STBY, HIGH);     // aktiverer driver
  
  //Kjører driver
  analogWrite(PWMA, 255);       
  analogWrite(PWMB, 255);
  lastMoveTime = millis(); // Starttid
}

void loop() {
  // Sjekker om det har gått riktig waitTime siden forrige bevegelse
  if (millis() - lastMoveTime >= waitTime) { 
    rotateSegment();
    lastMoveTime += waitTime;   
  }
}

//Denne funksjonen får motoren til å rotere stepsPerSegment halvsteg fremover
void rotateSegment() {
  for (int i = 0; i < stepsPerSegment; ++i) {
    setStep(stepIndex);  // Setter spoler etter tabellen
    stepIndex = (stepIndex + 1) & 7;  // Neste halvsteg
    delay(delayTime);  
  }
}

//Denne gir korrekt HIGH/LOW til hver H-bro inngang for gitt indeks i stepSequence
inline void setStep(byte idx) {
  digitalWrite(AIN1, stepSequence[idx][0]);
  digitalWrite(AIN2, stepSequence[idx][1]);
  digitalWrite(BIN1, stepSequence[idx][2]);
  digitalWrite(BIN2, stepSequence[idx][3]);
}
