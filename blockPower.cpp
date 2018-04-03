#include <DualMC33926MotorShield.h>
#include "blockPower.h"

extern DualMC33926MotorShield motorDriver;
extern Block block[_NUM_BLOCKS];

void Block::init( int motorNum ){
  _motorNum = motorNum;
};

void  Block::setOccupied(){
  _enterTime = millis();
  _occupied = true;
};

void  Block::setClear(){
  setStop();
  _occupied = false;
  _transitTime = millis() - _enterTime;
  if( _transitTime > _MAX_TRANSIT_TIME )
    _transitTime = _MAX_TRANSIT_TIME;
  Serial.print( "transitTime for motor " );
  Serial.println( _motorNum );
  Serial.print( ": " );
  Serial.println( _transitTime );
  _exitTime = millis();
};

void  Block::setFull(){
    if( _speed != 100.0 ) {
      Serial.print( "Set Full Speed motor " );
      Serial.println( _motorNum );
    }
    _setSpeed( 100.0 );
};

void  Block::setHalf(){
    if( _speed != 50.0 ) {
      Serial.print( "Set Half Speed motor " );
      Serial.println( _motorNum );
    }
    _setSpeed( 50.0 );
};

void  Block::setStop(){
    _setSpeed( 0.0 );
    Serial.print( "Set Stop motor " );
    Serial.println( _motorNum );
};

void  Block::check(){
  int targetSpeed;

  if( millis() - _exitTime > 2 * _transitTime )
    setFull();
  else if( millis() - _exitTime > _transitTime )
    setHalf();
  int reading = analogRead( _IN_VOLTAGE_PIN );
  float inVoltage = 1.4 + 43.0 * reading / (204.6 * 10.0);  // add in the diode drop
  reading = analogRead( _IN_DIRECTION_PIN );
  bool inDirection = (reading > 10.0);
  reading = analogRead( _IN_SUPPLY_PIN );
  float inSupply = 43.0 * reading / (204.6 * 10.0);     // small drop since there is no load
  float ratio = inVoltage / inSupply;
  if( inSupply != 0.0 ) {
    targetSpeed = ratio * 400.0 * _speed / 100.0;
    if( !inDirection )
      targetSpeed *= -1;
  } else
    targetSpeed = 0;
  if( (inSupply > 6.40) && (inVoltage > 1.5) ) {  
    // accounts for diode voltage drop under load
    digitalWrite(_NOT_D2_PIN,HIGH);  // turn on outputs
    _outState = HIGH;
  } else if( (inSupply < 5.0) || (inVoltage < 1.5) ) {
    // drive too low or direction measurement inaccurate
    digitalWrite(_NOT_D2_PIN,LOW);  // turn off outputs
    _outState = LOW;
    motorDriver.setM1Speed( 0 );
    motorDriver.setM2Speed( 0 );
    _prevSpeed = 0;
  }

  int speedIncrement = _SPEED_CHANGE_RATE * (millis() - _lastCheckTime) / 1000;
  if( !speedIncrement )
    speedIncrement = 1;
  if( _outState ) {
    if( _prevSpeed < targetSpeed ) {
      _prevSpeed += speedIncrement;
      if( _prevSpeed > targetSpeed )
        _prevSpeed = targetSpeed;
    }
    else if ( _prevSpeed > targetSpeed ) {
      _prevSpeed -= speedIncrement;
      if( _prevSpeed < targetSpeed )
        _prevSpeed = targetSpeed;
    }
    if( _motorNum )
      motorDriver.setM2Speed( _prevSpeed );
    else
      motorDriver.setM1Speed( _prevSpeed );
  }
  _lastCheckTime = millis();
}

void  Block::_setSpeed( float speed ){
  _speed = speed;
}

void  BlockPowerManager::init() {
  motorDriver.init();
  digitalWrite(_NOT_D2_PIN,LOW);  // turn off outputs
  for( int i = 0; i < _NUM_BLOCKS; i++ )
    block[i].init(i);
}

void  BlockPowerManager::check() {
  for( int i = 0; i < _NUM_BLOCKS; i++ )
    block[i].check();
};

