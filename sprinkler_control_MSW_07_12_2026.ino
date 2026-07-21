
#include <DS3231.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

//I2C pins declaration
LiquidCrystal_I2C lcd(0x27, 16, 2);

DS3231 myRTC;
bool pmFlag;
//bool hourFlag, dayFlag1;
//bool minuteFlag;
byte year, month, date, dofw, hour, minute, second;
byte HourStart1, MinuteStart1;

const byte G2 = 8;
const byte Ad0 = 7;
const byte Ad1 = 6;
const byte Ad2 = 5;
byte relay_ch;

char buffer[10];
String command;
String weekday;

void setup() {
  // Start the Serial interface
  Serial.begin(9600);

  // Start the I2C interface
  Wire.begin();

  // Initialize the LCD
  lcd.init();
  lcd.init();
  lcd.backlight();
  lcd.clear();

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
  HourStart1 = 19;// Start at 7:45PM
  MinuteStart1 = 45;
//  dayFlag1 = false;
//  hourFlag = false;
//  minuteFlag = false;
  command = "NULL";
  delay(500);
}

void loop() {
  int runtime1 = 300;// 5 minutes
  int runtime2 = 180;// 3 minutes
  int runtime3 = 240;// 4 minutes
  int runtime4 = 240;// 4 minutes
  int runtime5 = 240;// 4 minutes
  int runtime6 = 240;// 4 minutes
  int runtime7 = 240;// 4 minutes

  unsigned long duration = 0;
  bool century = false;
  bool h12Flag = false;
  bool minuteFlag = false;
  bool hourFlag = false;
  bool dayFlag1 = false;

  myRTC.setClockMode(h12Flag);

  if (command.equals("NULL")) {
    Serial.println(F("Enter command:  Test, StartTime, RunTime, Auto  "));

    while (Serial.available() == 0) {
      dofw = myRTC.getDoW();
      date = myRTC.getDate();
      hour = myRTC.getHour(h12Flag, pmFlag);
      minute = myRTC.getMinute();
      year = myRTC.getYear();
      month = myRTC.getMonth(century);

      lcd.setCursor(0,0);
      switch (dofw) {
       case 1:
          lcd.print("Sunday   ");
         break;
        case 2:
          lcd.print("Monday   ");
          break;
        case 3:
          lcd.print("Tuesday  ");
          break;
        case 4:
          lcd.print("Wednesday");
          break;
        case 5:
          lcd.print("Thursday ");
          break;
        case 6:
          lcd.print("Friday   ");
          break;
        case 7:
          lcd.print("Saturday ");
          break; 
        default:
        dayFlag1 = false;
         break;
        }
      lcd.setCursor(0,1);
      sprintf(buffer, "%02d", month);
      lcd.print(buffer);
      lcd.setCursor(2,1);
      lcd.print("/");

      lcd.setCursor(3,1);
      sprintf(buffer, "%02d", date);
      lcd.print(buffer);
      lcd.setCursor(5,1);
      lcd.print("/");

      lcd.setCursor(6,1);
      sprintf(buffer, "%02d", year);
      lcd.print(buffer);

      lcd.setCursor(9,1);
      sprintf(buffer, "%02d", hour);
      lcd.print(buffer);
      lcd.setCursor(11,1);
      lcd.print(":");

      lcd.setCursor(12,1);
      sprintf(buffer, "%02d", minute);
      lcd.print(buffer);
      delay(1000);
      }

    Serial.print(F("CMD: "));
    command = Serial.readStringUntil('\n');
    command.trim();
    Serial.println(command);

    while (Serial.available() > 0) {
     Serial.read();
     }
   }
    else if (command.equals("Test")) {
    Serial.print(F("Enter Zone # to test (1,2,3,4,5,6,7): "));

    while (Serial.available() == 0) {
      }

    relay_ch = Serial.parseInt();

    while (Serial.available() > 0) {
      Serial.read();
       }

    Serial.println(relay_ch);


    Serial.print(F("Duration(seconds): "));

    while (Serial.available() == 0) {
      }

    duration = Serial.parseInt();


    while (Serial.available() > 0) {
      Serial.read();
       }

    Serial.println(duration);
    duration = (long)duration * 1000;
    delay(10000);

    switch (relay_ch) {
      case 1: //"Andrea's Garden" and Front Lawn
        digitalWrite (Ad0,LOW);
        digitalWrite (Ad1,LOW);
        digitalWrite (Ad2,LOW);
        digitalWrite (G2,HIGH);
        Serial.println(F("Testing Zone #1"));
        delay(duration);
        Serial.println(F("Zone #1 testing done!"));
        digitalWrite (G2,LOW);
        break;
      case 2: //Front Lawn by the street
        digitalWrite (Ad0,HIGH);
        digitalWrite (Ad1,LOW);
        digitalWrite (Ad2,LOW);
        digitalWrite (G2,HIGH);
        Serial.println(F("Testing Zone #2"));
        delay(duration);
        Serial.println(F("Zone #2 testing done!"));
        digitalWrite (G2,LOW);
        break;
      case 3: //Front bushes on side of driveway / Peach and Apricot trees / Flowers near wooden steps
        digitalWrite (Ad0,LOW);
        digitalWrite (Ad1,HIGH);
        digitalWrite (Ad2,LOW);
        digitalWrite (G2,HIGH);
        Serial.println(F("Testing Zone #3"));
        delay(duration);
        Serial.println(F("Zone #3 testing done!"));
        digitalWrite (G2,LOW);
        break;
      case 4: // Bushes on LEFT side of concrete steps
        digitalWrite (Ad0,HIGH);
        digitalWrite (Ad1,HIGH);
        digitalWrite (Ad2,LOW);
        digitalWrite (G2,HIGH);
        Serial.println(F("Testing Zone #4"));
        delay(duration);
        Serial.println(F("Zone #4 testing done!"));
        digitalWrite (G2,LOW);
        break;
      case 5: // Bushes on RIGHT side of concrete steps
        digitalWrite (Ad0,LOW);
        digitalWrite (Ad1,LOW);
        digitalWrite (Ad2,HIGH);
        digitalWrite (G2,HIGH);
        Serial.println(F("Testing Zone #5"));
        delay(duration);
        Serial.println(F("Zone #5 testing done!"));
        digitalWrite (G2,LOW);
        break;
      case 6: // Backyard bushes / down slope veggies / strawberry tubs / Citrus tubs / Raided wooden bed planter
        digitalWrite (Ad0,HIGH);
        digitalWrite (Ad1,LOW);
        digitalWrite (Ad2,HIGH);
        digitalWrite (G2,HIGH);
        Serial.println(F("Testing Zone #6"));
        delay(duration);
        Serial.println(F("Zone #6 testing done!"));
        digitalWrite (G2,LOW);
        break;
      case 7: // Backyard wildflower garden / Citrus trees in pots
        digitalWrite (Ad0,LOW);
        digitalWrite (Ad1,HIGH);
        digitalWrite (Ad2,HIGH);
        digitalWrite (G2,HIGH);
        Serial.println(F("Testing Zone #7"));
        delay(duration);
        Serial.println(F("Zone #7 testing done!"));
        digitalWrite (G2,LOW);
        break;
      default:
        Serial.println(F("Incorrect Zone Entered"));
        digitalWrite (G2,LOW);
         break;
        }
    command = "NULL";
  }

// Enter the starting hour of the automatic sprinkler cycle
   else if (command.equals("StartTime")) {

    Serial.println(F("Enter the starting hour of the automatic sprinkler cycle (1 -> 23):"));

      while (Serial.available() == 0) {
        }

    HourStart1 = Serial.parseInt();

    while (Serial.available() > 0) {
      Serial.read();
       }

    Serial.println(F("Enter the starting minute of the automatic sprinkler cycle (1 -> 23):"));

      while (Serial.available() == 0) {
        }

    MinuteStart1 = Serial.parseInt();

    while (Serial.available() > 0) {
      Serial.read();
       }

    Serial.println(F("New Start Hour/Minute Accepted"));

    command = "NULL";
  } 

// Enter the running time for each sprinkler zone in automatic mode
/*  else if (command.equals("RunTime")) {

    Serial.println("Enter the running time for each sprinkler zone in automatic mode (milliseconds) :");
//
    Serial.print ("Running time for Zone 1: ");
      while (Serial.available() == 0) {
        }
    runtime1 = Serial.parseInt();

    while (Serial.available() > 0) {
      Serial.read();
       }
    Serial.println(runtime1);
//
    Serial.print ("Running time for Zone 2: ");
      while (Serial.available() == 0) {
        }
    runtime2 = Serial.parseInt();

    while (Serial.available() > 0) {
      Serial.read();
       }
    Serial.println(runtime2);
//
    Serial.print ("Running time for Zone 3: ");
      while (Serial.available() == 0) {
        }
    runtime3 = Serial.parseInt();

    while (Serial.available() > 0) {
      Serial.read();
       }
    Serial.println(runtime3);
//
    Serial.print ("Running time for Zone 4: ");
      while (Serial.available() == 0) {
        }
    runtime4 = Serial.parseInt();

    while (Serial.available() > 0) {
      Serial.read();
       }
    Serial.println(runtime4);
//
    Serial.print ("Running time for Zone 5: ");
      while (Serial.available() == 0) {
        }
    runtime5 = Serial.parseInt();

    while (Serial.available() > 0) {
      Serial.read();
       }
    Serial.println(runtime5);
//
    Serial.print ("Running time for Zone 6: ");
      while (Serial.available() == 0) {
        }
    runtime6 = Serial.parseInt();

    while (Serial.available() > 0) {
      Serial.read();
       }
    Serial.println(runtime6);

    Serial.println("New Runtimes Accepted");

    command = "NULL";
  } 
*/
   else if (command.equals("Auto")) {

    year = myRTC.getYear();
    month = myRTC.getMonth(century);
    date = myRTC.getDate();
    dofw = myRTC.getDoW();
    hour = myRTC.getHour(h12Flag, pmFlag);
    minute = myRTC.getMinute();

     switch (dofw) {
      case 1: //Sunday
      dayFlag1 = true;
      break;

    case 2: //Monday
      dayFlag1 = true;
      break;

    case 3: //Tuesday
      dayFlag1 = true;
      break;

    case 4: //Wednesday
      dayFlag1 = true;
      break;

    case 5: //Thursday
      dayFlag1 = true;
      break;

    case 6: //Friday
      dayFlag1 = true;
      break;

    case 7: //Saturday
      dayFlag1 = true;
      break;

    default:
    dayFlag1 = false;
      break;
    }

    lcd.setCursor(0,0);
    lcd.print("AUTO MODE    ");
    lcd.setCursor(0,1);
    sprintf(buffer, "%02d", month);
    lcd.print(buffer);
    lcd.setCursor(2,1);
    lcd.print("/");

    lcd.setCursor(3,1);
    sprintf(buffer, "%02d", date);
    lcd.print(buffer);
    lcd.setCursor(5,1);
    lcd.print("/");

    lcd.setCursor(6,1);
    sprintf(buffer, "%02d", year);
    lcd.print(buffer);

    lcd.setCursor(9,1);
    sprintf(buffer, "%02d", hour);
    lcd.print(buffer);
    lcd.setCursor(11,1);
    lcd.print(":");

    lcd.setCursor(12,1);
    sprintf(buffer, "%02d", minute);
    lcd.print(buffer);

    lcd.setCursor(15,1);
    sprintf(buffer, "%01d", dayFlag1);
    lcd.print(buffer);

    Serial.print("Hour = ");
    Serial.println(hour);

    delay(1000);


    if (hour == HourStart1)
      {hourFlag = true;}
      else {hourFlag = false;}

    if (minute == MinuteStart1)
      {minuteFlag = true;}

      if ((dayFlag1 == true) && (hourFlag == true) && (minuteFlag == true) ) {
//Ad[0,0,0] = Relay Channel 0 = Sprinkler Zone 1
        digitalWrite (Ad0,LOW);
        digitalWrite (Ad1,LOW);
        digitalWrite (Ad2,LOW);
  //Enable Relay
        digitalWrite (G2,HIGH);
        hour = myRTC.getHour(h12Flag, pmFlag);
        minute = myRTC.getMinute();
        Serial.print(F("Sprinkler Zone 1 ON"));
        Serial.println();
        Serial.print(hour, DEC);
        Serial.print(" ");
        Serial.print(minute, DEC);
        Serial.println();
        duration = (long) runtime1 * 1000;
        delay((duration));
    //Disable Relay
        digitalWrite (G2,LOW);
        Serial.print(F("Sprinkler Zone 1 OFF"));
        Serial.println();
        delay(5000);//5 second delay

//Ad[0,0,1] = Relay Channel 1 = Sprinkler Zone 2
        digitalWrite (Ad0,HIGH);
        digitalWrite (Ad1,LOW);
        digitalWrite (Ad2,LOW);
  //Enable Relay
        digitalWrite (G2,HIGH);
        hour = myRTC.getHour(h12Flag, pmFlag);
        minute = myRTC.getMinute();
        Serial.print(F("Sprinkler Zone 2 ON"));
        Serial.println();
        Serial.print(hour, DEC);
        Serial.print(" ");
        Serial.print(minute, DEC);
        Serial.println();
        duration = (long) runtime2 * 1000;
        delay(duration);
    //Disable Relay
        digitalWrite (G2,LOW);
        Serial.print(F("Sprinkler Zone 2 OFF"));
        Serial.println();
        delay(5000);//5 second delay

//Ad[0,1,0] = Relay Channel 2 = Sprinkler Zone 3
        digitalWrite (Ad0,LOW);
        digitalWrite (Ad1,HIGH);
        digitalWrite (Ad2,LOW);
  //Enable Relay
        digitalWrite (G2,HIGH);
        hour = myRTC.getHour(h12Flag, pmFlag);
        minute = myRTC.getMinute();
        Serial.print(F("Sprinkler Zone 3 ON"));
        Serial.println();
        Serial.print(hour, DEC);
        Serial.print(" ");
        Serial.print(minute, DEC);
        Serial.println();
        duration = (long) runtime3 * 1000;
        delay(duration);
    //Disable Relay
        digitalWrite (G2,LOW);
        Serial.print(F("Sprinkler Zone 3 OFF"));
        Serial.println();
        delay(5000);//5 second delay

//Ad[0,1,1] = Relay Channel 3 = Sprinkler Zone 4
        digitalWrite (Ad0,HIGH);
        digitalWrite (Ad1,HIGH);
        digitalWrite (Ad2,LOW);
  //Enable Relay
        digitalWrite (G2,HIGH);
        hour = myRTC.getHour(h12Flag, pmFlag);
        minute = myRTC.getMinute();
        Serial.print(F("Sprinkler Zone 4 ON"));
        Serial.println();
        Serial.print(hour, DEC);
        Serial.print(" ");
        Serial.print(minute, DEC);
        Serial.println();
        duration = (long) runtime4 * 1000;
        delay(duration);
    //Disable Relay
        digitalWrite (G2,LOW);
        Serial.print(F("Sprinkler Zone 4 OFF"));
        Serial.println();
        delay(5000);//5 second delay

//Ad[1,0,0] = Relay Channel 4 = Sprinkler Zone 5
        digitalWrite (Ad0,LOW);
        digitalWrite (Ad1,LOW);
        digitalWrite (Ad2,HIGH);
  //Enable Relay
        digitalWrite (G2,HIGH);
        hour = myRTC.getHour(h12Flag, pmFlag);
        minute = myRTC.getMinute();
        Serial.print(F("Sprinkler Zone 5 ON"));
        Serial.println();
        Serial.print(hour, DEC);
        Serial.print(" ");
        Serial.print(minute, DEC);
        Serial.println();
        duration = (long) runtime5 * 1000;
        delay(duration);
    //Disable Relay
        digitalWrite (G2,LOW);
        Serial.print(F("Sprinkler Zone 5 OFF"));
        Serial.println();
        delay(5000);//5 second delay

//Ad[1,0,1] = Relay Channel 5 = Sprinkler Zone 6
        digitalWrite (Ad0,HIGH);
        digitalWrite (Ad1,LOW);
        digitalWrite (Ad2,HIGH);
  //Enable Relay
        digitalWrite (G2,HIGH);
        hour = myRTC.getHour(h12Flag, pmFlag);
        minute = myRTC.getMinute();
        Serial.print(F("Sprinkler Zone 6 ON"));
        Serial.println();
        Serial.print(hour, DEC);
        Serial.print(" ");
        Serial.print(minute, DEC);
        Serial.println();
        duration = (long) runtime6 * 1000;
        delay(duration);
    //Disable Relay
        digitalWrite (G2,LOW);
        Serial.print(F("Sprinkler Zone 6 OFF"));
        Serial.println();
        delay(5000);//5 second delay;

//Ad[1,1,0] = Relay Channel 6 = Sprinkler Zone 7
        digitalWrite (Ad0,LOW);
        digitalWrite (Ad1,HIGH);
        digitalWrite (Ad2,HIGH);
  //Enable Relay
        digitalWrite (G2,HIGH);
        hour = myRTC.getHour(h12Flag, pmFlag);
        minute = myRTC.getMinute();
        Serial.print(F("Sprinkler Zone 7 ON"));
        Serial.println();
        Serial.print(hour, DEC);
        Serial.print(" ");
        Serial.print(minute, DEC);
        Serial.println();
        duration = (long) runtime6 * 1000;
        delay(duration);
    //Disable Relay
        digitalWrite (G2,LOW);
        Serial.print(F("Sprinkler Zone 7 OFF"));
        Serial.println();
        delay(5000);//5 second delay
        minuteFlag = false;
      }
  }

}


