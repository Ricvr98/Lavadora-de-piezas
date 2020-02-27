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

void setup() 
{
  // Iniciamos la comunicación serie
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
  // Mandamos comandos para toma de temperatura a los sensores
  Serial.println("Mandando comandos a los sensores");
  sensorDS18B20.requestTemperatures();
 
  // Leemos y mostramos los datos de los sensores DS18B20 por dirección única
  Serial.print("Temperatura sensor 1: ");
  Serial.print(sensorDS18B20.getTempC(sensorTina_1));
  Serial.println(" C");
  Serial.print("Temperatura sensor 2: ");
  Serial.print(sensorDS18B20.getTempC(sensorTina_2));
  Serial.println(" C");
  
  delay(1000);
}
