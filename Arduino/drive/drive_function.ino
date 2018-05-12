void loco(int vel,int dir_pin,int pwm_pin,int slp_pin)
{
  if(vel<=0)
    {
     //digitalWrite(slp_pin,HIGH);
        digitalWrite(dir_pin,LOW);
        analogWrite(pwm_pin,abs(vel));
    }
         
  else
    { 
      //digitalWrite(slp_pin,HIGH);
      digitalWrite(dir_pin,HIGH); 
      analogWrite(pwm_pin,abs(vel));
    }
}

void rotate(int set_r,int set_l)
{
  
    if(abs(lpot-set_l)>angle_tolerance)
    {
      
      if((lpot-set_l)<0){
        digitalWrite(lmINA,HIGH);
        digitalWrite(lmINB,LOW);
        analogWrite(lmpwm,80);
      }
      else if((lpot-set_l)>0){
        digitalWrite(lmINA,LOW);
        digitalWrite(lmINB,HIGH);
        analogWrite(lmpwm,80);
      }
    }
    else
    {
     digitalWrite(lmINA,LOW);
     digitalWrite(lmINB,LOW);
      analogWrite(lmpwm,0);
    }

   
    if(abs(rpot-set_r)>angle_tolerance)
    {
      
      if((rpot-set_r)<0)
      {
        digitalWrite(rmINA,HIGH);
        digitalWrite(rmINB,LOW);
        analogWrite(rmpwm,80);
      }
      else if((rpot-set_r)>0)
      {
        digitalWrite(rmINA,LOW);
        digitalWrite(rmINB,HIGH);
        analogWrite(rmpwm,80);
      }
    }
    else
    {
      digitalWrite(rmINA,LOW);
      digitalWrite(rmINB,LOW);
      analogWrite(rmpwm,0);
    }
    rpot = analogRead(rpotPin);
    lpot = analogRead(lpotPin);
}



void rotate_steer_right(int vel)
{ analogRead(rpotPin);
  if(vel>0.8){
    
    digitalWrite(rmINA,LOW);
    digitalWrite(rmINB,HIGH);
    analogWrite(rmpwm,100);
  }
  else if (vel< -0.8){

    digitalWrite(rmINA,HIGH);
    digitalWrite(rmINB,LOW);
    analogWrite(rmpwm,100);
    
  }
  
  else{
   
   digitalWrite(rmINA,LOW);
   digitalWrite(rmINB,LOW);
   analogWrite(rmpwm,0);
  }
}

void rotate_steer_left(int vel)
{
  if(vel>0.8){
    digitalWrite(lmINA,HIGH);
    digitalWrite(lmINB,LOW);
    analogWrite(lmpwm,100);
    
  }
  
  else if (vel<-0.8){
    digitalWrite(lmINA,LOW);
    digitalWrite(lmINB,HIGH);
    analogWrite(lmpwm,100);
    
  }
  
  else{
    digitalWrite(lmINA,LOW);
    digitalWrite(lmINB,LOW);
    analogWrite(lmpwm,0);
    
  }
 
}
///
