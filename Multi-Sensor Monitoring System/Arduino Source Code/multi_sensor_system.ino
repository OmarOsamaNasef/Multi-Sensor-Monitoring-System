#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
SoftwareSerial BT(10, 11); // RX, TX

int flamePin = 8;
int buz = 4;
int redLed = 5;
int yellowLed = 6;
int greenLed = 7;
int waterPin = A0;

int trig = 3;
int echo = 2;

int mode = 0; // 0 = OFF, 1 = Flame, 2 = Ultrasonic, 3 = Water

long time;
float dis;

void setup() {
  Serial.begin(9600);
  BT.begin(9600);

  pinMode(flamePin, INPUT);
  pinMode(buz, OUTPUT);
  pinMode(redLed, OUTPUT);
  pinMode(yellowLed, OUTPUT);
  pinMode(greenLed, OUTPUT);

  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);

  lcd.init();
  lcd.backlight();

  lcd.print("Bluetooth Ready");
  delay(1000);
  lcd.clear();
}

void loop() {
  // استقبال أوامر البلوتوث
  if (BT.available()) {
    char cmd = BT.read();

    if (cmd == '1') mode = 1;
    else if (cmd == '2') mode = 2;
    else if (cmd == '3') mode = 3;
    else if (cmd == '0') mode = 0;

    lcd.clear();
  }

  // تنفيذ المود
  if (mode == 1) Flame();
  else if (mode == 2) Ultrasonic();
  else if (mode == 3) Water();
  else Idle();
}

// ================= Flame =================
void Flame() {
  lcd.setCursor(0,0);
  lcd.print("Mode: Flame   ");

  int flame = digitalRead(flamePin);

  if (flame == HIGH) {
    lcd.setCursor(0,1);
    lcd.print("FIRE ALERT!!! ");

    digitalWrite(buz, HIGH);
    digitalWrite(redLed, HIGH);
    digitalWrite(greenLed, LOW);

    delay(100) ;
  } else {
    lcd.setCursor(0,1);
    lcd.print("Safe          ");

    digitalWrite(buz, LOW);
    digitalWrite(redLed, LOW);
    digitalWrite(greenLed, HIGH);
  }
}

// ================= Ultrasonic =================
void Ultrasonic() {
  lcd.setCursor(0,0);
  lcd.print("Mode: Distance");

  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  time = pulseIn(echo, HIGH, 20000);
  dis = 0.017 * time;

  lcd.setCursor(0,1);
  lcd.print("Dist: ");
  lcd.print(dis);
  lcd.print("cm   ");

  if (dis < 5) {
    digitalWrite(buz, HIGH);
    digitalWrite(redLed, HIGH);
            digitalWrite(yellowLed,LOW);
    digitalWrite(greenLed, LOW);
  } else if (dis < 10) {
        digitalWrite(yellowLed,HIGH);
        digitalWrite(redLed,LOW);
    digitalWrite(greenLed, LOW);

    digitalWrite(buz, HIGH);
    delay(100);
    digitalWrite(buz, LOW);
  } else {
    digitalWrite(buz, LOW);
    digitalWrite(greenLed, HIGH);
        digitalWrite(yellowLed, LOW);
        digitalWrite(redLed, LOW);

  }
}

// ================= Water =================
void Water() {
  lcd.setCursor(0,0);
  lcd.print("Mode: Water   ");

  int val = analogRead(waterPin);

  lcd.setCursor(0,1);
  lcd.print("Val: ");
  lcd.print(val);
  lcd.print("    ");

  if (val < 150) {
    digitalWrite(redLed, HIGH);
        digitalWrite(yellowLed, LOW);
    digitalWrite(greenLed, LOW);

  } else if (val < 400) {
    digitalWrite(yellowLed, HIGH);
    digitalWrite(redLed, LOW);
    digitalWrite(greenLed, LOW);
  } else {
    digitalWrite(greenLed, HIGH);
        digitalWrite(redLed, LOW);
    digitalWrite(yellowLed, LOW);

  }
}

// ================= Idle =================
void Idle() {
  lcd.setCursor(0,0);
  lcd.print("ًPlease Give    ");
  lcd.setCursor(0,1);
  lcd.print("US Bouns  ");

  digitalWrite(buz, LOW);
  digitalWrite(redLed, LOW);
  digitalWrite(yellowLed, LOW);
  digitalWrite(greenLed, LOW);
}
