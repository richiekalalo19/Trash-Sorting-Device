#include <NewPing.h>
#include <Servo.h>
const int refreshf = 2550;
const int usip = 12;
const int irip = 11;
const int ldrip = 10; 
const int indip = 9;
const int capip = 8;
const int servop = 3;
const int othledop[] = {7, 6, 5};
const int orgledop[] = {4, 2, 0};
const int usop = 13;
const byte iraip = A0;
const byte ldraip = A1;
int capb, indb, dts, irb_ntc, irb, irv, ldrb_ntc, ldrb, ldrv; 
NewPing sonar(usop, usip, 40);
Servo myservo;
int serial = 1;

void led(int rgbarr[3], char inputarr[3]){
  for(int currRgbArrInd = 0; currRgbArrInd < 3; currRgbArrInd++){
    if((int)inputarr[currRgbArrInd]-48){
      digitalWrite(rgbarr[currRgbArrInd], HIGH);  
    }else{
      digitalWrite(rgbarr[currRgbArrInd], LOW);
    };
  };
  delay(50);
};

void setup() {
  if(serial){Serial.begin(9600);}else{pinMode(0, OUTPUT);};
  pinMode(usop, OUTPUT);
  for(int currLed = 2; currLed < 8; currLed++){
    pinMode(currLed, OUTPUT);
  };
  for(int currIn = 8; currIn < 13; currIn++){
    pinMode(currIn, INPUT);
  };
  led(othledop, "000"); led(orgledop, "000");
  myservo.attach(3);
}

int ldrbg[2], firstloop, OrgCap, OrgLdr;
void loop() { 
  OrgCap = 0; OrgLdr = 0;
  myservo.write(90);
  delay(450);
  capb = digitalRead(capip);
  indb = digitalRead(indip);
  dts = sonar.ping_cm();
  irb = 1; if(digitalRead(irip) == 1){irb = 0;}; irv = analogRead(iraip);
  ldrb = 1; if(digitalRead(ldrip) == 1){ldrb = 0;}; ldrv = analogRead(ldraip);
  if(serial){Serial.println("cap: " + String(capb) + " | ind: " + String(indb) + " | ir: " + String(irb) + " | irv: " + String(irv) + " | dt: " + String(dts) + " | ldrv: " + String(ldrv) + "/1023 (1023||100 NTC)" + " | FirstloopPass: " + String(firstloop));};
  
  if(!(irb || (dts < 7 && dts > 0))){ //No Detect
    led(othledop, "100");
    if(serial){Serial.println("N/D");};
    for(int currIter = 0; currIter < 2; currIter++){
      ldrbg[currIter] = ldrv - 100 + 200*currIter;
    };
    //if(serial){Serial.println(String(ldrbg[0]) + ", " + String(ldrbg[1]));};
    led(othledop, "000");
    delay(refreshf);
    if(!firstloop){firstloop = 1;}
    return;
  };
  if(!firstloop){delay(refreshf); return;}

  if(indb){ //Metal
    if(serial){Serial.println("Metal");};
    led(othledop, "001");
    myservo.write(0);
    delay(refreshf);
    led(othledop, "000");
    return;
  };

  if(capb){ //Organic Capacitance
    OrgCap = 1;
    led(orgledop, "100");
  }else{OrgCap = 0;};
  if(ldrv>ldrbg[1]){ //Organic Opacity
    OrgLdr = 1;
    led(orgledop, "010");
  }else{OrgLdr = 0;};
  
  if(OrgCap == 1 || OrgLdr == 1){ //Organic
    if(serial){Serial.println("Org. OrgCap: " + String(OrgCap) + " OrgLdr: " + String(OrgLdr) + " (" + String (ldrv) + "| bg: " + String(ldrbg[1]) + ")");};
    myservo.write(180);
    delay(refreshf);
    led(orgledop, "000");
    return;
  };

  //Inorg
  if(serial){Serial.println("Inorg");};
  led(othledop, "010");
  myservo.write(0);
  delay(refreshf);
  led(othledop, "000");
  return;
}
