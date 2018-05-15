#define EN1 43
#define EN2 25
#define EN3 37
#define EN4 27
#define INA1 45
#define INA2 23
#define INA3 39
#define INA4 29
#define INB1 47
#define INB2 35
#define INB3 41
#define INB4 31
#define PWM1 8
#define PWM2 5
#define PWM3 7
#define PWM4 4

//Small Pololu
#define DIR 42
#define SLP 44
#define CS_G A0
#define PWM_G 6

// L293D
#define ENABLE 46
#define INPUT1 48
#define INPUT2 50

// RHINO Encoders
#define ENCA1 3
#define ENCA2 2

// 2-ch POLULO
#define PWM3 7
#define EN3 37
#define INA3 39
#define INB3 41

#define t1_factor 0.5
#define t2_factor 0.5
#define t3_factor 0.5

#include<ros.h>
#include<geometry_msgs/Vector3.h>
#include<Wire.h>

float o[3] = {0};
float t1_raw = 0, t3_raw = 0, t2_raw = 0, t1i_raw = 0, t2i_raw = 0, t3i_raw = 0, t1 = 0, t2 = 0, t3 = 0, t1f = 0, t2f = 0, t3f = 0, t1i = 0, t2i = 0, t3i = 0;
int n1 = 0, n2 = 0, n3 = 0, n4 = 0, n5 = 0, n4i = 0, n5i = 0;
float dn4 = 0, dn5 = 0;
unsigned long int ti = 0;
float dt = 0;
float w_rm1 = 0, w_rm2 = 0, e_rm1 = 0, e_rm2 = 0, I_rm1 = 0, I_rm2 = 0, D_rm1 = 0, D_rm2 = 0, ep_rm1 = 0, ep_rm2 = 0;
float pwm1 = 0, pwm2 = 0, pwm3 = 0, pwm1i = 0, pwm2i = 0, pwm4 = 0;
float e1 = 0, e1p = 0, I1 = 0, D1 = 0, e2 = 0, e2p = 0, I2 = 0, D2 = 0, w1 = 0, w2 = 0, w3 = 0;
float Kp1 = -0.02, Ki1 = -0.00066, Kd1 = 0, Kp2 = 0.02, Ki2 = -0.00066, Kd2 = 0, Kp_rm1 = 0, Ki_rm1 = 0, Kd_rm1 = 0, Kp_rm2 = 0, Ki_rm2 = 0, Kd_rm2 = 0;
float c[2] = {0};
int c2 = 0;

int Map[256] = {
  0xFF,0x38,0x28,0x37,0x18,0xFF,0x27,0x34,0x08,0x39,0xFF,0xFF,0x17,0xFF,0x24,0x0D,
  0x78,0xFF,0x29,0x36,0xFF,0xFF,0xFF,0x35,0x07,0xFF,0xFF,0xFF,0x14,0x13,0x7D,0x12,
  0x68,0x69,0xFF,0xFF,0x19,0x6A,0x26,0xFF,0xFF,0x3A,0xFF,0xFF,0xFF,0xFF,0x25,0x0E,
  0x77,0x76,0xFF,0xFF,0xFF,0x6B,0xFF,0xFF,0x04,0xFF,0x03,0xFF,0x6D,0x6C,0x02,0x01,
  0x58,0xFF,0x59,0xFF,0xFF,0xFF,0xFF,0x33,0x09,0x0A,0x5A,0xFF,0x16,0x0B,0xFF,0x0C,
  0xFF,0xFF,0x2A,0x2B,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x15,0xFF,0x7E,0x7F,
  0x67,0xFF,0x66,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x5B,0xFF,0xFF,0xFF,0xFF,0xFF,
  0x74,0x75,0xFF,0xFF,0x73,0xFF,0xFF,0xFF,0x5D,0x5E,0x5C,0xFF,0x72,0x5F,0x71,0x00,
  0x48,0x47,0xFF,0x44,0x49,0xFF,0xFF,0x1D,0xFF,0x46,0xFF,0x45,0xFF,0xFF,0x23,0x22,
  0x79,0xFF,0x7A,0xFF,0x4A,0xFF,0xFF,0x1E,0x06,0xFF,0x7B,0xFF,0xFF,0xFF,0x7C,0x11,
  0xFF,0xFF,0xFF,0x43,0x1A,0xFF,0x1B,0x1C,0xFF,0x3B,0xFF,0xFF,0xFF,0xFF,0xFF,0x0F,
  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x05,0xFF,0xFF,0xFF,0x6E,0xFF,0x6F,0x10,
  0x57,0x54,0xFF,0x2D,0x56,0x55,0xFF,0x32,0xFF,0xFF,0xFF,0x2E,0xFF,0xFF,0xFF,0x21,
  0xFF,0x53,0xFF,0x2C,0x4B,0xFF,0xFF,0x1F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x20,
  0x64,0x3D,0x65,0x42,0xFF,0x3E,0xFF,0x31,0x63,0x3C,0xFF,0x2F,0xFF,0xFF,0xFF,0x30,
  0x4D,0x52,0x4E,0x41,0x4C,0x3F,0xFF,0x40,0x62,0x51,0x4F,0x50,0x61,0x60,0x70,0xFF };

