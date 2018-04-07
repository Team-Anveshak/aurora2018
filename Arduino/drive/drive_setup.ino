void setup(){
  nh.initNode();
  nh.subscribe(locomotion_sub);

  TCCR3B = TCCR3B & B11111000 | B00000001;    // set timer 3 divisor to     1 for PWM frequency of 31372.55 Hz  
  TCCR2B = TCCR2B & B11111000 | B00000001;    // set timer 2 divisor to     1 for PWM frequency of 31372.55 Hz
  
  pinMode(dir1,OUTPUT);
  pinMode(dir2,OUTPUT);
  pinMode(dir3,OUTPUT);
  pinMode(dir4,OUTPUT); 
  
  pinMode(pwm1,OUTPUT);
  pinMode(pwm2,OUTPUT);
  pinMode(pwm3,OUTPUT);
  pinMode(pwm4,OUTPUT);
  
  pinMode(lpotPin, INPUT);
  pinMode(rpotPin, INPUT);

  digitalWrite(slp1,HIGH);
  digitalWrite(slp2,HIGH);
  digitalWrite(slp3,HIGH);
  digitalWrite(slp4,HIGH);
  
  nh.advertise(diag_pub);
 
}
