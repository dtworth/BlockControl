/* MIT License
Copyright (c) 2018 Dan Worth

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include <DualMC33926MotorShield.h>
#include <Bounce2.h>
#include "blockPower.h"

#define BLOCK1_DETECTOR 2
#define BLOCK2_DETECTOR 3

DualMC33926MotorShield motorDriver;
Block block[_NUM_BLOCKS];

Bounce  debounce[_NUM_BLOCKS];

void setup() {
  BlockPowerManager::init();
  Serial.begin(115200);

  pinMode( BLOCK1_DETECTOR, INPUT_PULLUP );
  pinMode( BLOCK2_DETECTOR, INPUT_PULLUP );

  debounce[0].attach( BLOCK1_DETECTOR );
  debounce[1].attach( BLOCK2_DETECTOR );
}

void loop() {
  
  for( int i=0; i<_NUM_BLOCKS; i++ )
    if( debounce[i].update() )
      if( debounce[i].read() )
        block[i].setClear();
      else
        block[i].setOccupied();
  
  BlockPowerManager::check();

  delay(100);
}
