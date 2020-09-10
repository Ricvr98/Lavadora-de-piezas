#include <LiquidCrystal.h>
#include <DallasTemperature.h>
#include <OneWire.h>
//inicializacion de configuracion de pines de salida
const int pin_RS = 8; 
const int pin_EN = 9; 
const int pin_d4 = 4; 
const int pin_d5 = 5; 
const int pin_d6 = 6; 
const int pin_d7 = 7; 
const int pin_temp = 10;
const int pin_nivel_bajo = 2;
const int pin_nivel_alto = 3;
const int pin_bomba = 12;
const int pin_res = 13;

//inicializamos pines donde sera instalado el display
LiquidCrystal lcd( pin_RS,  pin_EN,  pin_d4,  pin_d5,  pin_d6,  pin_d7);

//inicializamos el sensor digital de temperatura ds18b20
OneWire oneWireObjeto(pin_temp);
DallasTemperature sensorDS18B20(&oneWireObjeto);
// Variables con las direcciones únicas de los 4 sensores DS18B20
DeviceAddress sensorTina_1  = {0x28 ,0xFF ,0x81 ,0xD4,0x63 ,0x15 ,0x02 ,0xAF};
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
int estado = 1; //define el inicio del case en el void loop

void setup() 
{
   pinMode(pin_bomba,OUTPUT);
   digitalWrite(pin_bomba,HIGH);
   pinMode(pin_res,OUTPUT);
   digitalWrite(pin_res,HIGH);
   pinMode(pin_nivel_bajo,INPUT);
   pinMode(pin_nivel_alto,INPUT);
   Serial.begin(9600);
   lcd.begin(16, 2);
   lcd.setCursor(0,0);
   lcd.print("Lavadora 1.0");
   lcd.setCursor(0,2);
   lcd.print("Meca-nica power");
   delay(2000);
}
 
void loop() 
{
  
 
  switch (estado)
  {
    case 1: //standby
    setInputFlags();
    resolveInputFlags();
    evaluate();
    
    /*if(Serial.available()>0)
    {
      estado= Serial.read();
      admin_valor(estado);
    }
    else
    {
      delay(500);
    }*/
               break;
    case 2: //Lavado
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Ciclo de lavado");
    delay(5000);
    //lcd.clear();
    calentar();
    inyeccion();
               break;           
    case 3: //Manual
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Ciclo manual");
    delay(5000);
    lcd.clear();
    estado = 1;
               break;
  }
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
void evaluate(){
  if (parameters[2] == 1){
    estado = 2;
  }
  else if( parameters[3] == 1){
    estado = 3;
  }
  else{
    estado = 1;
  }
}

void calentar(){
  Serial.print("Calentar \n");
  int bajo = digitalRead(pin_nivel_bajo);
  int alto = digitalRead(pin_nivel_alto);
  int nivel = (alto + bajo);
  int set = parameters[0];
  int temp;
  Serial.print(nivel);
  Serial.print("\n");

  if(nivel == 1 || nivel == 2){
    
    do{
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Por favor llene");
      lcd.setCursor(0,2);
      lcd.print("el tanque.");
      delay(1000);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Por favor llene");
      lcd.setCursor(0,2);
      lcd.print("el tanque..");
      delay(1000);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Por favor llene");
      lcd.setCursor(0,2);
      lcd.print("el tanque...");
      delay(1000);
      bajo = digitalRead(pin_nivel_bajo);
      alto = digitalRead(pin_nivel_alto);
      nivel = (alto + bajo); 
      Serial.print(bajo);
      Serial.print("bajo\n");
      Serial.print(alto);
      Serial.print("alto\n");
      Serial.print(nivel);
      Serial.print("nivel\n");                 
    }
    while(nivel != 0);
  }
  do
  {
    bajo = digitalRead(pin_nivel_bajo);
    alto = digitalRead(pin_nivel_alto);
    nivel = (alto + bajo);
    sensorDS18B20.requestTemperatures();
    temp = (sensorDS18B20.getTempC(sensorTina_1));
    Serial.print("calentando agua\n");
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Calentando agua");
    lcd.setCursor(0,2);
    lcd.print(temp);
    lcd.setCursor(3,2);
    lcd.print("C");
    lcd.setCursor(10,2);
    lcd.print(set);
    lcd.setCursor(16,2);
    lcd.print("C");
    delay(1000);
    if(temp >= set){
      return 0;
    }
    else{
      digitalWrite(pin_res,LOW); 
    }
  }
  while(nivel==0);
  
  
}
void inyeccion(){
  int bajo = digitalRead(pin_nivel_bajo);
  int alto = digitalRead(pin_nivel_alto);
  int nivel = (alto + bajo);
  long int tiempo_ciclo = (((parameters[1])*60000)-(4294967295));
  long int tiempo_inicial = millis();
  long int tiempo_final = (tiempo_inicial + tiempo_ciclo);
  long int tiempo_actual = millis();
  int temp;
  long int lcd_time;
  if(nivel <= 1){
  do{
      bajo = digitalRead(pin_nivel_bajo);
      alto = digitalRead(pin_nivel_alto);
      nivel = (alto + bajo);
      tiempo_actual = millis();
      digitalWrite(pin_bomba,LOW);
      sensorDS18B20.requestTemperatures();
      temp = (sensorDS18B20.getTempC(sensorTina_1));
      lcd_time = (tiempo_actual/60000);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Ciclo activo");
      lcd.setCursor(0,2);
      lcd.print(parameters[1]);
      lcd.setCursor(3,2);
      lcd.print("min");
      lcd.setCursor(10,2);
      lcd.print(lcd_time);
      lcd.setCursor(14,2);
      lcd.print("min");
    }
  while(tiempo_actual < tiempo_final && nivel <= 1);
  }
  if(nivel > 1){
    
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Nivel de agua");
    lcd.setCursor(0,2);
    lcd.print("Insuficiente");
    delay(5000);
    estado = 2;
    return 0;
  }
  if(tiempo_actual > tiempo_final){
     lcd.clear();
     lcd.setCursor(0,1);
     lcd.print("Ciclo terminado");
     delay(5000); 
     estado = 1;    
  }
  digitalWrite(pin_res,HIGH);
  digitalWrite(pin_bomba,HIGH);
  return 0;
}

void printScreen() {
  lcd.clear();
  lcd.print(screens[currentScreen][0]);
  lcd.setCursor(0,1);
  lcd.print(parameters[currentScreen]);
  lcd.print(" ");
  lcd.print(screens[currentScreen][1]);
}
