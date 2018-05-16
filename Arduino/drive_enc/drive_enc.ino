#define BAUD_RATE 57600
#define M_DIR1  5
#define M_DIR2  7
#define M_PWM1  4
#define M_PWM2  6
#define ENC1  18
#define ENC2  19

int enc_1_tick=0;
int enc_2_tick=0;
int count;
int pwm_value_1;
int pwm_value_2;
int data;


unsigned long previoustime;
double m_1_vel,m_2_vel;



void setup() {
 Serial.begin(BAUD_RATE);
 
 pinMode(M_DIR1,OUTPUT);
 pinMode(M_DIR2,OUTPUT);
 pinMode(M_PWM1,OUTPUT);
 pinMode(M_PWM2,OUTPUT);
 pinMode(ENC1,INPUT);
 pinMode(ENC2,INPUT);

 attachInterrupt(digitalPinToInterrupt(ENC1),Enc_1_count,RISING);
 attachInterrupt(digitalPinToInterrupt(ENC2),Enc_2_count,RISING);

}

void loop() 
{
 if(count>=100)
{
  detachInterrupt(digitalPinToInterrupt(ENC1));
  detachInterrupt(digitalPinToInterrupt(ENC2));

  
  m_1_vel=double(enc_1_tick*1000000)/double(micros()-previoustime);
  m_2_vel=double(enc_2_tick*1000000)/double(micros()-previoustime);

  
  Serial.print("e");
  Serial.print(",");
  Serial.print(m_1_vel);
  Serial.print(",");
  Serial.print(m_2_vel);
  Serial.print("\n");
  
  enc_1_tick=0;
  enc_2_tick=0;
  
  previoustime=micros();
  
  
  
  attachInterrupt(digitalPinToInterrupt(ENC1), Enc_1_count, FALLING);
  attachInterrupt(digitalPinToInterrupt(ENC2), Enc_2_count, FALLING);
}

count++;

//Serial_read();
//Update_motor();
delay(100);

}

void Serial_read(){

  data=Serial.parseInt();

    pwm_value_1=(data/1000);
    pwm_value_2=(data-(1000*pwm_value_1));
  
}

void Enc_1_count()
{
  enc_1_tick++;
}

void Enc_2_count()
{
  enc_2_tick++;
}
/*
void Update_motor()
{
 analogWrite(M_PWM1,pwm_value_1);
 analogWrite(M_PWM2,pwm_value_2);
  
}*/

