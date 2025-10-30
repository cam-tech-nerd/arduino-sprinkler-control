#include <DS3231.h>
#include <Wire.h>

DS3231 myRTC;
bool century = false;
bool h12Flag = false;
bool pmFlag = false;
bool dayFlag,hourFlag, minuteFlag;
int DofW, Hour, Minute, Second;
int HourStart1, MinuteStop1;
int loopcount;
const int G2 = 8;
const int Ad0 = 7;
const int Ad1 = 6;
const int Ad2 = 5;
unsigned int relay_ch;
unsigned long duration;
unsigned long runtime1, runtime3, runtime4, runtime5;
String command;
String weekday;

void setup() {
    // For Debugging
  Serial.begin(9600);

  // Start the I2C interface
  Wire.begin();

  // put your setup code here, to run once:
  //Setup the 3-to-8 line enable pin (G2)
  pinMode (G2, OUTPUT);
  pinMode (Ad0, OUTPUT);
  pinMode (Ad1, OUTPUT);
  pinMode (Ad2, OUTPUT);
  digitalWrite (G2,LOW);
  digitalWrite (Ad0,LOW);
  digitalWrite (Ad1,LOW);
  digitalWrite (Ad2,LOW);
  relay_ch = 0;
  loopcount = 0;
  HourStart1 = 9;// Start at 9AM
  MinuteStop1 = 10;
  runtime1 = 240000;
  runtime3 = 180000;
  runtime4 = 240000;
  runtime5 = 240000;
  dayFlag = false;
  hourFlag = false;
  minuteFlag = false;
  command = "NULL";
  delay(2000);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (command.equals("NULL")) {
  Serial.println("Enter command:  Test, StartHour, RunTime, Auto  ");

  while (Serial.available() == 0) {
  }

  Serial.print("CMD: ");
  command = Serial.readStringUntil('\n');
  command.trim();
  Serial.println(command);

  while (Serial.available() > 0) {
    Serial.read();
    }
  }

  if (command.equals("Test")) {
    Serial.print("Enter Zone # to test (1,3,4,5): ");

    while (Serial.available() == 0) {
      }

    relay_ch = Serial.parseInt();

    while (Serial.available() > 0) {
      Serial.read();
       }

    Serial.println(relay_ch);


    Serial.print("Duration(milliseconds): ");

    while (Serial.available() == 0) {
      }

    duration = Serial.parseInt();

    while (Serial.available() > 0) {
      Serial.read();
       }

    Serial.println(duration);

    delay(30000);

    switch (relay_ch) {
      case 1:
        digitalWrite (Ad0,LOW);
        digitalWrite (Ad1,LOW);
        digitalWrite (Ad2,LOW);
        digitalWrite (G2,HIGH);
        Serial.println("Testing Zone #1");
        delay(duration);
        Serial.println("Zone #1 testing done!");
        digitalWrite (G2,LOW);
        break;
      case 3:
        digitalWrite (Ad0,HIGH);
        digitalWrite (Ad1,LOW);
        digitalWrite (Ad2,LOW);
        digitalWrite (G2,HIGH);
        Serial.println("Testing Zone #3");
        delay(duration);
        Serial.println("Zone #3 testing done!");
        digitalWrite (G2,LOW);
        break;
      case 4:
        digitalWrite (Ad0,LOW);
        digitalWrite (Ad1,HIGH);
        digitalWrite (Ad2,LOW);
        digitalWrite (G2,HIGH);
        Serial.println("Testing Zone #4");
        delay(duration);
        Serial.println("Zone #4 testing done!");
        digitalWrite (G2,LOW);
        break;
      case 5:
        digitalWrite (Ad0,HIGH);
        digitalWrite (Ad1,HIGH);
        digitalWrite (Ad2,LOW);
        digitalWrite (G2,HIGH);
        Serial.println("Testing Zone #5");
        delay(duration);
        Serial.println("Zone #5 testing done!");
        digitalWrite (G2,LOW);
        break;
      default:
        Serial.println("Incorrect Zone Entered");
        digitalWrite (G2,LOW);
         break;
        }
    command = "NULL";
  }

// Enter the starting hour of the automatic sprinkler cycle
  if (command.equals("StartHour")) {

    Serial.println("Enter the starting hour of the automatic sprinkler cycle (1 -> 23):");

      while (Serial.available() == 0) {
        }

    HourStart1 = Serial.parseInt();

    while (Serial.available() > 0) {
      Serial.read();
       }

    Serial.println("New Start Hour Accepted");

    command = "NULL";
  } 

// Enter the running time for each sprinkler zone in automatic mode
  if (command.equals("RunTime")) {

    Serial.println("Enter the running time for each sprinkler zone in automatic mode (milliseconds) :");
    Serial.print ("Running time for Zone 1: ");

      while (Serial.available() == 0) {
        }

    runtime1 = Serial.parseInt();

    while (Serial.available() > 0) {
      Serial.read();
       }

    Serial.println(runtime1);

    Serial.print ("Running time for Zone 3: ");

      while (Serial.available() == 0) {
        }

    runtime3 = Serial.parseInt();

    while (Serial.available() > 0) {
      Serial.read();
       }

    Serial.println(runtime3);

    Serial.print ("Running time for Zone 4: ");

      while (Serial.available() == 0) {
        }

    runtime4 = Serial.parseInt();

    while (Serial.available() > 0) {
      Serial.read();
       }

    Serial.println(runtime4);

    Serial.print ("Running time for Zone 5: ");

      while (Serial.available() == 0) {
        }

    runtime5 = Serial.parseInt();

    while (Serial.available() > 0) {
      Serial.read();
       }

    Serial.println(runtime5);

    Serial.println("New Runtimes Accepted");

    command = "NULL";
  } 

  if (command.equals("Auto")) {

    DofW = myRTC.getDoW();
    switch (DofW) {
      case 1:
        weekday = "Sunday";
        break;
      case 2:
        weekday = "Monday";
        break;
      case 3:
        weekday = "Tuesday";
        break;
      case 4:
        weekday = "Wednesday";
        break;
      case 5:
        weekday = "Thursday";
        break;
      case 6:
        weekday = "Friday";
        break;
      case 7:
        weekday = "Saturday";
        break; 
      default:
        break;
      }

    Hour = myRTC.getHour(h12Flag, pmFlag);
    Minute = myRTC.getMinute();
    if ((DofW == 1)||(DofW ==3)||(DofW == 5)||(DofW == 7)){
      dayFlag = true;
      }
      else {dayFlag = false;}

    if (Hour == HourStart1)
    {hourFlag = true;}
      else {hourFlag = false;}

    if (Minute < MinuteStop1)
      {minuteFlag = true;}
    else 
      {minuteFlag = false;}

      if ((dayFlag == true) && (hourFlag == true) && (minuteFlag == true)) {
//Ad[0,0,0] = Relay Channel 0; (Sprinkler Zone 1)
        digitalWrite (Ad0,LOW);
        digitalWrite (Ad1,LOW);
        digitalWrite (Ad2,LOW);
  //Enable Relay
        digitalWrite (G2,HIGH);

        Serial.print("Sprinkler Zone 1 ON");
        Serial.println();

        delay(runtime1);
    //Disable Relay
        digitalWrite (G2,LOW);
        Serial.print("Sprinkler Zone 1 OFF");
        Serial.println();
        delay(5000);//5 second delay

//Ad[0,0,1] = Relay Channel 1; (Sprinkler Zone 3)  
        digitalWrite (Ad0,HIGH);
        digitalWrite (Ad1,LOW);
        digitalWrite (Ad2,LOW);
// Enable Relay
        digitalWrite (G2,HIGH);
        Hour = myRTC.getHour(h12Flag, pmFlag);
        Minute = myRTC.getMinute();
        Serial.print(Hour, DEC);
        Serial.print(" ");
        Serial.print(Minute, DEC);
        Serial.println();

        Serial.print("Sprinkler Zone 3 ON");  
        Serial.println();

        delay(runtime3);
//  Disable Relay
        digitalWrite (G2,LOW);

        Serial.print("Sprinkler Zone 3 OFF");
        Serial.println();
        delay(5000);

//Ad[0,1,0] = Relay Channel 2; (Sprinkler Zone 4)  
        digitalWrite (Ad0,LOW);
        digitalWrite (Ad1,HIGH);
        digitalWrite (Ad2,LOW);
// Enable Relay
        digitalWrite (G2,HIGH);

        Hour = myRTC.getHour(h12Flag, pmFlag);
        Minute = myRTC.getMinute();
       Serial.print(Hour, DEC);
        Serial.print(" ");
        Serial.print(Minute, DEC);
        Serial.println();

        Serial.print("Sprinkler Zone 4 ON");  
        Serial.println();

        delay(runtime4);

        digitalWrite (G2,LOW);

        Serial.print("Sprinkler Zone 4 OFF");
        Serial.println();
        delay(5000);

//Ad[0,1,1] = Relay Channel 3; (Sprinkler Zone 5)  
        digitalWrite (Ad0,HIGH);
        digitalWrite (Ad1,HIGH);
        digitalWrite (Ad2,LOW);
// Enable Relay
        digitalWrite (G2,HIGH);

        Hour = myRTC.getHour(h12Flag, pmFlag);
       Minute = myRTC.getMinute();
        Serial.print(Hour, DEC);
        Serial.print(" ");
        Serial.print(Minute, DEC);
        Serial.println();

        Serial.print("Sprinkler Zone 5 ON");  
        Serial.println();

        delay(runtime5);
// Disable Relay
        digitalWrite (G2,LOW);

        Serial.print("Sprinkler Zone 5 OFF");
        Serial.println();
        delay(5000);

      }      
  Serial.println("Running in Auto Mode");
  Hour = myRTC.getHour(h12Flag, pmFlag);
  Minute = myRTC.getMinute();
  Serial.print(Hour, DEC);
  Serial.print(" ");
  Serial.print(Minute, DEC);
  Serial.println();
  delay(300000);

//    command = "NULL";
  }
}

