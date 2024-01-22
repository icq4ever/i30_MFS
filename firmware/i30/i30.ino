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

byte buffer[11];    // data buffer

void setup() {
  // put your setup code here, uto run once:
  Serial.begin(9600);

  pinMode(PIN_LIGHT_ON, INPUT_PULLUP);
  pinMode(PIN_WASH_F_SW, INPUT_PULLUP);
  pinMode(PIN_LIGHT_ON, INPUT);
  pinMode(PIN_WIPER_ON, INPUT_PULLUP);
  Serial.println("Started");
}

void loop() {
  readMFS();
  send2P5();
  delay(100);
}

void readMFS(){
  // light switch
  buffer[0] = checkLightSwitch(analogRead(PIN_LIGHT));

  // passing/high switch
  buffer[1]= checkPassingSwitch(analogRead(PIN_PASSING));

  // turn signal
  buffer[2] = checkTurnSignal(analogRead(PIN_TURN));

  // front wiper switch
  buffer[3] = checkFrontWiper(analogRead(PIN_WIPER_F));
  
  // front wiper speed
  buffer[4] = checkFrontWiperSpeed(analogRead(PIN_WIPER_SPEED));
  
  // rear wiper switch
  buffer[5] = checkRearWiperSwitch(analogRead(PIN_WIPER_R));
  
  // washer front sw
  buffer[6] = checkWasherFrontSW(digitalRead(PIN_WASH_F_SW));

  // washer rear sw
  buffer[7] = checkWasherRearSW(digitalRead(PIN_WASH_R_SW));
  
  // light on / wiper on
  buffer[8] = checkLightLowBackupOn(digitalRead(PIN_LIGHT_ON));
  buffer[9] = checkWiperLowBackupOn(digitalRead(PIN_WIPER_ON));
//  buffer[10] = '\n';
}

uint8_t checkLightSwitch(uint16_t val){
  if(val < 300)         return 0;
  else if (val < 700)   return 3;   // LOW
  else if (val < 900)   return 2;   // FOG
  else                  return 1;   // AUTO
}

uint8_t checkPassingSwitch(uint16_t val){
  // Serial.println("!");
  if(val < 500)         return 0;
  else if (val < 850)   return 1;   // high beam
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
  10 byte
  0 : LIGHT (0-3)
  1 : PASS/HIGH (0-2)
  2 : TURN SIGNAL (0-2)
  3 : FRONT WIPER ( 0-4)
  4 : FRONT WIPER SPEED (1-5)
  5 : REAR WIPER (0-2)
  6 : FRONT WASHER SW (0, 1)
  7 : REAR WASHER SW (0, 1)
  8 : LOW LIGHT BACKUP SW (0, 1)
  9: LOW WIPEr BACKUP SW (0, 1)
*/

void send2P5(){
  for(int i=0; i<10; i++){
    Serial.print(buffer[i]);  
  }
  // Serial.write(buffer, 11);
  Serial.println();
}
