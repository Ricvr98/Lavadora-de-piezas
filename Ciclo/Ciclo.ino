#include <NewPing.h>
#include <OneWire.h>
#include <DallasTemperature.h>
/*Aqui se configuran los pines donde debemos conectar el sensor*/
#define TEMP_PIN     9
#define TRIGGER_PIN  12
#define ECHO_PIN     11
#define MAX_DISTANCE 50
#define BOMBA_PIN    6
#define RES_PIN      7
/*Crear el objeto de la clase NewPing*/
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);
OneWire oneWireObjeto(TEMP_PIN);
DallasTemperature sensorDS18B20(&oneWireObjeto);
// Variables con las direcciones únicas de los 4 sensores DS18B20
DeviceAddress sensorTina_1  = {0x28 ,0xFF ,0x81 ,0xD4,0x63 ,0x15 ,0x02 ,0xAF};
//DeviceAddress sensorTina_2 = {0x28 ,0xFF ,0x3A ,0xA8 ,0x63 ,0x15 ,0x02 ,0x2A};

//int Bomba = 6;
//int Resistencia = 7;


void admin_valor(char(t))
{
  int tiempo = 600000;
  float temp  = 0;
  int min_t = 20;
  int max_t = 7;
  // Esperar 1 segundo entre mediciones
  delay(1000);
  // Obtener medicion de tiempo de viaje del sonido y guardar en variable uS
  /*int uS = sonar.ping_median();
  float boya = (uS / US_ROUNDTRIP_CM);
  Serial.print(boya);
  Serial.print("\n");
  */
  if(t=='s' || t=='S')
  {
      /*digitalWrite(RES_PIN,LOW);
      digitalWrite(BOMBA_PIN,LOW);
      */do
        { 
          // Esperar 1 segundo entre mediciones
          delay(1000);
          // Obtener medicion de tiempo de viaje del sonido y guardar en variable uS
          int uS = sonar.ping_median();
          float boya = (uS / US_ROUNDTRIP_CM);
          Serial.print(boya);
          Serial.print("\n");
          sensorDS18B20.requestTemperatures();
          temp = (sensorDS18B20.getTempC(sensorTina_1));
          if(min_t >= boya || boya <= max_t)
          {
            //encendemos las resistencias para calentar al agua
            digitalWrite(RES_PIN,LOW);
            // Mandamos comandos para toma de temperatura a los sensores
            
            Serial.print(temp);
            Serial.print("C");
            Serial.print("\n");
            do
            {
                sensorDS18B20.requestTemperatures();
                temp = (sensorDS18B20.getTempC(sensorTina_1));
                Serial.print(temp);
                Serial.print("C");
                Serial.print("\n");
                delay(1000);
                if(temp >= 60)
                {
                  digitalWrite(BOMBA_PIN,LOW);
                  delay(tiempo);
                  digitalWrite(RES_PIN,HIGH);
                  digitalWrite(BOMBA_PIN,HIGH);
                  Serial.print("Ciclo de lavado terminado\n");
                }
            }
            while(boya < min_t);
           }
          }
      while(t!= 'a' || t!= 'A');
  }
}
    
/* if(t=='B')
    {
      digitalWrite(Bomba,HIGH);
    }
  if( t=='b')
    {
      digitalWrite(Bomba,LOW);
    }
  if(t=='R')
    {
      digitalWrite(Resistencia,HIGH);
    }
  if( t=='r')
    {
      digitalWrite(Resistencia,LOW);
    }
 
}
*/
void captura()
{
  if(Serial.available()>0)
  {
    char estado= Serial.read();
    admin_valor(estado);
  }
    else
  {
    delay(500);
  }
  
}

void setup()
{
  pinMode(BOMBA_PIN,OUTPUT);
  digitalWrite(BOMBA_PIN,HIGH);
  pinMode(RES_PIN,OUTPUT);
  digitalWrite(RES_PIN,HIGH);
  Serial.begin(9600);
  // Iniciamos el bus 1-Wire
  sensorDS18B20.begin();
  // Buscamos los sensores conectados
  Serial.println("Buscando dispositivos...");
  Serial.println("Encontrados: ");
  Serial.print(sensorDS18B20.getDeviceCount());
  Serial.println(" sensores");

}
void loop()
{
  captura();
  
}
