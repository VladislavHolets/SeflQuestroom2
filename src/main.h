#include <Arduino.h>
#include <DFRobotDFPlayerMini.h>
#include <SoftwareSerial.h>
#include <Ethernet.h>
#include <EthernetClient.h>
#include <PCA9685.h>
#include <PinNames.h>
#include <variant.h>
#include <wiring_constants.h>
#include <wiring_digital.h>
#include <wiring_time.h>
#include <Wire.h>
#include <sefl_quest_core.h>
#include "tron_clients/tronclients.h"
#include "common_clients/common_clients.h"
#define TRON_ROOM
#define Uniboard (7)
using namespace SEFL;
