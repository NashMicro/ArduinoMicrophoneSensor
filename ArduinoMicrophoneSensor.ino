#include <Average.h>

int mic0Pin = A0;    
int mic1Pin = A1;    
int mic0Value = 0;  // variable to store the value coming from the #1
int mic1Value = 0;  // variable to store the value coming from mic #2
int led0Pin = 12;
int led1Pin = 5;

int mic0Ambient = 0;
int mic1Ambient = 0;
int ambientLoops = 0;

const int AMBIENT_DETECTION_LOOPS = 300;
const int LOOP_DELAY = 10;
const int AMBIENT_THREASHOLD = 3;          // The mic level over the detected ambient

int mic0AmbientVals[AMBIENT_DETECTION_LOOPS];
int mic1AmbientVals[AMBIENT_DETECTION_LOOPS];

void setup() {
  Serial.begin(9600);
  pinMode(led0Pin, OUTPUT);  
  pinMode(led1Pin, OUTPUT);  
}

void loop() {
  // read the value from the sensor:
  mic0Value = analogRead(mic0Pin);    
  mic1Value = analogRead(mic1Pin);  
  
  Serial.print(mic0Value);
  Serial.print(" (");
  Serial.print(mic0Ambient);
  Serial.print(")");
  Serial.print(" : ");
  Serial.print(mic1Value);
  Serial.print(" (");
  Serial.print(mic1Ambient);
  Serial.println(")");

  // The first loops (# defined by AMBIENT_DETECTION_LOOPS) are for detecting the level of ambient noise.
  if(ambientLoops < AMBIENT_DETECTION_LOOPS)
  {
     mic0AmbientVals[ambientLoops] = mic0Value;
     mic1AmbientVals[ambientLoops] = mic1Value;
         
     /*    
     Serial.print("Reading  ");
     Serial.print(mic0AmbientVals[ambientLoops]);
     Serial.print("  :  ");
     Serial.println(mic1AmbientVals[ambientLoops]);
     */
     
     if(ambientLoops == AMBIENT_DETECTION_LOOPS-1)
     {
       Serial.println("Detecting ambient.");
       long sum0 = 0;
       long sum1 = 0;
       const int STD_DEVS = 1;
              
       int mic0StdDev = stddev(mic0AmbientVals, AMBIENT_DETECTION_LOOPS);
       int mic1StdDev = stddev(mic1AmbientVals, AMBIENT_DETECTION_LOOPS);
     
       int mic0Avg = mean(mic0AmbientVals, AMBIENT_DETECTION_LOOPS);
       int mic1Avg = mean(mic1AmbientVals, AMBIENT_DETECTION_LOOPS);
       
       int mic0AvgCnt = 0;
       int mic1AvgCnt = 0;
       
       for(int x = 0; x < AMBIENT_DETECTION_LOOPS; x++)
       {
           if(abs(mic0AmbientVals[x] - mic0Avg) <  mic0StdDev * STD_DEVS)
           {
             sum0 += mic0AmbientVals[x];
             mic0AvgCnt++;
           }
           if(abs(mic1AmbientVals[x] - mic1Avg) <  mic1StdDev * STD_DEVS)
           {
             sum1 += mic1AmbientVals[x];
             mic1AvgCnt++;
           }
           Serial.println(sum1);
       }
       mic0Ambient = sum0 / mic0AvgCnt;
       mic1Ambient = sum1 / mic1AvgCnt;
       digitalWrite(led0Pin, true);
       digitalWrite(led1Pin, true);
       delay(1000);
       digitalWrite(led0Pin, false);
       digitalWrite(led1Pin, false);  
     }
     ambientLoops++;   
  }

  if(mic0Value > mic0Ambient + AMBIENT_THREASHOLD) digitalWrite(led0Pin, true);
  if(mic1Value > mic1Ambient + AMBIENT_THREASHOLD) digitalWrite(led1Pin, true);
  
  delay(LOOP_DELAY);
  
  if(mic0Value > mic0Ambient + AMBIENT_THREASHOLD) digitalWrite(led0Pin, false);
  if(mic1Value > mic1Ambient + AMBIENT_THREASHOLD) digitalWrite(led1Pin, false);
}
