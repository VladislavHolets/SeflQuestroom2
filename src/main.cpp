
#include "main.h"

#ifdef USE_DFPLAYER
SoftwareSerial dfserial(PB_4, PB_3);
DFRobotDFPlayerMini player;
#endif
SoftwareSerial serial(PA_3, PA_2); // PA_12
void setup() {

    SPI.setMOSI(PB15);
    SPI.setMISO(PB14);
    SPI.setSCLK(PB13);

    pinMode(PA1, OUTPUT);
    digitalWrite(PA1, LOW);

    serial.begin(9600);
    delay(100);
    SEFL::Logger::setPrinter(&serial);
    SEFL::Logger::setLogLevel(SEFL::Logger::Level::WARNING); // VERBOSE   NOTICE SILENT
    //SEFL::Logger::setPostMessage();
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
    const char placement[] = "tr22";
#endif
#if Uniboard == 1
    byte mac[] = {0x30, 0x16, 0x00, 0x00, 0x02, 0x01};
    const char uniboard_name[]="U1";

#endif

#if Uniboard == 2
    byte mac[] = {0x30, 0x16, 0x00, 0x00, 0x02, 0x02};
    const char uniboard_name[] = "U2";
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
    const char uniboard_name[] = "U9";
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
    LEDMatrixPuzzleNeopixelKeyboard matrix(mqttclient, "matrix_puzzle", 1, placement);
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

  RAMPuzzle rampuzzle(mqttclient,"ram_puzzle",1,placement);
  const uint8_t ram_led_pins[]={3,4,5,6};
  const uint8_t ram_sensor_pins[]={4,5,6,7};
  rampuzzle.setLedPins(ram_led_pins,sizeof(ram_led_pins));
  rampuzzle.setSensorPins(ram_sensor_pins,sizeof(ram_sensor_pins));
  TronLegacyPuzzle chips_puzzle(mqttclient,"chips_puzzle",1,placement);
  TronLegacyAdapter chips_adapter;
  chips_adapter.setResetPin(7);
  chips_adapter.setManualPin(8);
  chips_adapter.setSolvedStatePin(8);
  chips_adapter.setResetTimeout(3000);
  chips_puzzle.setAdapter(chips_adapter);
#endif
// board 2
#if Uniboard == 2
    /*
     * гамма
     * таймери 2
     * аларм лампи 2
     * сонік дальномір
     * лазерний промінь
     *
     *
     */
        GammaPuzzle gamma_puzzle(mqttclient, "gammapuzzle", 1, placement);
        const uint8_t gamma_buttons[] = {
                0, 1, 2, 3, 4, 5, 6
        };
        const uint8_t gamma_neons[] = {
                6,1, 2, 3, 4, 5, 0
        };
        const int8_t gamma_correct_order[] = {
                0, 5, 3, 1, 4, 2,6
        };
        uint32_t gamma_correct_timeout = 5000;
        uint32_t gamma_incorrect_timeout = 2000;
        gamma_puzzle.setButtonsPins(gamma_buttons, sizeof(gamma_buttons));
        gamma_puzzle.setNeonsPins(gamma_neons, sizeof(gamma_neons));
        gamma_puzzle.setCorrectOrder(gamma_correct_order, sizeof(gamma_correct_order));
        gamma_puzzle.setCorrectAnimationTimeout(gamma_correct_timeout);
        gamma_puzzle.setIncorrectAnimationTimeout(gamma_incorrect_timeout);
        //HC595_Driver driver(uext_config,2);

        /*******************************************************************************************************************
         * BEGINING of HC595 dependent classes initialisation!
         ******************************************************************************************************************/

        HC595_cfg cfg{};
        cfg.data_pin = uext_config.MOSI;
        cfg.latch_pin = uext_config.SCLK;
        cfg.clock_pin = uext_config.SSEL;
        cfg.chip_amount =4;
        HC595_Driver hc595_driver(cfg.data_pin, cfg.clock_pin, cfg.latch_pin, cfg.chip_amount);
        Tron_Segment_Timer timer_1(mqttclient, "timer_1", 1, placement);
        timer_1.setDriver(hc595_driver);
        timer_1.setOverflowPeriod(60000);
        timer_1.setStartingValue(60);
        timer_1.setStoppingValue(0);
        timer_1.setIncreasingOrder(false);
        uint8_t timer_1_segments[]{
                2, 3
        };
        uint8_t timer_1_segments_base[]{
                DEC, DEC
        };
        timer_1.setSegments(timer_1_segments, sizeof(timer_1_segments));
        timer_1.setSegmentsBase(timer_1_segments_base, sizeof(timer_1_segments_base));


        Tron_Segment_Timer timer_2(mqttclient, "timer_2", 1, placement);
        timer_2.setDriver(hc595_driver);
        timer_2.setOverflowPeriod(60000);
        timer_2.setStartingValue(60);
        timer_2.setStoppingValue(0);
        timer_2.setIncreasingOrder(false);
        uint8_t timer_2_segments[]{
                0, 1
        };
        uint8_t timer_2_segments_base[]{
                DEC, DEC
        };
        timer_2.setSegments(timer_2_segments, sizeof(timer_2_segments));
        timer_2.setSegmentsBase(timer_2_segments_base, sizeof(timer_2_segments_base));

        /*******************************************************************************************************************
         * ENDING of HC595 dependent classes initialisation!
         ******************************************************************************************************************/


        TronLegacyAdapter sonic_adapter;
        sonic_adapter.setResetPin(7);
        sonic_adapter.setManualPin(8);
        sonic_adapter.setSolvedStatePin(7);
        TronLegacyPuzzle sonic_range_finder(mqttclient, "sonic_range_finder",1,placement);
        sonic_range_finder.setAdapter(sonic_adapter);
        PlatonicBodies platonic_bodies(mqttclient,"platonic_bodies",1,placement);
        platonic_bodies.setLaserPin(11);
        platonic_bodies.setSensorPin(8);
        platonic_bodies.setSensorSignalInverted(false);
        TronCubeChest cube_chest_1(mqttclient,"cube_chest_1",1,placement);
        cube_chest_1.setLedPin(13);
        cube_chest_1.setMagnetPin(12);
        Magnet alarm_1(mqttclient,"alarm_1",1,placement);
        alarm_1.setPinNumber(14);
        alarm_1.setInverted(false);

#endif

// board 3
#if Uniboard == 3
    Generator generator_puzzle(mqttclient,"generator_puzzle",1,placement);
    generator_puzzle.setCorrectAnimationTimeout(1500);
    generator_puzzle.setSolvedAnimationTimeout(10000);
    uint8_t generator_wires_pins[]{
            0,1,2
    };
    uint8_t generator_correct_pattern[]{
            0,0,0
    };
    int16_t motor_pins[] {
        12,13,14
    };
    //4
    generator_puzzle.setWiresPins(generator_wires_pins,sizeof (generator_wires_pins));
    generator_puzzle.setCorrectPattern(generator_correct_pattern,sizeof (generator_correct_pattern));
    generator_puzzle.setFrameLightPin(1);
    generator_puzzle.setGeneratorLedsPin(0);
    generator_puzzle.setMotorsEnablePin(4);
    generator_puzzle.setGeneratorMotorsPin(motor_pins, sizeof(motor_pins)/sizeof(motor_pins[0]));



    //Mext white, Pext green

    ANDPuzzle and_puzzle(mqttclient,"&_puzzle",1,"tr22");
    TronLegacyAdapter tronLegacyAdapter;
    tronLegacyAdapter.setResetPin(6);
    tronLegacyAdapter.setManualPin(7);
    tronLegacyAdapter.setSolvedStatePin(3);
    tronLegacyAdapter.setPowerPin(-1);
    and_puzzle.setAdapter(tronLegacyAdapter);
    and_puzzle.setLedOnTimeout(60000);
#endif

// board 4
#if Uniboard == 4
    /*
     * laser barier
     */

#endif

// board 5
#if Uniboard == 5
    /*
     * pressing floor
     */

    Panel panels[] =  {
            {.sensor_pin = 0, .led_pin = 1},
            {.sensor_pin = 1, .led_pin = 2},
            {.sensor_pin = 2, .led_pin = 3},
            {.sensor_pin = 3, .led_pin = 4},
            {.sensor_pin = 4, .led_pin = 5}

    };

    FloorPuzzle floor_puzzle(mqttclient, "floor_puzzle", 1, placement);
    floor_puzzle.setPanels(panels, sizeof(panels)/sizeof(panels[0]),0);
    floor_puzzle.setPatternSize(2);

    InfiniteMirror infiniteMirror(mqttclient, "infinite_mirror", 1, placement);
    int16_t mirror_cathodes[3]{12,13,14};
        infiniteMirror.setLedCathodes(mirror_cathodes,sizeof(mirror_cathodes)/sizeof(mirror_cathodes[0]));
#endif
#if Uniboard == 6
    //    StatusBarHC595 status_bar_1(mqttclient,"status_bar_1",1,placement);
//    status_bar_1.setDriver(hc595_driver);
//    status_bar_1.setStatusLampsChip(0);
//    const uint8_t status_bar_1_led_pins[]={
//            7,6,5,4,3,2
//    };
//    status_bar_1.setStatusLampsPins(status_bar_1_led_pins,sizeof(status_bar_1_led_pins));
//
//    StatusBarHC595 status_bar_2(mqttclient,"status_bar_2",1,placement);
//    status_bar_2.setDriver(hc595_driver);
//    status_bar_2.setStatusLampsChip(4);
//    const uint8_t status_bar_2_led_pins[]={
//            7,6,5,4,3,2
//    };
//    status_bar_2.setStatusLampsPins(status_bar_2_led_pins,sizeof(status_bar_2_led_pins));
#endif
#if Uniboard == 7
    DiskHolderArray disk_receiver_1(mqttclient,"disk_receiver_1",1,"tr22");
    HolderPins pins[6]{
            {6,0,0,0,500,140,0},
            {7,1,1,0,500,150,0},
            {8,2,2,0,500,180,0},
            {9,3,3,0,500,180,0},
            {10,4, 4,0,100,150,0},
            {11,5,5,0,100,140,0}
    };
    disk_receiver_1.setHolders(pins,6);
    disk_receiver_1.setHoldertype(SEFL::RECEIVER);

#endif
#if Uniboard == 8
    DiskHolderArray disk_dispenser_1(mqttclient,"disk_dispenser_1",1,"tr22");

 //   dispence_angle = 150;
 //   receive_angle = 0;
    HolderPins pins[6]{
            {6,0,0,0,950,140,0},
            {7,1,1,0,950,150,0},
            {8,2,2,0,980,180,0},
            {9,3,3,0,950,180,0},
            {10,4, 4,0,950,150,0},
            {11,5,5,0,950,140,0}
    };
    disk_dispenser_1.setHolders(pins,6);
    disk_dispenser_1.setHoldertype(SEFL::DISPENSER);
#endif
#if Uniboard == 9
    DiskHolderArray disk_receiver_2(mqttclient, "disk_receiver_2", 1, "tr22");
    HolderPins pins[6]{
            {6,  0, 0, 0, 950, 140, 0},
            {7,  1, 1, 0, 950, 150, 0},
            {8,  2, 2, 0, 980, 180, 0},
            {9,  3, 3, 0, 950, 180, 0},
            {10, 4, 4, 0, 950, 150, 0},
            {11, 5, 5, 0, 950, 140, 0}
    };
    disk_receiver_2.setHoldertype(SEFL::RECEIVER);
    disk_receiver_2.setHolders(pins, 6);

#endif
#if Uniboard == 10
    DiskHolderArray disk_dispenser_2(mqttclient,"disk_dispenser_2",1,"tr22");
    HolderPins pins[6]{
            {6,0,0,0,950,140,0},
            {7,1,1,0,950,150,0},
            {8,2,2,0,980,180,0},
            {9,3,3,0,950,180,0},
            {10,4, 4,0,950,150,0},
            {11,5,5,0,950,140,0}
    };
    disk_dispenser_2.setHoldertype(SEFL::DISPENSER);
    disk_dispenser_2 .setHolders(pins,6);
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


    //тут створюються всі об'єкти всіх вір+++туальних пристроїв за прикладом вище

    SEFL::Logger::notice("main", "Starting Quest_Start_Button instance");

    //	тут об'єкти пристроїв додаються до менеджера плати за прикладом
// Board 1
#if Uniboard == 1

    b_manager.addClient(matrix);
    b_manager.addClient(rampuzzle);
    b_manager.addClient(chips_puzzle);
#endif

    // board 2
#if Uniboard == 2
    b_manager.addClient(gamma_puzzle);
    b_manager.addClient(timer_1);
    b_manager.addClient(timer_2);
    b_manager.addClient(sonic_range_finder);
    b_manager.addClient(platonic_bodies);
    b_manager.addClient(cube_chest_1);
    b_manager.addClient(alarm_1);
#endif

    // board 3
#if Uniboard == 3
    b_manager.addClient(generator_puzzle);
    b_manager.addClient(and_puzzle);
#endif

    // board 4
#if Uniboard == 4
    b_manager.addClient(and_puzzle);
#endif

    // board 5
#if Uniboard == 5
    b_manager.addClient(floor_puzzle);
    b_manager.addClient(infiniteMirror);
#endif
#if Uniboard == 6

#endif
#if Uniboard == 7
    b_manager.addClient(disk_receiver_1);
#endif
#if Uniboard == 8
    b_manager.addClient(disk_dispenser_1);
#endif
#if Uniboard == 9
    b_manager.addClient(disk_receiver_2);
#endif
#if Uniboard == 10
    b_manager.addClient(disk_dispenser_2);
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

    Logger::warning("main", "init went fine, starting the loop");

    while (true) {
        b_manager.loop();
    }
}

void loop() {
}
