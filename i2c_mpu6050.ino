#include <Wire.h> 
#define SERIAL_ON false

const int THIS_addr = 0x08;
const int MPU_addr = 0x68;
int16_t AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ;

int minVal = 265; int maxVal = 402;

double x; double y; double z;

unsigned long currentTime, previousTime, deltaTime;

int pollingAcc = 0;
int pollingRate = 50;

int serialAcc = 0;
int serialRate = 500;

int sendDelay = 100; 
int sendAcc = 0;

bool dataReady = false;


void setup(){
  Wire.begin(THIS_addr);        // join i2c bus with address #8
  Wire.onReceive(requestEvent); // register event
  
  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);

  // Configuration du low-pass filter pour
  // eliminer le bruit
  Wire.beginTransmission(0x68);
  Wire.write(0x1A);  // the config address
  Wire.write(0x05);  // the config value
  Wire.endTransmission(true);

  Serial.begin(9600);

  currentTime = millis();
  previousTime = currentTime;
}

int xAng, yAng, zAng;

void loop() {
  currentTime = millis();
  deltaTime = currentTime-previousTime;
  previousTime = currentTime;

  pollingAcc += deltaTime;

  if (pollingAcc >= pollingRate) {
    pollingAcc = 0;
  
    Wire.beginTransmission(MPU_addr);
    Wire.write(0x3B);
    Wire.endTransmission(false);
  
    Wire.requestFrom(MPU_addr, 14, true);
    
    AcX = Wire.read() << 8 | Wire.read(); 
    AcY = Wire.read() << 8 | Wire.read(); 
    AcZ = Wire.read() << 8 | Wire.read();
    
    xAng = map(AcX, minVal, maxVal, -90, 90); 
    yAng = map(AcY, minVal, maxVal, -90, 90); 
    zAng = map(AcZ, minVal, maxVal, -90, 90);
  
    dataReady = true;
  }

  if (SERIAL_ON) {
    serialAcc += deltaTime;
    if (serialAcc >= serialRate) {
      serialAcc = 0;
  
      // Calculate only for humans
      x = RAD_TO_DEG * (atan2(-yAng, -zAng) + PI); 
      y = RAD_TO_DEG * (atan2(-xAng, -zAng) + PI); 
      z = RAD_TO_DEG * (atan2(-yAng, -xAng) + PI);
  
      Serial.print("AngleX= "); Serial.println(x);
    
      Serial.print("AngleY= "); Serial.println(y);
    
      Serial.print("AngleZ= "); Serial.println(z); 
      Serial.println("-----------------------------------------");
    }
  }
}

int value = 0;
void requestEvent() {
  if(dataReady){
    Wire.write (65);
    Serial.println (0x65);
    // Each integer is 2 bytes
//    Wire.write (xAng & 0xFF00 >> 8);
//    Wire.write (xAng & 0x00FF);
//    Wire.write (yAng & 0xFF00 >> 8);
//    Wire.write (yAng & 0x00FF);
//    Wire.write (zAng & 0xFF00 >> 8);
//    Wire.write (zAng & 0x00FF);
    dataReady = false;
  
  }
  
}
