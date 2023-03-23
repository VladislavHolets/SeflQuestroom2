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
    SEFL::Logger::getInstance()->setLogLevel(SEFL::Logger::Level::NOTICE); // VERBOSE   NOTICE SILENT
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
  //мак адреси для гб ідуть формату 30:16:00:00:00:ХХ
  // для гр 30:16:00:00:01:ХХ
#if defined(GHOST_RIDER_ROOM)
    const char placement[] = "gr22";
#endif
#if Uniboard == 1
  byte mac[] = {0x30, 0x16, 0x00, 0x00, 0x01, 0x01};
  const char uniboard_name[]="U1";
#endif

#if Uniboard == 2
  byte mac[] = {0x30, 0x16, 0x00, 0x00, 0x01, 0x01};
  const char uniboard_name[]="U2";
#endif

#if Uniboard == 3
  byte mac[] = {0x30, 0x16, 0x00, 0x00, 0x01, 0x03};
  const char uniboard_name[]="U3";
#endif

#if Uniboard == 4
  byte mac[] = {0x30, 0x16, 0x00, 0x00, 0x01, 0x04};
  const char uniboard_name[]="U4";
#endif

#if Uniboard == 5
  byte mac[] = {0x30, 0x16, 0x00, 0x00, 0x01, 0x05};
  const char uniboard_name[]="U5";
#endif
    /*

  #if Uniboard == 10
    byte mac[] = { 0x30, 0x16, 0x00, 0x00, 0x00, 0x10 };
     const char uniboard_name[]="U10";
  #endif
  */
    Ethernet.init(PB12);
    EthernetClient client;
    SEFL::Logger::verbose("main", "Inited ethernet");

    SEFL::Logger::verbose("main", "Starting MQTT_Manager instance");

    SEFL::MQTTClientObjectBound<SEFL::Quest_Board_Manager> mqttclient(1024);
    SEFL::clbwrapobj = &mqttclient;

    SEFL::Logger::verbose("main", "Started MQTT_Manager instance");

    SEFL::Logger::verbose("main", "Starting ethernet");
    Ethernet.begin(mac);
    mqttclient.begin(SEFL::RYBALSKA3_MQTT_CONFIG.IP, client);
    SEFL::Logger::verbose("main", "Started ethernet");

    SEFL::Logger::verbose("main", "Starting Quest_Board_Manager instance");

    SEFL::Quest_Board_Manager b_manager(mqttclient, SEFL::RYBALSKA3_MQTT_CONFIG, uniboard_name, placement);

    SEFL::Logger::verbose("main", "Started Quest_Board_Manager instance");

  //тут створюються всі об'єкти всіх віртуальних пристроїв за прикладом вище
  ////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////

  // board 1
#if Uniboard == 1
  SEFL::Quest_Start_Button start_button(mqttclient, 0, "hardware_start_button", 2, "gr22");

  SEFL::Magnet entrance_magnet(mqttclient, "entrance_magnet", 1, "gr22");
  entrance_magnet.setInverted(true);
  entrance_magnet.setPinNumber(0);

  SEFL::Crypt crypt(mqttclient, "crypt", 1, "gr22");
  crypt.setRightTorch(1);
  crypt.setRightRelay(1);
  crypt.setLeftTorch(2);
  crypt.setLeftRelay(2);

  SEFL::Magnet portal_magnet(mqttclient, "portal_magnet", 2, "gr22");
  portal_magnet.setInverted(true);
  portal_magnet.setPinNumber(3);

  SEFL::Magnet main_light(mqttclient, "main_light", 1, "gr22");
  main_light.setInverted(false);
  main_light.setPinNumber(4);

  SEFL::Tombstone tombstone(mqttclient, "tombstone", 1, "gr22");
  tombstone.setMagnetTombstone(5);
  tombstone.setMotojacketButton(4);

  tombstone.setInverted(false);

  SEFL::Contract contract(mqttclient, "contract", 1, "gr22");
  contract.setHellLight(6);
  contract.setHellMagnet(7);
  contract.setSwithContract(5);
  contract.setSmokeButton(8);
  contract.setInvertet(true);

