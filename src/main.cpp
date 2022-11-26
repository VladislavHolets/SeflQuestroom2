
#include "main.h"
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
    SEFL::Logger::getInstance()->setLogLevel(SEFL::Logger::Level::SILENT); // VERBOSE   NOTICE SILENT9
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
#if defined(TRON_ROOM)
    const char placement[]="tr22";
#endif
#if Uniboard == 1
    byte mac[] = {0x30, 0x16, 0x00, 0x00, 0x02, 0x01};
    const char uniboard_name[]="U1";

#endif

#if Uniboard == 2
    byte mac[] = {0x30, 0x16, 0x00, 0x00, 0x02, 0x02};
    const char uniboard_name[]="U2";
#endif

#if Uniboard == 3
    byte mac[] = {0x30, 0x16, 0x00, 0x00, 0x02, 0x03};
    const char uniboard_name[]="U3";
#endif

#if Uniboard == 4
    byte mac[] = {0x30, 0x16, 0x00, 0x00, 0x02, 0x04};
    const char uniboard_name[]="U4";
#endif

#if Uniboard == 5
    byte mac[] = {0x30, 0x16, 0x00, 0x00, 0x02, 0x05};
    const char uniboard_name[]="U5";
#endif
#if Uniboard == 6
    byte mac[] = {0x30, 0x16, 0x00, 0x00, 0x02, 0x06};
    const char uniboard_name[]="U6";
#endif
#if Uniboard == 7
    byte mac[] = {0x30, 0x16, 0x00, 0x00, 0x02, 0x07};
    const char uniboard_name[]="U7";
#endif
#if Uniboard == 8
    byte mac[] = {0x30, 0x16, 0x00, 0x00, 0x02, 0x08};
    const char uniboard_name[]="U8";
#endif
#if Uniboard == 9
    byte mac[] = {0x30, 0x16, 0x00, 0x00, 0x02, 0x09};
    const char uniboard_name[]="U9";
#endif
#if Uniboard == 10
    byte mac[] = {0x30, 0x16, 0x00, 0x00, 0x02, 0x0A};
    const char uniboard_name[]="U10";
#endif
#if Uniboard == 11
    byte mac[] = {0x30, 0x16, 0x00, 0x00, 0x02, 0x0B};
    const char uniboard_name[]="U11";
#endif
#if Uniboard == 12
    byte mac[] = {0x30, 0x16, 0x00, 0x00, 0x02, 0x0C};
    const char uniboard_name[]="U12";
#endif
#if Uniboard == 13
    byte mac[] = {0x30, 0x16, 0x00, 0x00, 0x02, 0x0D};
    const char uniboard_name[]="U13";
#endif
#if Uniboard == 14
    byte mac[] = {0x30, 0x16, 0x00, 0x00, 0x02, 0x0E};
    const char uniboard_name[]="U14";
#endif
#if Uniboard == 15
    byte mac[] = {0x30, 0x16, 0x00, 0x00, 0x02, 0xF};
    const char uniboard_name[]="U15";
#endif
#if Uniboard == 16
    byte mac[] = {0x30, 0x16, 0x00, 0x00, 0x02, 0x10};
    const char uniboard_name[]="U16";
#endif
#if Uniboard == 17
    byte mac[] = {0x30, 0x16, 0x00, 0x00, 0x02, 0x11};
    const char uniboard_name[]="U17";
#endif
#if Uniboard == 18
    byte mac[] = {0x30, 0x16, 0x00, 0x00, 0x02, 0x12};
    const char uniboard_name[]="U18";
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

    SEFL::Quest_Board_Manager b_manager(mqttclient, SEFL::EDMONTON_MQTT_CONFIG, uniboard_name, placement);

    SEFL::Logger::verbose("main", "Started Quest_Board_Manager instance");

    //тут створюються всі об'єкти всіх віртуальних пристроїв за прикладом вище
    ////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    // board 1
#if Uniboard == 1
    LEDMatrixPuzzleNeopixelKeyboard matrix(mqttclient, "matrix", 1, placement);
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

  RAMPuzzle rampuzzle(mqttclient,"rampuzzle",1,placement);
  const uint8_t ram_led_pins[]={3,4,5,6};
  const uint8_t ram_sensor_pins[]={4,5,6,7};
  rampuzzle.setLedPins(ram_led_pins,sizeof(ram_led_pins));
  rampuzzle.setSensorPins(ram_sensor_pins,sizeof(ram_sensor_pins));

