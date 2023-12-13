#include <Servo.h>
#include <U8glib.h>

int led = 13; // define the LED pin
int reelSwitch = 2; // magnetic senso rpin
int switchState; // variable to store reel switch value
int miniReelSwitch = 4; // KY-025 digital interface
int analogPin = A0; // KY-025 analog interface
int miniSwitchState; // digital readings, big reel switch
int miniSwitchStateCorner2;
int miniReelSwitchCorner2 = 7;
int analogVal; //analog readings

U8GLIB_SSD1306_128X64 oled(U8G_I2C_OPT_NO_ACK);


Servo myservofront;
Servo myservoside;
int pos = 0;

int ballPosX = 5;
int ballPosY = 15;

int miniSwitchStateHasBeen = 0;
int switchStateHasBeen = 0;

void setup() {

  oled.setFont(u8g_font_helvB10);
  pinMode(2, INPUT);

  myservofront.attach(9);
  myservoside.attach(8);
  pinMode(led, OUTPUT);
  pinMode (reelSwitch, INPUT);
  pinMode(miniReelSwitch, INPUT);
  //pinMode(analogPin, OUTPUT);
  Serial.begin(9600);
}

void loop()
{
  updateServo();

  updateOled(ballPosX, ballPosY);

}

void updateOled(int ballPosX, int ballPosY) {

  oled.firstPage();
  do {
    oled.drawLine(0, 5, 80, 5);
    oled.drawLine(80, 5, 80, 25);
    oled.drawLine(80, 25, 127, 25);
    oled.drawLine(0, 25, 60, 25);
    oled.drawLine(60, 25, 60, 45);
    oled.drawLine(60, 45, 127, 45);
    //Serial.println (ballPosX);
    //Serial.println (ballPosY);
    oled.drawCircle(ballPosX, ballPosY, 5);
  } while (oled.nextPage());
}

void updateServo() {
  switchState = digitalRead(reelSwitch);
  miniSwitchState = digitalRead(miniReelSwitch);
  miniSwitchStateCorner2 = digitalRead(miniReelSwitchCorner2);
  if (switchState == LOW) {
    switchStateHasBeen += 1;
    Serial.print(miniSwitchStateHasBeen);
    if (miniSwitchStateHasBeen > 0) {
      resetSlowServo(myservoside, 60);
    }
    slowServo(myservofront, 120);
    ballPosX = 70;
    ballPosY = 35;
    delay(3000);
  }
  else if (miniSwitchState == HIGH) {
    miniSwitchStateHasBeen += 1;
    if (switchStateHasBeen > 0) {
      resetSlowServo(myservofront, 120);
    }
    slowServo(myservoside, 60);
    ballPosX = 70;
    ballPosY = 15;
    delay(3000);
  }
  else if (miniSwitchStateCorner2 == LOW) {
    miniSwitchStateHasBeen += 1;
    if (switchStateHasBeen > 0) {
      resetSlowServo(myservofront, 120);
    }
    slowServo(myservoside, 60);
    ballPosX = 70;
    ballPosY = 35;
    delay(3000);
    ballPosX = 120;
    ballPosY = 35;
  }
  /*else {
    pos = 90;
    myservofront.write(pos);
    pos = 90;
    myservoside.write(pos);
    }*/



  /* for (pos = 0; pos <= 70; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
    }
    for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
    }
  */
  // Read the digital interface

  if (miniSwitchState == HIGH or switchState == LOW) // if magnetic field is detected
  {
    digitalWrite(led, HIGH); // turn ON Arduino's LED
  }
  else
  {
    digitalWrite(led, LOW); // turn OFF Arduino's LED
  }
  // Read the analog interface
  analogVal = analogRead(analogPin);
  //Serial.println(analogVal); // print analog value to serial
  delay(100);
}

void slowServo(Servo servo, int pos) {
  for (int i = 10; i >= 1; i--) {
    servo.write(((i * ((90 - pos) / 10)) + pos));
    //serial.Print
    delay(100);
  }
  /*if (played > 0) {
    for (int i = 1; i <= 1; i = i + 1) {
      servo.write(((i * ((90 - pos) / 10)) + pos));
      //serial.Print
      delay(100);
    }
  }*/
}

void resetSlowServo(Servo servo, int pos) {
  for (int i = 1; i <= 10; i++) {
    servo.write(((i * ((90 - pos) / 10)) + pos));
    //serial.Print
    delay(100);
  }
}
