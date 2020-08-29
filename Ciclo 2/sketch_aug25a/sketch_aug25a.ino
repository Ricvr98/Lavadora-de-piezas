void setup() 
{
   Serial.begin(9600);
}
 
void loop() 
{
  int estado = 1;
 
  switch (estado)
  {
    case 1: //standby
    if(Serial.available()>0)
    {
      estado= Serial.read();
      admin_valor(estado);
    }
    else
    {
      delay(500);
    }
               break;
    case 2: //Calentamiento del agua
               break;           
    case 3: //lavado
               break;
    case 4: //Manual
               break;
  }
}
