
#include "main.h"
extern MQTTClient *clbwrapobj;
#ifdef USE_DFPLAYER
SoftwareSerial dfserial(PB_4, PB_3);
DFRobotDFPlayerMini player;
#endif
SoftwareSerial serial(PA_3, PA_2); // PA_12
void setup()
{

  SPI.setMOSI(PB15);
  SPI.setMISO(PB14);
  SPI.setSCLK(PB13);

  pinMode(PA1, OUTPUT);
  digitalWrite(PA1, LOW);

  serial.begin(9600);
  delay(100);
  SEFL::Logger::getInstance()->setPrinter(&serial);

  SEFL::Logger::getInstance()->setLogLevel(SEFL::Logger::Level::NOTICE); // VERBOSE   NOTICE
  SEFL::Logger::getInstance()->setPostMessage();
  SEFL::Logger::notice("main", "Initing board");
#ifdef USE_DFPLAYER
  dfserial.begin(9600);
  player.begin(dfserial);
  player.volume(25);
  player.enableDAC();
#endif
  delay(100);

  pinMode(PB10, OUTPUT);
  digitalWrite(PB10, LOW);
  Wire.setSCL(PB6);
  Wire.setSDA(PB7);
  Wire.begin();
  Pext.getHandler()->resetDevices();
  Pext.getHandler()->init();
  Pext.getHandler()->setPWMFrequency(1600); // 1600
  Pext.getHandler()->setAllChannelsPWM(4096);
  SEFL::Logger::notice("main", "Initing ethernet");

#if Uniboard == 1
  byte mac[] = {0x30, 0x16, 0x00, 0x00, 0x02, 0x03};
#endif

#if Uniboard == 2
  byte mac[] = {0x30, 0x16, 0x00, 0x00, 0x02, 0x04};
#endif

#if Uniboard == 3
  byte mac[] = {0x30, 0x16, 0x00, 0x00, 0x02, 0x06};
#endif

#if Uniboard == 4
  byte mac[] = {0x30, 0x16, 0x00, 0x00, 0x02, 0x07};
#endif

#if Uniboard == 5
  byte mac[] = {0x30, 0x16, 0x00, 0x00, 0x02, 0x08};
#endif
  //мак адреси для гб ідуть формату 30:16:00:00:00:ХХ
  // для гр 30:16:00:00:01:ХХ
  // для трону 30:16:00:00:02:ХХ
  Ethernet.init(PB12);
  EthernetClient client;
  SEFL::Logger::verbose("main", "Inited ethernet");

  SEFL::Logger::verbose("main", "Starting MQTT_Manager instance");

  SEFL::MQTTClientObjectBound<SEFL::Quest_Board_Manager> mqttclient(1024);
  SEFL::clbwrapobj = &mqttclient;

  SEFL::Logger::verbose("main", "Started MQTT_Manager instance");

  SEFL::Logger::verbose("main", "Starting ethernet");
  Ethernet.begin(mac);
  mqttclient.begin(SEFL::EDMONTON_MQTT_CONFIG.IP, client);
  SEFL::Logger::verbose("main", "Started ethernet");

  SEFL::Logger::verbose("main", "Starting Quest_Board_Manager instance");

#if Uniboard == 1
  SEFL::Quest_Board_Manager b_manager(mqttclient, SEFL::EDMONTON_MQTT_CONFIG, "U1", "tr22");
#endif

#if Uniboard == 2
  SEFL::Quest_Board_Manager b_manager(mqttclient, SEFL::EDMONTON_MQTT_CONFIG, "U2", "tr22");
#endif

#if Uniboard == 3
  SEFL::Quest_Board_Manager b_manager(mqttclient, SEFL::EDMONTON_MQTT_CONFIG, "U3", "tr22");
#endif

#if Uniboard == 4
  SEFL::Quest_Board_Manager b_manager(mqttclient, SEFL::EDMONTON_MQTT_CONFIG, "U4", "tr22");
#endif

#if Uniboard == 5
  SEFL::Quest_Board_Manager b_manager(mqttclient, SEFL::EDMONTON_MQTT_CONFIG, "U5", "tr22");
#endif

  SEFL::Logger::verbose("main", "Started Quest_Board_Manager instance");

  //тут створюються всі об'єкти всіх віртуальних пристроїв за прикладом вище
  ////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////

  // board 1
#if Uniboard == 1
  LEDMatrixPuzzleNeopixelKeyboard matrix(mqttclient, "matrix", 1, "tr22");
  const uint8_t matrix_pext_pins[] = {
            0, 1, 2};
  const uint8_t matrix_mext_pins[] = {
            0, 1, 2};
  const uint8_t matrix_correct_colors[] = {
      1, 4, 1, 4, 2, 2, 1, 2, 4};
  const uint8_t matrix_cols=3,matrix_rows=3;
  const char matrix_keyboard[matrix_cols*matrix_rows]={
          '1','4','7',
          '2','5','8',
          '3','6','9',
  };
  const uint8_t matrix_strip_pin = 3;
  const uint8_t matrix_segment_size = 2;
  matrix.setKeyboardPextPins(matrix_pext_pins,sizeof (matrix_pext_pins));
  matrix.setKeyboardMextPins(matrix_mext_pins,sizeof (matrix_mext_pins));
  matrix.setKeyboardKeymap(matrix_keyboard, matrix_rows, matrix_cols);
  matrix.setPattern(matrix_correct_colors, sizeof(matrix_correct_colors));
  matrix.setStripPin(matrix_strip_pin);
  matrix.setStripSegmentSize(matrix_segment_size);

  RAMPuzzle rampuzzle(mqttclient,"rampuzzle",1,"tr22");
  const uint8_t ram_led_pins[]={3,4,5,6};
  const uint8_t ram_sensor_pins[]={4,5,6,7};
  rampuzzle.setLedPins(ram_led_pins,sizeof(ram_led_pins));
  rampuzzle.setSensorPins(ram_sensor_pins,sizeof(ram_sensor_pins));

#endif
// board 2
#if Uniboard == 2
    GammaPuzzle gammapuzzle(mqttclient,"gammapuzzle");
    const uint8_t gamma_buttons[]={
            0,1,2,3,4,5,6
    };
    const uint8_t gamma_neons[]={
            0,1,2,3,4,5,6
    };
    const int8_t gamma_correct_order[]={
            0,1, 2,3,4,5,6
    };
    uint32_t gamma_correct_timeout=2000;
    uint32_t gamma_incorrect_timeout=2000;
    gammapuzzle.setButtonsPins(gamma_buttons,sizeof(gamma_buttons));
    gammapuzzle.setNeonsPins(gamma_neons,sizeof(gamma_neons));
    gammapuzzle.setCorrectOrder(gamma_correct_order,sizeof (gamma_correct_order));
    gammapuzzle.setCorrectAnimationTimeout(gamma_correct_timeout);
    gammapuzzle.setIncorrectAnimationTimeout(gamma_incorrect_timeout);

#endif

// board 3
#if Uniboard == 3

#endif

// board 4
#if Uniboard == 4

#endif

// board 5
#if Uniboard == 5

#endif

  //тут створюються всі об'єкти всіх віртуальних пристроїв за прикладом вище

  SEFL::Logger::notice("main", "Starting Quest_Start_Button instance");

  //	тут об'єкти пристроїв додаються до менеджера плати за прикладом
// Board 1
#if Uniboard == 1

    b_manager.addClient(&matrix);
    b_manager.addClient(&rampuzzle);

#endif

  // board 2
#if Uniboard == 2

#endif

  // board 3
#if Uniboard == 3

#endif

  // board 4
#if Uniboard == 4

#endif

  // board 5
#if Uniboard == 5

#endif

  //	тут об'єкти пристроїв додаються до менеджера плати за прикладом вище

  while (1)
  {
    b_manager.loop();
//    Logger::notice("test_keyboard",test_keyboard.getKey());
//    delay(500);
  }
}
void loop()
{
}
