// including libraries
#include <Servo.h>
#include <U8glib.h>

int led = 13; // define the LED pin
int reelSwitch = 2; // magnetic senso rpin
int switchState; // variable to store reel switch value
int miniReelSwitch = 4; // define minireelswitch pin
int miniSwitchState; // variable to store minireel switch value
int miniSwitchStateCorner2; // variable to store minireel switch corner2 value
int miniReelSwitchCorner2 = 7; // define minireelswitchcorner2 pin

U8GLIB_SSD1306_128X64 oled(U8G_I2C_OPT_NO_ACK); // object for led display


Servo myservofront; // initiate front servo, which rotates on one axis
Servo myservoside; // initiate side servo, which rotates on the other axis
int pos = 0; // initiate position variable for servos position

int ballPosX = 5; // initiate ballposition for led-display in x axis
int ballPosY = 15; // initiate ballposition for led-display in  yaxis

int miniSwitchStateHasBeen = 0; // initiate miniSwitchStateHasBeen variable to count number of times the magnet sensor has been activated
int switchStateHasBeen = 0; // initiate switchStateHasBeen variable to count number of times the magnet sensor has been activated

// setup everything, attach servos to pins
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
//main loop, updates servo depending on the magnetsensors value. Also updates the display with the balls position in the x and y axis.
void loop()
{
  updateServo();

  updateOled(ballPosX, ballPosY);

}
// function for updating the display, depending on the balls position
void updateOled(int ballPosX, int ballPosY) {

  oled.firstPage(); // resets the screen
  do {
    oled.drawLine(0, 5, 80, 5);
    oled.drawLine(80, 5, 80, 25);
    oled.drawLine(80, 25, 127, 25);
    oled.drawLine(0, 25, 60, 25);  // draws the barriers of the track
    oled.drawLine(60, 25, 60, 45); 
    oled.drawLine(60, 45, 127, 45);
    //Serial.println (ballPosX);
    //Serial.println (ballPosY);
    oled.drawCircle(ballPosX, ballPosY, 5); // draws a circle representing the ball depending on its real time position
  } while (oled.nextPage());
}

//function for updating the servo depending on which corner the ball is in
void updateServo() {
  switchState = digitalRead(reelSwitch); // read the reelswitches value, store it in a varible
  miniSwitchState = digitalRead(miniReelSwitch); // read the minireelswitches value, store it in a varible
  miniSwitchStateCorner2 = digitalRead(miniReelSwitchCorner2); // read the second minireelswitches value, store it in a varible
  if (switchState == LOW) { // checks if the big magnetic sensor detects anything, it is set to Low because of the way that it is connected so it is revesered
    switchStateHasBeen += 1; // adds the times it has detected something
    if (miniSwitchStateHasBeen > 0) { // resets the servos position if the other magnetic sensors never has detected anything before
      resetSlowServo(myservoside, 60);
    }
    slowServo(myservofront, 120); //set the wanted position with the slowServo function
    ballPosX = 70; // sets the balls display position
    ballPosY = 35; // sets the balls display position
    delay(3000);
  }
  else if (miniSwitchState == HIGH) { // checks if the second magnetic sensor has detected anything
    miniSwitchStateHasBeen += 1; // adds the times it has detected something
    if (switchStateHasBeen > 0) { // resets the servos position if the other magnetic sensors never has detected anything before
      resetSlowServo(myservofront, 120); 
    }
    slowServo(myservoside, 60); //set the wanted position with the slowServo function
    ballPosX = 70; //sets the balls display position
    ballPosY = 15; //sets the balls display position
    delay(3000);
  }
  else if (miniSwitchStateCorner2 == LOW) { // checks if the big magnetic sensor detects anything, it is set to Low because of the way that it is connected so it is revesered
    miniSwitchStateHasBeen += 1; // adds the times it has detected something
    if (switchStateHasBeen > 0) { // resets the servos position if the other magnetic sensors never has detected anything before
      resetSlowServo(myservofront, 120);
    }
    slowServo(myservoside, 60); //set the wanted position with the slowServo function
    ballPosX = 70; //sets the balls display position
    ballPosY = 35; //sets the balls display position
    delay(3000);
    ballPosX = 120; //sets the balls display position to the last position after 3 seconds because the end doesnt have a sensor
    ballPosY = 35; //sets the balls display position to the last position after 3 seconds because the end doesnt have a sensor
    
  // checks if any of the sensors detect anything, a bit unnecessary for the actual product
  if (miniSwitchState == HIGH or switchState == LOW) // if magnetic field is detected
  {
    digitalWrite(led, HIGH); // turn ON Arduino's LED
  }
  else
  {
    digitalWrite(led, LOW); // turn OFF Arduino's LED
  }
  delay(100);
}

//function for turning the servo slowly, works so that it turns a little bit many times with a delaay between to slow it down
void slowServo(Servo servo, int pos) {
  for (int i = 10; i >= 1; i--) {
    servo.write(((i * ((90 - pos) / 10)) + pos));
    delay(100);
  }
}

//function for resetting the servo slowly, basically the slowServo function but reversed
void resetSlowServo(Servo servo, int pos) {
  for (int i = 1; i <= 10; i++) {
    servo.write(((i * ((90 - pos) / 10)) + pos));
    delay(100);
  }
}
