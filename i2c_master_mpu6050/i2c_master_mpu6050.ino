#include <Wire.h>

const int SLAVE_addr = 8;
const int DATA_SIZE = 6;

int dataReceived[3];

void setup() {
  Wire.begin();        // join i2c bus (address optional for master)
  Serial.begin(9600);  // start serial for output
}

int value = 0;
int count = 0;
bool dataReady = false;
double x, y, z;

unsigned long currentTime, previousTime, deltaTime;

int serialAcc = 0;
int serialRate = 500;



void loop() {
  currentTime = millis();
  deltaTime = currentTime-previousTime;
  previousTime = currentTime;
  
  readI2C();

  if (dataReady) {
    dataReady = false;

    //interpretData();
    
  }

  //printData(deltaTime);
}

void printData(int dt) {
  serialAcc += dt;

  if (serialAcc >= serialRate) {
    serialAcc = 0;

    Serial.print("x°: "); Serial.print (dataReceived[0]); Serial.print("\t");
    Serial.print("y°: "); Serial.print (dataReceived[1]); Serial.print("\t");
    Serial.print("z°: "); Serial.print (dataReceived[2]); Serial.print("\t");

//    Serial.print("x°: "); Serial.print (x); Serial.print("\t");
//    Serial.print("y°: "); Serial.print (y); Serial.print("\t");
//    Serial.print("z°: "); Serial.print (z); Serial.print("\t");
    
    Serial.println();
  }
}

void interpretData() {
  int xAng = dataReceived[0];
  int yAng = dataReceived[1];
  int zAng = dataReceived[2];

  //dataReceived[0] = dataReceived[1] = dataReceived[2] = -99;
  
  // Calculate only for humans
  x = RAD_TO_DEG * (atan2(-yAng, -zAng) + PI); 
  y = RAD_TO_DEG * (atan2(-xAng, -zAng) + PI); 
  z = RAD_TO_DEG * (atan2(-yAng, -xAng) + PI);
}

void readI2C() {
  Wire.requestFrom(SLAVE_addr, DATA_SIZE);
  
  count = 0;
  
  while (Wire.available()) { // slave may send less than requested
    int high = Wire.read(); // receive a byte as character
    int low  = Wire.read(); // receive a byte as character

    value = ((high << 8) & 0xFF00) + (low & 0x00FF);

    Serial.print (value);
    Serial.print ("\t");
    
//    if (count % 2 == 0) {
//      value = c << 8;
//    } else {
//      value = value + c;
//      dataReceived[count >> 1] = value;
//    }
    
    count = count + 1;

    dataReady = true;
  }

  if (dataReady) {
    Serial.println("");  
  }

}

