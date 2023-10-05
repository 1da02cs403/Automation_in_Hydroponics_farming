#define SensorPin 0  
const int trigPin = 8;
const int echoPin = 9;
const int buzzer = 11;
// the pH meter Analog output is connected with the Arduino’s Analog
unsigned long int avgValue;  //Store the average value of the sensor feedback
float b;
int buf[10],temp;
//
long duration;
int distance;
int safetyDistance;
#include <SoftwareSerial.h>
SoftwareSerial mySerial(3, 2); 

void setup()
{
  pinMode(13,OUTPUT);  
  Serial.begin(9600);  
  Serial.println("Ready");    //Test the serial monitor
  pinMode(10, OUTPUT);
  digitalWrite(10, HIGH);
  delay(1000);
  //
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  pinMode(buzzer, OUTPUT);
  Serial.begin(9600); // Starts the serial communication
  //
  Serial.begin(9600);
  
  
  mySerial.begin(9600);

  Serial.println("Initializing..."); 
  delay(1000);

  mySerial.println("AT"); 
  updateSerial();

  mySerial.println("AT+CMGF=1"); 
  updateSerial();
  mySerial.println("AT+CMGS=\"+919663580673\"");
  updateSerial();
  mySerial.print("Hydrophonics system"); 
  mySerial.println("pH Value: ");
  mySerial.print("14.00");
  mySerial.println("pH Value: ");
  mySerial.print("13.00");
  mySerial.println("pH Value: ");
  mySerial.print("18.00");
  updateSerial();
  mySerial.write(26);

}
void loop()
{
  for(int i=0;i<10;i++)       //Get 10 sample value from the sensor for smooth the value
  { 
    buf[i]=analogRead(SensorPin);
    delay(10);
  }
  for(int i=0;i<9;i++)        //sort the analog from small to large
  {
    for(int j=i+1;j<10;j++)
    {
      if(buf[i]>buf[j])
      {
        temp=buf[i];
        buf[i]=buf[j];
        buf[j]=temp;
      }
    }
  }
  avgValue=0;
  for(int i=2;i<8;i++)                      //take the average value of 6 center sample
    avgValue+=buf[i];
  float phValue=(float)avgValue*5.0/1024/6; //convert the analog into millivolt
  phValue=3.5*phValue;                      //convert the millivolt into pH value
  Serial.print("    pH:");  
  Serial.print(phValue,2);
  Serial.println(" ");
  if (phValue < 8) 
  {
    digitalWrite(10, HIGH);
    Serial.println("Water Pump is ON ");
  } 
  else 
  {
    digitalWrite(10, LOW);
    Serial.println("Water Pump is OFF ");
  } 

  
  //dist-buzzer
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);

  // Calculating the distance
  distance= duration*0.034/2;

  safetyDistance = distance;
  if (safetyDistance <= 20)
  {
    digitalWrite(buzzer, HIGH);
    Serial.println("Introdure Detected!!");
  }
  else{
    digitalWrite(buzzer, LOW);
  }
  delay(50);

 
}
void updateSerial()
  {
    delay(500);
    while (Serial.available()) 
    {
      mySerial.write(Serial.read());
    }
    while(mySerial.available()) 
   {
     Serial.write(mySerial.read());
   }
}