ros::NodeHandle nh;

geometry_msgs::Vector3 omegas;
geometry_msgs::Vector3 error;
ros::Publisher motor_speed("motor_speed", &omegas);
ros::Publisher Error("error", &error);

void rm1_count()
{
  n4++;
}
void rm2_count()
{
  n5++;
}

void Set(const geometry_msgs::Vector3& set_value)
{
  c[0] = set_value.x;
  c[1] = set_value.y;
  c2 = set_value.z;
}

void param_rec1(const geometry_msgs::Vector3& pid_parameters)
{
  Kp_rm1 = pid_parameters.x;
  Ki_rm1 = pid_parameters.y;
  Kd_rm1 = pid_parameters.z;
}

void param_rec2(const geometry_msgs::Vector3& pid_parameters)
{
  Kp_rm2 = pid_parameters.x;
  Ki_rm2 = pid_parameters.y;
  Kd_rm2 = pid_parameters.z;
}

void Check_Angle_t1(int posf, int posi){
  if(posi - posf > 300)
  {
    n1++;
  }
  else
  {
    if(posf - posi > 300)
    {
      n1--;
    }
  }
}
void Check_Angle_t2(int posf, int posi){
  if(posi - posf > 300)
  {
    n2++;
  }
  else
  {
    if(posf - posi > 300)
    {
      n2--;
    }
  }
}
void Check_Angle_t3(int posf, int posi){
  if(posi - posf > 300)
  {
    n3++;
  }
  else
  {
    if(posf - posi > 300)
    {
      n3--;
    }
  }
}

ros::Subscriber<geometry_msgs::Vector3> sub1("set", Set );
ros::Subscriber<geometry_msgs::Vector3> sub2("pid_param1", param_rec1 );
ros::Subscriber<geometry_msgs::Vector3> sub3("pid_param2", param_rec2 );

