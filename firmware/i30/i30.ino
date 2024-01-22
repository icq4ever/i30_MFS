/*
  arduino pin mapping 
*/
#define PIN_WASH_R_SW 2
#define PIN_WASH_F_SW 3
#define PIN_LIGHT_ON  5
#define PIN_WIPER_ON  9
#define PIN_WIPER_F A0
#define PIN_WIPER_SPEED A1
#define PIN_WIPER_R A7
#define PIN_LIGHT A6
#define PIN_TURN  A2
#define PIN_PASSING A3

byte buffer[12];    // data buffer

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  pinMode(PIN_LIGHT_ON, INPUT_PULLUP);
  pinMode(PIN_WASH_F_SW, INPUT_PULLUP);
  pinMode(PIN_LIGHT_ON, INPUT);
  pinMode(PIN_WIPER_ON, INPUT_PULLUP);
  Serial.println("Started");
}

void loop() {
  delay(100);
}

void readMFS(){
  buffer[0] = '/';

  // light switch
  buffer[1] = checkLightSwitch(analogRead(PIN_LIGHT));

  // passing/high switch
  buffer[2]= checkPassingSwitch(analogRead(PIN_PASSING));

  // turn signal
  buffer[3] = checkTurnSignal(analogRead(PIN_TURN));

  // front wiper switch
  buffer[4] = checkFrontWiper(analogRead(PIN_WIPER_F));
  
  // front wiper speed
  buffer[5] = checkFrontWiperSpeed(analogRead(PIN_WIPER_SPEED));
  
  // rear wiper switch
  buffer[6] = checkRearWiperSwitch(analogRead(PIN_WIPER_R));
  
  // washer front sw
  buffer[7] = checkWasherFrontSW(digitalRead(PIN_WASH_F_SW));

  // washer rear sw
  buffer[8] = checkWasherRearSW(digitalRead(PIN_WASH_R_SW));
  
  // light on / wiper on
  buffer[9] = checkLightLowBackupOn(digitalRead(PIN_LIGHT_ON));
  buffer[10] = checkWiperLowBackupOn(digitalRead(PIN_WIPER_ON));
  buffer[11] = '\n';
}

uint8_t checkLightSwitch(uint16_t val){
  if(val < 300)         return 0;
  else if (val < 500)   return 3;   // LOW
  else if (val < 800)   return 2;   // FOG
  else                  return 1;   // AUTO
}

uint8_t checkPassingSwitch(uint8_t val){
  if(val < 500)   return 0;
  else if (val < 750)   return 1;   // high beam
  else                  return 2;   // passing
}

uint8_t checkTurnSignal(uint16_t val){
  if(val < 600)         return 0;
  else if (val < 850)   return 2;   // right
  else                  return 1;   // left
}


uint8_t checkFrontWiper(uint16_t val){
  if(val < 200)        return 0;
  else if(val < 700)   return 4;
  else if (val < 900)  return 3;
  else if (val < 950)  return 2;
  else                 return 1;
  
}

uint8_t checkFrontWiperSpeed(uint16_t val){
  if (val< 700)         return 1;
  else if (val < 850)   return 2;
  else if (val < 930)   return 3;
  else if (val < 980)   return 4;
  else                  return 5;
}

uint8_t checkRearWiperSwitch(uint16_t val){
  if(val < 500)         return 0;
  else if (val < 850)   return 2;
  else                  return 1;
}



uint8_t checkWasherFrontSW(uint8_t val){
  if(val == 0)  return 1;
  else          return 0;
}

uint8_t checkWasherRearSW(uint8_t val){
  if(val == 0)  return 1;
  else          return 0;
}


uint8_t checkWiperLowBackupOn(uint8_t val){
  if(val == 0)    return 1;
  else            return 0;
}

uint8_t checkLightLowBackupOn(uint8_t val){
  return val;
}

/*
  11 byte
  0 : START CHAR ('/');
  1 : LIGHT (0-3)
  2 : PASS/HIGH (0-2)
  3 : TURN SIGNAL (0-2)
  4 : FRONT WIPER ( 0-4)
  5 : FRONT WIPER SPEED (1-5)
  6 : REAR WIPER (0-2)
  7 : FRONT WASHER SW (0, 1)
  8 : REAR WASHER SW (0, 1)
  9 : LOW LIGHT BACKUP SW (0, 1)
  10 : LOW WIPEr BACKUP SW (0, 1)
*/

void send2P5(){
  Serial.write(buffer, 12);
}