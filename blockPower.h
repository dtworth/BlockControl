#define  _IN_VOLTAGE_PIN 2
#define  _IN_DIRECTION_PIN 3
#define  _IN_SUPPLY_PIN 4
#define  _NOT_D2_PIN 4

#define _NUM_BLOCKS 2
#define _MAX_TRANSIT_TIME 30000
#define _SPEED_CHANGE_RATE  10

class Block {
  public:
    void  init(int);
    void  setFull();
    void  setHalf();
    void  setStop();
    void  setOccupied();
    void  setClear();
    void  check();
  private:
    void  _setSpeed(float);  // speed in percentage
    int _motorNum;
    float _speed = 0.0;   // percentage of max
    int _prevSpeed = 0;
    bool _outState = false;
    bool _occupied = false;
    long _enterTime = 0;
    long _exitTime = 0;
    long _transitTime = 0;
    long _lastCheckTime = millis();
};

class BlockPowerManager {
  public:
    static void  init();
    static void  check();
};