void setup(){
  //Initialize pins
  pinMode(EN1, OUTPUT);
  pinMode(EN2, OUTPUT);
  pinMode(EN3, OUTPUT);
  pinMode(EN4, OUTPUT);
  
  pinMode(INA1, OUTPUT);
  pinMode(INA2, OUTPUT);
  pinMode(INA3, OUTPUT);
  pinMode(INA4, OUTPUT);
  
  pinMode(INB1, OUTPUT);
  pinMode(INB2, OUTPUT);
  pinMode(INB3, OUTPUT);
  pinMode(INB4, OUTPUT);
  
  pinMode(PWM1, OUTPUT);
  pinMode(PWM2, OUTPUT);
  pinMode(PWM3, OUTPUT);
  pinMode(PWM4, OUTPUT);
  
  pinMode(INPUT1, OUTPUT);
  pinMode(INPUT2, OUTPUT);
  pinMode(ENABLE, OUTPUT);
  pinMode(SLP, OUTPUT);
  pinMode(DIR, OUTPUT);
  pinMode(PWM_G, OUTPUT);

  digitalWrite(EN1, HIGH);
  digitalWrite(EN2, HIGH);
  digitalWrite(EN3, HIGH);
  digitalWrite(EN4, HIGH);
  digitalWrite(SLP, HIGH);
  Wire.begin();
 
  /*Wire.requestFrom(0x21, 1);
  while(Wire.available() == 0);
  float t1i_raw = Map[Wire.read()]*(360.0/128.0);
  t1i = t1i_raw;
  
  Wire.requestFrom(0x27, 1);
  while(Wire.available() == 0);
  float t2i_raw = Map[Wire.read()]*(360.0/128.0);
  t2i = t2i_raw;
  */
  nh.initNode();
  nh.advertise(motor_speed);
  nh.advertise(Error);
  nh.subscribe(sub1);
  nh.subscribe(sub2);
  nh.subscribe(sub3);
  
  attachInterrupt(0, rm1_count, RISING);
  attachInterrupt(1, rm2_count, RISING);
  
  ti = micros();
}
//
void loop(){
  nh.spinOnce();
  dt = micros() - ti;
  ti = micros();
  
  switch(c2)
  {
    case 0:
      analogWrite(PWM1, 0);
      analogWrite(PWM2, 0);
      analogWrite(PWM3, 0);
      analogWrite(PWM4, 0);
      analogWrite(PWM_G, 0);
      analogWrite(ENABLE, 0);
      e2p = 0;
      e2 = 0;
      I2 = 0;
      pwm2i = 0;
      e1p = 0;
      e1 = 0;
      I1 = 0;
      pwm1i = 0;
      break;

/*    case 1:
      //Motor1
      Wire.requestFrom(0x21, 1);
      while(Wire.available() == 0);
      t1_raw = Map[Wire.read()]*(360.0/128.0);
      Check_Angle_t1(t1_raw, t1i_raw);
      t1i_raw = t1_raw;
      t1 = t1_raw + n1*360.0;
      
      w1 = (t1 - t1i)*1000000/dt;
      t1i = t1;
    
      e1 = (float)c[0] - w1;
      I1 = I1 + e1*dt*0.000001;
      D1 = (e1 - e1p)*1000000/dt;
      e1p = e1;
      pwm1 = pwm1i + Kp1*e1 + Ki1*I1 + Kd1*D1;
      pwm1i = pwm1;
      
      if(pwm1 < 0)
      {   
        digitalWrite(INA4, HIGH);
        digitalWrite(INB4, LOW);
      }
      else
      {
        digitalWrite(INA4, LOW);
        digitalWrite(INB4, HIGH);
      }
      error.x = pwm1;    
      pwm1 = abs(pwm1);
      constrain(pwm1, 0, 255);      
      analogWrite(PWM4, (int)pwm1);
      
      //Motor2
      Wire.requestFrom(0x27, 1);
      while(Wire.available() == 0);
      t2_raw = Map[Wire.read()]*(360.0/128.0);
      Check_Angle_t2(t2_raw, t2i_raw);
      t2i_raw = t2_raw;
      t2 = t2_raw + n2*360.0;
      
      w2 = (t2 - t2i)*1000000/dt;
      t2i = t2;
    
      e2 = (float)c[1] - w2;
      I2 = I2 + e2*dt*0.000001;
      D2 = (e2 - e2p)*1000000/dt;
      e2p = e2;
      pwm2 = pwm2i + Kp2*e2 + Ki2*I2 + Kd2*D2;
      pwm2i = pwm2;
      
      if(pwm2 < 0)
      {   
        digitalWrite(INA2, HIGH);
        digitalWrite(INB2, LOW);
      }
      else
      {
        digitalWrite(INA2, LOW);
        digitalWrite(INB2, HIGH);
      }
      error.y = pwm2;    
      pwm2 = abs(pwm2);
      constrain(pwm2, 0, 255);
      
      analogWrite(PWM2, (int)pwm2);
      break;
*/
    case 2:
      if(c[1] == 1)
      {
        digitalWrite(DIR, HIGH);
        analogWrite(PWM_G, c[0]);
      }
      else
      {
        digitalWrite(DIR, LOW);
        analogWrite(PWM_G, c[0]);
      }
      break;
      
    case 3:
      /*dn4 = n4 - n4i;
      n4i = n4;
      w_rm1 = dn4*360.0*1000000.0/(dt*1860.0);
      e_rm1 = c[0] - w_rm1;
      I_rm1 = I_rm1 + e_rm1*dt*0.000001;
      D_rm1 = (e_rm1 - ep_rm1)*1000000.0/dt;
      ep_rm1 = e_rm1;
      pwm3 = pwm3 + Kp_rm1*e_rm1 + Ki_rm1*I_rm1 + Kd_rm1*D_rm1;
      
      if(pwm3 < 0)
      {
        digitalWrite(INA1, HIGH);
        digitalWrite(INB1, LOW);
      }
      else
      {
        digitalWrite(INA1, LOW);
        digitalWrite(INB1, HIGH);
      }
      
      pwm3 = abs(pwm3);
      constrain(pwm3, 0, 255);
      analogWrite(PWM1, pwm3);
      
      dn5 = n5 - n5i;
      n5i = n5;
      w_rm2 = (float)dn5*360.0*1000000.0/(dt*1860.0);
      e_rm2 = (float)c[1] - w_rm2;
      I_rm2 = I_rm2 + e_rm2*dt*0.000001;
      D_rm2 = (e_rm2 - ep_rm2)*1000000.0/dt;
      ep_rm2 = e_rm2;
      pwm4 = pwm4 + Kp_rm2*e_rm2 + Ki_rm2*I_rm2 + Kd_rm2*D_rm2;
      
      if(pwm4 < 0)
      {
        digitalWrite(INA3, HIGH);
        digitalWrite(INB3, LOW);
      }
      else
      {
        digitalWrite(INA3, LOW);
        digitalWrite(INB3, HIGH);
      }
      
      pwm4 = abs(pwm4);
      constrain(pwm4, 0, 255);
      analogWrite(PWM3, pwm4);
      break;*/
      if(c[0] > 0)
      {
        digitalWrite(INA1, HIGH);
        digitalWrite(INB1, LOW);
      }
      else
      {
        digitalWrite(INA1, LOW);
        digitalWrite(INB1, HIGH);
      }
      analogWrite(PWM1, abs(c[0]));
      
      if(c[1] > 0)
      {
        digitalWrite(INA3, HIGH);
        digitalWrite(INB3, LOW);
      }
      else
      {
        digitalWrite(INA3, LOW);
        digitalWrite(INB3, HIGH);
      }
      analogWrite(PWM3, abs(c[1]));
      
      
    case 4:
      if(c[1] == 1)
      {
        digitalWrite(INPUT1,HIGH);
        digitalWrite(INPUT2,LOW);
        analogWrite(ENABLE,c[0]);
      }
      else
      {
        digitalWrite(INPUT1,LOW);
        digitalWrite(INPUT2,HIGH);
        analogWrite(ENABLE,c[0]);
      }
      break;
  }
  omegas.x = w1;
  omegas.y = w2;
  omegas.z = 0;
  
  error.z = 0;
  
  motor_speed.publish(&omegas);
  Error.publish(&error);

}