#endif
// board 2
#if Uniboard == 2

  SEFL::Underwater underwater(mqttclient, "underwater", 1, "gr22");

  // setup player
  underwater.setPlayer(&player);

  // setup inputs
  underwater.setEncoderLeftA(0);
  // underwater.setEncoderleftB(1);
  underwater.setEncoderRightA(9);
  // underwater.setEncoderRightB(3);
  underwater.setButtonVolume(4);

  underwater.setLevelWater(5);

  // setup outputs
  // underwater.setRgbLightUnderwatherR(12);
  // underwater.setRgbLightUnderwatherG(13);
  underwater.setRgbLightUnderwatherB(14);
  underwater.setPump(4);
  underwater.setInverted(true);

  SEFL::Tornado tornado(mqttclient, "tornado", 1, "gr22");

  // setup player
  tornado.setPlayer(&player);

  // setup inputs
  tornado.setPotLt(6);
  tornado.setPotLc(7);
  tornado.setPotLb(8);

  // setup outputs
  tornado.setTornadoMagnet(3);

  tornado.setMotorLeft(5);
  tornado.setMotorCentre(6);
  tornado.setMotorRight(7);

  //	tornado.setRgbLightUnderwatherR(12);
  //	tornado.setRgbLightUnderwatherG(13);
  tornado.setRgbLightUnderwatherB(14);

  tornado.setRgbLightTornadoR(9);
  tornado.setRgbLightTornadoG(10);
  tornado.setRgbLightTornadoB(11);

  SEFL::Magnet elevator_in_magnet(mqttclient, "elevator_in_magnet", 2, "gr22");
  elevator_in_magnet.setInverted(true);
  elevator_in_magnet.setPinNumber(8);

  SEFL::Magnet elevator_out_magnet(mqttclient, "elevator_out_magnet", 1, "gr22");
  elevator_out_magnet.setInverted(true);
  elevator_out_magnet.setPinNumber(0);

  SEFL::Elevatorin elevatorin(mqttclient, "elevator_in", 1, "gr22");

  // setup player
  elevatorin.setPlayer(&player);

  // setup inputs
  elevatorin.setElevatorInSwith(12);
  elevatorin.setElevatorOutSwith(13);
  elevatorin.setElevatorInButton(14);
  elevatorin.setOutMagnet(&elevator_out_magnet);
  elevatorin.setInMagnet(&elevator_in_magnet);
  // setup outputs
  elevatorin.setElevatorInLed(2);

  SEFL::Elevatorout elevatorout(mqttclient, "elevator_out", 1, "gr22");

  // setup player
  elevatorout.setPlayer(&player);

  // setup inputs
  elevatorout.setElevatorInSwith(12);
  elevatorout.setElevatorOutSwith(13);
  elevatorout.setOutMagnet(&elevator_out_magnet);
  elevatorout.setInMagnet(&elevator_in_magnet);
  // setup outputs

  SEFL::Labyrinth labyrinth(mqttclient, "labyrinth", 1, "gr22");

  // setup player
  labyrinth.setPlayer(&player);

  // setup inputs

  labyrinth.setLabyrinthUpButton(11);
  labyrinth.setLabyrinthDownButton(10);
  labyrinth.setLabyrinthLeftButton(2);  //	PA7    SDA
  labyrinth.setLabyrinthRightButton(3); // 2 PA6    SCL
  labyrinth.setInMagnet(&elevator_in_magnet);

  labyrinth.setElevatorInSwith(12);
  labyrinth.setElevatorOutSwith(13);

  SEFL::Home home(mqttclient, "home", 1, "gr22");

  //			 setup outputs

  //		labyrinth.setLabyrinthAddressLed(12); ---------->>  PB7
  //
  //		Adafruit_NeoPixel labyrinth_pixels(36, 12, NEO_GRB + NEO_KHZ800);
  //		labyrinth_pixels.begin();

#endif

