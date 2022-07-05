


#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <HX711.h>


// HX711 circuit wiring

const int LOADCELL_DOUT_PIN = 4;
const int LOADCELL_SCK_PIN = 5;
const int CS = 10;


HX711 scale;

double mtime;
double force;
double dt;
double dp;
double impulse;
double last;
double ig;


File dataFile;
void setup() {

  Serial.begin(57600);

int done = 0;  // Global

  Serial.println("First column - time elasped.");
  delay(500);
  Serial.println("Second column - thrust measured in N");
  delay(500);
  Serial.println("Third Column - Total Impulse");
  delay(500);
  Serial.println("Press G and Enter to continue");
  Serial.println("...");
  delay(100);
  Serial.println("...");
  delay(100);
  Serial.println("...");
  delay(100);
  Serial.println("...");
  delay(100);

  
  while(done == 0)
  {
  while (Serial.available() > 0)
{
if (Serial.read() == 'G')
  {
  done = 1;
  } 
}
  }
while(Serial.available() > 0)
  {
byte dummyread = Serial.read();
  }



  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);

  double taret = 80;
  scale.tare(taret);
  scale.set_scale(8.60f);

  mtime = micros()/1000000.f;
  force = scale.get_units()*0.009806f;
  dt = 1/80.f;
  dp = force*dt;
  impulse = impulse + dp;
  last = mtime;
  ig = mtime + 10.f;

  pinMode(10, OUTPUT);
  digitalWrite(10, LOW);

  
 Serial.print("Initializing SD card...");
 Serial.println("\n");
 if (!SD.begin(CS)) 
  {
    Serial.println("\n");
    Serial.println("initialization failed!");
    Serial.println("\n");
    while (1);
  }

  Serial.print("Time");
  Serial.print("\t");
  Serial.print("Thrust(N)");
  Serial.print("\t");
  Serial.print("Total-Impulse");
  Serial.println("\n");
    
dataFile = SD.open("THRUST.csv" , FILE_WRITE);

if (dataFile) {
      Serial.print("Writing to THRUST.csv...");
      Serial.println("\n");
      dataFile.println("THRUST MEASUREMENT ");
      dataFile.print("\n");
      dataFile.print("Time");
      dataFile.print("\t");
      dataFile.print("Thrust(N)");
      dataFile.print("\t");
      dataFile.print("Total-Impulse");
      dataFile.print("\n");
      dataFile.close();
    } else {
      Serial.println("error opening file");
      Serial.println("\n");
    }
}

void loop() {

  mtime = micros()/1000000.f;
  force = scale.get_units()*0.009806f;
  dt = mtime-last;
  dp = force*dt;
  impulse = impulse + dp;
  last = mtime;

  
  Serial.println(mtime,10);
  Serial.print("\t");
  Serial.print(force,10);
  Serial.print("\t");
  Serial.print(impulse,10);


dataFile = SD.open("THRUST.csv" , FILE_WRITE);

 if (dataFile) 
 {
  dataFile.print(mtime);
  dataFile.print("\t");
  dataFile.print(force);
  dataFile.print("\t");
  dataFile.print(impulse);
  dataFile.close();
   }
  else
    Serial.println("error opening file");
  //sd card data logging   
  
  if (ig <= mtime && ig + 2 >= mtime){
    digitalWrite(10, HIGH);
  }
  else{
    digitalWrite(10, LOW);
  }
}
