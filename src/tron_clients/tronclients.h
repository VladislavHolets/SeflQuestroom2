//
// Created by vladi on 22.10.2022.
//
#ifndef SEFLQUESTROOM2_TRONCLIENTS_H
#define SEFLQUESTROOM2_TRONCLIENTS_H

#if defined(ARDUINO_ARCH_STM32)
#include "ledmatrixpuzzle.h"
#include "ledmatrixpuzzleneopixel.h"
#include "ledmatrixpuzzleneopixelkeyboard.h"
#include "rampuzzle.h"
#include "gammapuzzle.h"
#include "tronsegmenttimer.h"
#include "statusbar.h"
#include "statusbarhc595.h"
#include "generator.h"
#include "diskholder.h"
#include "andpuzzle.h"
#include "tronlegacypuzzle.h"
#include "floorpuzzle.h"
#include "infinitemirror.h"
#include "platonicbodies.h"
#include "laserbarier.h"
#include "troncubechest.h"
//#include ""
#endif
#if defined(ARDUINO_ARCH_ESP32)
#include "blasterclient.h"
#endif
#endif //SEFLQUESTROOM2_TRONCLIENTS_H
