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
        digitalWrite(ldir,HIGH);
        analogWrite(lpwm,80);
      }
      else if((lpot-set_l)>0){
        digitalWrite(ldir,LOW);
        analogWrite(lpwm,80);
      }
    }
    else
    {
      digitalWrite(ldir,LOW);
      analogWrite(lpwm,0);
    }

   
    if(abs(rpot-set_r)>angle_tolerance)
    {
      
      if((rpot-set_r)<0)
      {
        digitalWrite(rdir,HIGH);
        analogWrite(rpwm,80);
      }
      else if((rpot-set_r)>0)
      {
        digitalWrite(rdir,LOW);
        analogWrite(rpwm,80);
      }
    }
    else
    {
      digitalWrite(rdir,LOW);
      analogWrite(rpwm,0);
    }
    rpot = analogRead(rpotPin);
    lpot = analogRead(lpotPin);
}



void rotate_steer_right(int vel)
{ analogRead(rpotPin);
  if(vel>0.8){
    
    digitalWrite(rdir,LOW);
    analogWrite(rpwm,100);
  }
  else if (vel< -0.8){

    digitalWrite(rdir,HIGH);
    analogWrite(rpwm,100);
  }
  
  else{
   
    digitalWrite(rdir,LOW);
    analogWrite(rpwm,0);
  }
}

void rotate_steer_left(int vel)
{
  if(vel>0.8){
    digitalWrite(ldir,HIGH);
    analogWrite(lpwm,100);
    
  }
  
  else if (vel<-0.8){
    digitalWrite(ldir,LOW);
    analogWrite(lpwm,100);
    
  }
  
  else{
     digitalWrite(ldir,LOW);
    analogWrite(lpwm,0);
    
  }
 
}

