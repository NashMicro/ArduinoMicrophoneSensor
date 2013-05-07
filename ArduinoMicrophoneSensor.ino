int mic0Pin = A0;    
int mic1Pin = A1;    
int mic0Value = 0;  // variable to store the value coming from the #1
int mic1Value = 0;  // variable to store the value coming from mic #2
int led0Pin = 12;
int led1Pin = 5;

int mic0Ambient = 0;
int mic1Ambient = 0;
int ambientLoops = 0;

const int AMBIENT_DETECTION_LOOPS = 500;
const int LOOP_DELAY = 10;
const int AMBIENT_THREASHOLD = 2;          // The mic level over the detected ambient

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
  Serial.print(" : ");
  Serial.println(mic1Value);
  
  // The first loops (# defined by AMBIENT_DETECTION_LOOPS) are for detecting the level of ambient noise.
  if(ambientLoops < AMBIENT_DETECTION_LOOPS)
  {
     if(mic0Value > mic0Ambient) mic0Ambient = mic0Value;
     if(mic1Value > mic1Ambient) mic1Ambient = mic1Value;
     ambientLoops++;     
     
     if(ambientLoops == AMBIENT_DETECTION_LOOPS)
     {
       digitalWrite(led0Pin, true);
       digitalWrite(led1Pin, true);
       delay(1000);
       digitalWrite(led0Pin, false);
       digitalWrite(led1Pin, false);
     }
  }

  if(mic0Value > mic0Ambient + AMBIENT_THREASHOLD) digitalWrite(led0Pin, true);
  if(mic1Value > mic1Ambient + AMBIENT_THREASHOLD) digitalWrite(led1Pin, true);
  
  delay(LOOP_DELAY);
  
  if(mic0Value > mic0Ambient + AMBIENT_THREASHOLD) digitalWrite(led0Pin, false);
  if(mic1Value > mic1Ambient + AMBIENT_THREASHOLD) digitalWrite(led1Pin, false);
}
