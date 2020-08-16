#include <LiquidCrystal.h>

LiquidCrystal lcd(8, 13, 9, 4, 5, 6, 7);

const int numeroDeMenus=5;
const int numeroMaximoDeSubmenus=4;

char tituloMenu[numeroDeMenus][16] = {
  "1  Menu 1    ",
  "2  Menu 2    ",               
  "3  Menu 3    ",
  "4  Menu 4    ",
  "5  Menu 5    " };
  
byte numeroDeSubmenus[numeroDeMenus] = {4,3,2,1,4};

char tituloSubmenu[numeroDeMenus][numeroMaximoDeSubmenus][16] = 
{
  "1.1 Submenu 1","1.2 Submenu 2","1.3 Submenu 3","1.4 Submenu 4",
  "2.1 Submenu 1","2.2 Submenu 2","2.3 Submenu 3","",
  "3.1 Submenu 1","3.2 Submenu 2","","",
  "4.1 Submenu 1","","","",
  "5.1 Submenu 1","5.2 Submenu 2","5.3 Submenu 3","5.4 Submenu 4"};

int adc_key_val[5] ={
  50, 200, 400, 600, 800 };
int NUM_KEYS = 5;
int adc_key_in;
int key=-1;
int oldkey=-1;
boolean luzEncendida=true;
boolean cursorActivo=false;
unsigned long time;
int x=0;
int y=0;

void setup()
{
  lcd.clear(); 
  lcd.begin(16, 2);
  lcd.setCursor(0,0); 
  lcd.print("www.ajpdsoft.com");
  lcd.setCursor(0,1); 
  lcd.print("Menu version 0.1");
  delay(5000);
  lcd.setCursor(0,0); 
  lcd.print("Muevase con las ");
  lcd.setCursor(0,1); 
  lcd.print("teclas direccion");
  delay(4000);
  lcd.clear();
  lcd.setCursor(0,0); 
  lcd.print("Menu principal");
  lcd.setCursor(0,1); 
  lcd.print(tituloMenu[x]);
  time = millis();
  //  pinMode(10, OUTPUT);
}

void loop()
{
  // Si han pasado mas de 10 segundos apagamos la luz
  if (millis()-time > 10000) {
    pinMode(10, OUTPUT);
    digitalWrite(10, LOW);
    luzEncendida=false;
  }
  // Si han pasado mas de 5 segundos apagamos el cursor
  if (millis()-time > 5000) {
    lcd.noBlink();
    cursorActivo=false;
  } 

  adc_key_in = analogRead(0);    // Leemos el valor de la pulsacion
  key = get_key(adc_key_in);    // Obtenemos el boton pulsado

  if (key != oldkey)   // if keypress is detected
  
    delay(50);  // Espera para evitar los rebotes de las pulsaciones
    adc_key_in = analogRead(0);    // Leemos el valor de la pulsacion
    key = get_key(adc_key_in);    // Obtenemos el boton pulsado
    if (key != oldkey)    
    {
      // TODO: falta la comprobacion de si se ha desbordado el tiempo
      time = millis();  
      if (!luzEncendida) {  // Al pulsar cualquier tecla encendemos la pantalla
        pinMode(10, INPUT);
        luzEncendida=true;
      } 
      else {  // si la pantalla esta encendida seguimos funcionando normalmente
        oldkey = key;
        if (key >=0){  // Si se ha pulsado cualquier tecla
          lcd.blink();  // Mostramos el cursor parpadeando
          cursorActivo=true;
        }
        if (key == 0){  // Se ha pulsado la tecla derecha
          x++;
          if (x>numeroDeMenus-1) x=numeroDeMenus-1;
          y=0;
        }
        if (key == 1) {  // Se ha pulsado la tecla arriba
          y++;
          if (y > numeroDeSubmenus[x]-1) y=numeroDeSubmenus[x]-1;
        }
        if (key == 2) {  // Se ha pulsado la tecla abajo
          y--;
          if (y < 0) y=0;
        }
        if (key == 3) {  // Se ha pulsado la tecla izquierda
          x--;
          if (x<0) x=0;
          y=0;
        }
        if (key == 4){  // Se ha pulsado la tecla de seleccion
        }
        lcd.clear();
        lcd.setCursor(0,0); 
        lcd.print(tituloMenu[x]);
        lcd.setCursor(0,1); 
        lcd.print(tituloSubmenu[x][y]);
      }
    }
  }
  delay(50);
}

// Convertimos el valor leido en analogico en un numero de boton pulsado
int get_key(unsigned int input)
{
  int k;

  for (k = 0; k < NUM_KEYS; k++)
  {
    if (input < adc_key_val[k])
    {
      return k;
    }
  }

  if (k >= NUM_KEYS)k = -1;  // Error en la lectura
  return k;
}
