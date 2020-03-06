int Bomba = 6;
int Resistencia = 7;
//int Est_B=0;
//int R=0;
//int Est_R=0;

void admin_valor(char(t))
{
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
    char estado= Serial.read();
    admin_valor(estado);
  }
  
}

void setup()
{
  pinMode(Bomba,OUTPUT);
  pinMode(Resistencia,OUTPUT);
  Serial.begin(9600);
}
void loop()
{
  captura();
  
}