#endif
// board 2
#if Uniboard == 2
    GammaPuzzle gamma_puzzle(mqttclient,"gammapuzzle",1,placement);
    const uint8_t gamma_buttons[]={
            0,1,2,3,4,5,6
    };
    const uint8_t gamma_neons[]={
            0,1,2,3,4,5,6
    };
    const int8_t gamma_correct_order[]={
            0,1, 2,3,4,5,6
    };
    uint32_t gamma_correct_timeout=5000;
    uint32_t gamma_incorrect_timeout=2000;
    gamma_puzzle.setButtonsPins(gamma_buttons,sizeof(gamma_buttons));
    gamma_puzzle.setNeonsPins(gamma_neons,sizeof(gamma_neons));
    gamma_puzzle.setCorrectOrder(gamma_correct_order,sizeof (gamma_correct_order));
    gamma_puzzle.setCorrectAnimationTimeout(gamma_correct_timeout);
    gamma_puzzle.setIncorrectAnimationTimeout(gamma_incorrect_timeout);
    StatusBar status_bar_1(mqttclient,"statusbar_1",1,placement);
    const uint8_t status_bar_led_pins[]={
            9,10,11,12,13,14
    };
    status_bar_1.setStatusLampsPins(status_bar_led_pins,sizeof(status_bar_led_pins));
    //HC595_Driver driver(uext_config,2);
    HC595_cfg cfg{};
    cfg.data_pin=uext_config.MOSI;
    cfg.latch_pin=uext_config.SCLK;
    cfg.clock_pin=uext_config.SSEL;
    cfg.chip_amount=4;
    HC595_Driver hc595_driver(cfg.data_pin,cfg.clock_pin,cfg.latch_pin,cfg.chip_amount);
    Pext.digitalWrite(7,LOW);
    Tron_Segment_Timer timer_1(mqttclient,"timer_1",1,placement);
    timer_1.setDriver(hc595_driver);
    timer_1.setOverflowPeriod(60000);
    timer_1.setStartingValue(60);
    timer_1.setStoppingValue(0);
    timer_1.setIncreasingOrder(false);
    uint8_t timer_1_segments[]{
        2,3
    };
    uint8_t timer_1_segments_base[]{
        DEC,DEC
    };
    timer_1.setSegments(timer_1_segments,sizeof (timer_1_segments));
    timer_1.setSegmentsBase(timer_1_segments_base,sizeof (timer_1_segments_base));
    Tron_Segment_Timer timer_2(mqttclient,"timer_2",1,placement);
    timer_2.setDriver(hc595_driver);
    timer_2.setOverflowPeriod(60000);
    timer_2.setStartingValue(60);
    timer_2.setStoppingValue(0);
    timer_2.setIncreasingOrder(false);
    uint8_t timer_2_segments[]{
            0,1
    };
    uint8_t timer_2_segments_base[]{
            DEC,DEC
    };
    timer_2.setSegments(timer_2_segments,sizeof (timer_2_segments));
    timer_2.setSegmentsBase(timer_2_segments_base,sizeof (timer_2_segments_base));
#endif

// board 3
#if Uniboard == 3
    Generator generator_puzzle(mqttclient,"generator_puzzle",1,placement);
    generator_puzzle.setCorrectAnimationTimeout(1500);
    generator_puzzle.setSolvedAnimationTimeout(2000);
    uint8_t generator_wires_pins[]{
            0,1,2
    };
    uint8_t generator_correct_pattern[]{
            0,0,0
    };
    generator_puzzle.setWiresPins(generator_wires_pins,sizeof (generator_wires_pins));
    generator_puzzle.setCorrectPattern(generator_correct_pattern,sizeof (generator_correct_pattern));
    generator_puzzle.setFrameLightPin(0);
    generator_puzzle.setGeneratorLedsPin(1);
    generator_puzzle.setGeneratorMotorsPin(2);


#endif

// board 4
#if Uniboard == 4
//PextServo servo1;
//servo1.attach(0);
//while(1){
//    servo1.write(1);
//    delay(2000);
//    servo1.write(90);
//    delay(2000);
//    servo1.write(180);
//    delay(2000);
//    servo1.write(90);
//    delay(2000);
//}
#endif

// board 5
#if Uniboard == 5

#endif
#if Uniboard == 6

#endif
#if Uniboard == 7

