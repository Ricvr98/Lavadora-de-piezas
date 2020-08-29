#include <OneWire.h>
#include <DallasTemperature.h>

// Pin donde se conecta el bus 1-Wire
const int pinDatosDQ = 9;
// Instancia a las clases OneWire y DallasTemperature
OneWire oneWireObjeto(pinDatosDQ);
DallasTemperature sensorDS18B20(&oneWireObjeto);
// Variables con las direcciones únicas de los 4 sensores DS18B20
DeviceAddress sensorTina_1  = {0x28 ,0xFF ,0x81 ,0xD4 ,0x63 ,0x15 ,0x02 ,0xAF};
DeviceAddress sensorTina_2 = {0x28 ,0xFF ,0x3A ,0xA8 ,0x63 ,0x15 ,0x02 ,0x2A};
//Pines en donde se emiten las señales de activacion para relees
int Bomba = 6;
int Resistencia = 7;

float control_temp()
{
  sensorDS18B20.requestTemperatures();
  int t_1 = sensorDS18B20.getTempC(sensorTina_1);
  int t_2 = sensorDS18B20.getTempC(sensorTina_2);
  return t_1;
}

void admin_valor(float(t))
{
  float t_r = control_temp();
  
  if(t=='B')
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


void captura()
{
  if(Serial.available()==0)
  {
    delay(100);
  }
  if(Serial.available()>0)
  {
    float estado= Serial.read();
    admin_valor(estado);
  }
  
}

void setup()
{
  //Inicializamos los pines correspondientes a los relees
  pinMode(Bomba,OUTPUT);
  pinMode(Resistencia,OUTPUT);
  Serial.begin(9600);
  // Iniciamos el bus 1-Wire
  sensorDS18B20.begin();
}
void loop()
{
  
  captura();
  
}