// board 3
#if Uniboard == 3

  SEFL::Magnet flat_light(mqttclient, "flat_light", 1, "gr22");
  flat_light.setInverted(true);
  flat_light.setPinNumber(12);

  SEFL::Magnet garage_light(mqttclient, "garage_light", 1, "gr22");
  garage_light.setInverted(true);
  garage_light.setPinNumber(11);

  SEFL::Motorcycles motorcycles(mqttclient, "motorcycles", 1, "gr22");
  // setup player
  motorcycles.setPlayer(&player);
  // setup inputs
  motorcycles.setPotL(1);
  motorcycles.setPotC(3);
  motorcycles.setPotR(5);

  motorcycles.setArduinoIn(11);

  // setup outputs
  motorcycles.setMotorcyclesAddressLed(14); // ---->> PB_2 ближайший к дф плееру
  motorcycles.setArduinoPower(6);

  SEFL::Keyboard keyboard(mqttclient, "keyboard", 1, "gr22");
  // setup player
  keyboard.setPlayer(&player);
  // setup inputs
  keyboard.setKeyboardPin(7);
  // setup outputs
  keyboard.setKeyboardMagnet(1);
  keyboard.setFireplaceSmokeRelay(13);

  SEFL::Fireplace fireplace(mqttclient, "fireplace", 1, "gr22");
  // setup player
  fireplace.setPlayer(&player);
  // setup inputs
  fireplace.setFireplaceButtont(9);
  fireplace.setLiquidLevelSensor(15);
  // setup outputs
  fireplace.setFireplaceAcLightRelay(14);
  fireplace.setFireplaceSmokeRelay(13);
  fireplace.setFireplaceCoolerOut(4);
  fireplace.setFireplaceLedOut(5);
  fireplace.setLiquidLevelLed(0);

#endif

// board 4
#if Uniboard == 4

  SEFL::Magnet garage_magnet(mqttclient, "garage_magnet", 2, "gr22");
  garage_magnet.setInverted(true);
  garage_magnet.setPinNumber(0);

  SEFL::Magnet sportbike_light(mqttclient, "sportbike_light", 1, "gr22");
  sportbike_light.setInverted(true);
  sportbike_light.setPinNumber(5);

  SEFL::Sportbike sportbike(mqttclient, "sport_bike", 1, "gr22");
  // setup player
  sportbike.setPlayer(&player);
  // setup inputs
  sportbike.setSportbikeKey(0);
  sportbike.setSportbikeLeftButton(1);
  sportbike.setSportbikeRightButton(2);
  sportbike.setSportbikeThrottle(3);
  sportbike.setSportbikeRideLightButton(6);

  // setup outputs
  sportbike.setSportbikeLeftRelay(2);
  sportbike.setSportbikeRightRelay(3);
  sportbike.setSportbikeRideLight(4);
  sportbike.setSportbikePowerRelay(1);
  sportbike.setSportbikeRightRelayFront(6);
  sportbike.setSportbikeLeftRelayFront(7);
  sportbike.setDashboardLight(5);
  sportbike.setTachometerPower(10);

  SEFL::Sportbikeride sportbikeride(mqttclient, "sport_bike_ride", 1, "gr22");
  // setup player
  sportbikeride.setPlayer(&player);
  // setup inputs

  // setup outputs
  sportbikeride.setSportbikeRideLight(4);
  sportbikeride.setDashboardLight(5); // 5

  sportbikeride.setSportbikeLeftRelay(2);
  sportbikeride.setSportbikeRightRelay(3);
  sportbikeride.setSportbikeRightRelayFront(6);
  sportbikeride.setSportbikeLeftRelayFront(7);

  sportbikeride.setSportbikePowerRelay(1); // 1
  sportbikeride.setTachometerPower(10);
  sportbikeride.setSportbikePowerServo(8);

  sportbikeride.setSportbikeServo(9);

#endif

// board 5
#if Uniboard == 5

  SEFL::Magnet cargoLift_magnet(mqttclient, "cargo_lift_magnet", 2, "gr22");
  cargoLift_magnet.setInverted(true);
  cargoLift_magnet.setPinNumber(1);

  SEFL::Chains chains(mqttclient, "chains", 1, "gr22");
  // setup player

  // setup inputs
  chains.setChainsLeftButton(0);
  chains.setChainsRightButton(1);

  // setup outputs
  chains.setChainsLeftMagnet(2);
  chains.setChainsRightMagnet(0);

  SEFL::ElevatorChopperDown elevatorChopperDown(mqttclient, "elevator_chopper_down", 1, "gr22");
  // setup player

  // setup inputs
  elevatorChopperDown.setCargoLiftSwitch(2);
  elevatorChopperDown.setWinchBottomSwitch(3);

  // setup outputs
  elevatorChopperDown.setWinchOnRelay(14);
  elevatorChopperDown.setWinchDownRelay(13);
  elevatorChopperDown.setFabricMagnet(5);
  elevatorChopperDown.setChopperLight(4);

  SEFL::ElevatorChopperUp elevatorChopperUp(mqttclient, "elevator_chopper_up", 1, "gr22");
  // setup player

  // setup inputs
  elevatorChopperUp.setCargoLiftUSwitch(2);
  elevatorChopperUp.setWinchTopSwitch(4);

  // setup outputs
  elevatorChopperUp.setWinchOnRelay(14);
  elevatorChopperUp.setWinchUpRelay(12);

  SEFL::Chopper chopper(mqttclient, "chopper", 1, "gr22");
  // setup player
  chopper.setPlayer(&player);
  // setup inputs
  chopper.setChopperCharger(5);
  chopper.setChopperStarter(6);
  // setup outputs
  chopper.setChopperLight(4);
  chopper.setChopperAddressLed(6);

  SEFL::Chopperride chopperride(mqttclient, "chopper_ride", 1, "gr22");
  // setup player
  chopperride.setPlayer(&player);
  // setup inputs
  chopperride.setChopperStarter(6);
  chopperride.setChopperLeftButton(7);
  chopperride.setChopperRightButton(8);
  chopperride.setChopperThrottle(11);
  // setup outputs
  chopperride.setChopperLight(4);
  chopperride.setChopperAddressLed(6);
  chopperride.setContactMagnet(8);

  SEFL::Magnet cargo_light(mqttclient, "chopper_light", 1, "gr22");
  cargo_light.setInverted(false);
  cargo_light.setPinNumber(11);