#endif
#if Uniboard == 8

#endif
#if Uniboard == 9

#endif
#if Uniboard == 10

#endif
#if Uniboard == 11
    Quest_Tron_Target target_11(mqttclient,0,"target_11",1,placement);
    uint8_t target_11_led_pins[]{0,1,2,3,4,5,6,7,8,9,10};
    target_11.setLedPins(target_11_led_pins, sizeof(target_11_led_pins));
#endif
#if Uniboard == 12
    Quest_Tron_Target target_12(mqttclient,0,"target_12",1,placement);
    uint8_t target_12_led_pins[]{0,1,2,3,4,5,6,7,8,9,10};
    target_12.setLedPins(target_12_led_pins, sizeof(target_12_led_pins));
#endif
#if Uniboard == 13
    Quest_Tron_Target target_13(mqttclient,0,"target_13",1,placement);
    uint8_t target_13_led_pins[]{0,1,2,3,4,5,6,7,8,9,10};
    target_13.setLedPins(target_13_led_pins, sizeof(target_13_led_pins));
#endif
#if Uniboard == 14
    Quest_Tron_Target target_14(mqttclient,0,"target_14",1,placement);
    uint8_t target_14_led_pins[]{0,1,2,3,4,5,6,7,8,9,10};
    target_14.setLedPins(target_14_led_pins, sizeof(target_14_led_pins));
#endif
#if Uniboard == 15
    Quest_Tron_Target target_21(mqttclient,0,"target_21",1,placement);
    uint8_t target_21_led_pins[]{0,1,2,3,4,5,6,7,8,9,10};
    target_21.setLedPins(target_21_led_pins, sizeof(target_21_led_pins));
#endif
#if Uniboard == 16
    Quest_Tron_Target target_22(mqttclient,0,"target_22",1,placement);
    uint8_t target_22_led_pins[]{0,1,2,3,4,5,6,7,8,9,10};
    target_22.setLedPins(target_22_led_pins, sizeof(target_22_led_pins));
#endif
#if Uniboard == 17
    Quest_Tron_Target target_23(mqttclient,0,"target_23",1,placement);
    uint8_t target_23_led_pins[]{0,1,2,3,4,5,6,7,8,9,10};
    target_23.setLedPins(target_23_led_pins, sizeof(target_23_led_pins));
#endif
#if Uniboard == 18
    Quest_Tron_Target target_24(mqttclient,0,"target_24",1,placement);
    uint8_t target_24_led_pins[]{0,1,2,3,4,5,6,7,8,9,10};
    target_24.setLedPins(target_24_led_pins, sizeof(target_24_led_pins));
#endif


    //тут створюються всі об'єкти всіх віртуальних пристроїв за прикладом вище

    SEFL::Logger::notice("main", "Starting Quest_Start_Button instance");

    //	тут об'єкти пристроїв додаються до менеджера плати за прикладом
// Board 1
#if Uniboard == 1

    b_manager.addClient(matrix);
    b_manager.addClient(rampuzzle);

#endif

    // board 2
#if Uniboard == 2
    b_manager.addClient(gamma_puzzle);
    b_manager.addClient(status_bar_1);
    b_manager.addClient(timer_1);
    b_manager.addClient(timer_2);
#endif

    // board 3
#if Uniboard == 3
    b_manager.addClient(generator_puzzle);
#endif

    // board 4
#if Uniboard == 4

#endif

    // board 5
#if Uniboard == 5

#endif
#if Uniboard == 6

#endif
#if Uniboard == 7

#endif
#if Uniboard == 8

#endif
#if Uniboard == 9

#endif
#if Uniboard == 10

#endif
#if Uniboard == 11
    b_manager.addClient(target_11);
#endif
#if Uniboard == 12
    b_manager.addClient(target_12);
#endif
#if Uniboard == 13
    b_manager.addClient(target_13);
#endif
#if Uniboard == 14
    b_manager.addClient(target_14);
#endif
#if Uniboard == 15
    b_manager.addClient(target_21);
#endif
#if Uniboard == 16
    b_manager.addClient(target_22);
#endif
#if Uniboard == 17
    b_manager.addClient(target_23);
#endif
#if Uniboard == 18
    b_manager.addClient(target_24);
#endif


    //	тут об'єкти пристроїв додаються до менеджера плати за прикладом вище



    while (true)
    {
        b_manager.loop();
    }
}
void loop()
{
}
