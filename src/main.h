//
// Created by vladi on 23.03.2023.
//
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
#include "ghost_rider_clients/ghostriderclients.h"
#include "common_clients/common_clients.h"
#define GHOST_RIDER_ROOM
#define Uniboard (2)
#define USE_DFPLAYER
using namespace SEFL;
