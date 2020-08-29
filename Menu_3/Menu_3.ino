#include <LiquidCrystal.h>
const int pin_RS = 8; 
const int pin_EN = 9; 
const int pin_d4 = 4; 
const int pin_d5 = 5; 
const int pin_d6 = 6; 
const int pin_d7 = 7; 
const int pin_BL = 10;
LiquidCrystal lcd( pin_RS,  pin_EN,  pin_d4,  pin_d5,  pin_d6,  pin_d7);

//Input & Button Logic
const int numOfInputs = 1;
const int inputPins[numOfInputs] = {0};
int inputState[numOfInputs];
int lastInputState[numOfInputs] = {0};
bool inputFlags[numOfInputs] = {LOW};
long lastDebounceTime[numOfInputs] = {0};
long debounceDelay = 5;

//LCD Menu Logic
const int numOfScreens = 4;
int currentScreen = 0;
String screens[numOfScreens][2] = {{"Temperatura","C"}, {"Tiempo", "Min"}, 
{"Iniciar ciclo","Si"},{"Uso manual","Si"}};
int parameters[numOfScreens] = { 50, 10, 0, 0} ;//{Temperatura,tiempo,ciclo auto,ciclo manual};

void setup() {
  /*for(int i = 0; i < numOfInputs; i++) {
    pinMode(inputPins[i], INPUT);
    digitalWrite(inputPins[i], HIGH); // pull-up 20k
  }*/
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.setCursor(0,0);
  lcd.print("Lavadora 1.0");
  lcd.setCursor(0,2);
  lcd.print("Meca-nica power");
  delay(2000);
}

void loop() {
  setInputFlags();
  resolveInputFlags();
}

void setInputFlags() {
  for(int i = 0; i < numOfInputs; i++) {
    int reading = analogRead(inputPins[i]);
    if (reading != lastInputState[i]) {
      lastDebounceTime[i] = millis();
    }
    if ((millis() - lastDebounceTime[i]) > debounceDelay) {
      if (reading != inputState[i]) {
        inputState[i] = reading;
        inputFlags[i] = HIGH;
      }
    }
    lastInputState[i] = reading;
  }
}

void resolveInputFlags() {
  for(int i = 0; i < numOfInputs; i++) {
    if(inputFlags[i] == HIGH) {
      inputAction(lastInputState[i]);
      inputFlags[i] = LOW;
      printScreen();
    }
  }
}

void inputAction(int input) {
  Serial.print(input);
  Serial.print("\n");
  if(input == 478) {
    if (currentScreen == 0) {
      currentScreen = numOfScreens-1;
    }else{
      currentScreen--;
    }
  }else if(input == 0) {
    if (currentScreen == numOfScreens-1) {
      currentScreen = 0;
    }else{
      currentScreen++;
    }
  }else if(input > 110 && input < 150)/*130*/ {
    parameterChange(0);
  }else if(input > 285 && input < 325)/*305*/ {
    parameterChange(1);
  }
}

void parameterChange(int key) {
  if(key == 0) {
    parameters[currentScreen]++;
  }else if(key == 1) {
    parameters[currentScreen]--;
  }
}

void printScreen() {
  lcd.clear();
  lcd.print(screens[currentScreen][0]);
  lcd.setCursor(0,1);
  lcd.print(parameters[currentScreen]);
  lcd.print(" ");
  lcd.print(screens[currentScreen][1]);
}