#endif

  /*
  #if Uniboard == 10

    uint8_t magnet_pin = 4;
    uint8_t sensor_pins[4] = { 0, 1, 2, 3 };
    uint8_t servo_pins[4] = { 0, 1, 2, 3 };
    SEFL::Bookshelf book_shelf(mqttclient, servo_pins, sensor_pins, magnet_pin,
        "book_shelf", 1, "gb21","in","out",SEFL::Language::ENG);
    SEFL::Magnet entrance(mqttclient,"entrance",5,"gb21");
    entrance.setInverted(true);
    entrance.setPinNumber(5);
    SEFL::Magnet book_shelf_light(mqttclient,"book_shelf_light",5,"gb21");
    book_shelf_light.setPinNumber(6);
    book_shelf_light.setInverted(true);

  #endif
  */
  /////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////
  //тут створюються всі об'єкти всіх віртуальних пристроїв за прикладом вище

  SEFL::Logger::notice("main", "Starting Quest_Start_Button instance");

  //	тут об'єкти пристроїв додаються до менеджера плати за прикладом
// Board 1
#if Uniboard == 1
  b_manager.addClient(&start_button);
  b_manager.addClient(&entrance_magnet);
  b_manager.addClient(&crypt);
  b_manager.addClient(&portal_magnet);
  b_manager.addClient(&main_light);
  b_manager.addClient(&tombstone);
  b_manager.addClient(&contract);

#endif

  // board 2
#if Uniboard == 2
  b_manager.addClient(&underwater);
  b_manager.addClient(&tornado);
  b_manager.addClient(&elevator_in_magnet);
  b_manager.addClient(&elevator_out_magnet);
  b_manager.addClient(&elevatorin);
  b_manager.addClient(&elevatorout);
  b_manager.addClient(&labyrinth);
  b_manager.addClient(&home);
#endif

  // board 3
#if Uniboard == 3

  b_manager.addClient(&flat_light);
  b_manager.addClient(&garage_light);
  b_manager.addClient(&motorcycles);
  b_manager.addClient(&keyboard);
  b_manager.addClient(&fireplace);

#endif

  // board 4
#if Uniboard == 4
  b_manager.addClient(&garage_magnet);
  // b_manager.addClient(&garage_light);
  b_manager.addClient(&sportbike);
  b_manager.addClient(&sportbikeride);

#endif

  // board 5
#if Uniboard == 5
  b_manager.addClient(&cargoLift_magnet);
  b_manager.addClient(&chains);
  b_manager.addClient(&elevatorChopperDown);
  b_manager.addClient(&elevatorChopperUp);
  b_manager.addClient(&chopper);
  b_manager.addClient(&chopperride);
  b_manager.addClient(&cargo_light);

#endif

  /*
  #if Uniboard == 10
    b_manager.addClient(&book_shelf);
    b_manager.addClient(&entrance);
    b_manager.addClient(&book_shelf_light);

    #endif
  */

  //	тут об'єкти пристроїв додаються до менеджера плати за прикладом вище

  while (1)
  {
    b_manager.loop();

    //	SEFL::Logger::verbose("loop","alive");
    //	SEFL::Logger::verbose("loop",millis());
  }
}
void loop()
{
}
