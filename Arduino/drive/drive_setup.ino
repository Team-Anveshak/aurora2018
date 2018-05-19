void setup(){
  nh.initNode();
  nh.subscribe(locomotion_sub);

  TCCR3B = TCCR3B & B11111000 | B00000001;    // set timer 3 divisor to     1 for PWM frequency of 31372.55 Hz  
  TCCR4B = TCCR4B & B11111000 | B00000001;    // set timer 4 divisor to     1 for PWM frequency of 31372.55 Hz

  pan.attach(pan_pin);
  tilt.attach(tilt_pin);
  
  pinMode(dirlf,OUTPUT);
  pinMode(dirlb,OUTPUT);
  pinMode(dirrf,OUTPUT);
  pinMode(dirrb,OUTPUT); 
  
  pinMode(pwmlf,OUTPUT);
  pinMode(pwmlb,OUTPUT);
  pinMode(pwmrf,OUTPUT);
  pinMode(pwmrb,OUTPUT);
  
  pinMode(lpotPin, INPUT);
  pinMode(rpotPin, INPUT);

  digitalWrite(slplf,HIGH);
  digitalWrite(slplb,HIGH);
  digitalWrite(slprf,HIGH);
  digitalWrite(slprb,HIGH);

  if(analogRead(rpotPin)!=0 && analogRead(lpotPin)!=0){
    set_r_zero = analogRead(rpotPin);
    set_l_zero = analogRead(lpotPin);
  }
  
  set_r_angle=set_r_zero+140;
  set_l_angle=set_l_zero+140;
  set_r_90_angle=set_r_zero+240;
  set_l_90_angle=set_l_zero+240;
  
  analogWrite(pwmlf,0);
  analogWrite(pwmlb,0);
  analogWrite(pwmrf,0);
  analogWrite(pwmrb,0);
  
  nh.advertise(diag_pub);
  servo_now = millis();
  pan.write(pan_pos);
  tilt.write(tilt_pos);
 
}
