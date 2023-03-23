/*
 * hc595driver.cpp
 *
 *  Created on: Aug 16, 2021
 *      Author: vladi
 */

#include "hc595driver.h"
#include "utils/logger.h"

namespace SEFL
{

} /* namespace SEFL */

void SEFL::HC595_Driver::shiftOut(uint16_t length)
{
    if (length > HC595_BUFFER_SIZE)
    {
        length = HC595_BUFFER_SIZE;
    }
    digitalWrite(cfg_.data_pin, 0);
    digitalWrite(cfg_.clock_pin, 0);
    //int pinState;
    for (int l = 0; l < length; l++)
    {
        for (int i = 0; i < 8; i++)
        {
            digitalWrite(cfg_.clock_pin, 0);

            digitalWrite(cfg_.data_pin,  (data[l] & (1 << (7 - i + 0))));
            delayMicroseconds(50);
            digitalWrite(cfg_.clock_pin, 1);

            delayMicroseconds(50);
            //digitalWrite(cfg_.data_pin, 0);
        }
    }

    digitalWrite(cfg_.clock_pin, 0);
}

void SEFL::HC595_Driver::setOrder(BitOrder order_)
{
    this->order_ = order_;
}


SEFL::HC595_Driver::HC595_Driver(uint8_t data_pin, uint8_t clock_pin,
                                 uint8_t latch_pin, int16_t chip_amount) : cfg_{data_pin, clock_pin, latch_pin, chip_amount}, order_(MSBFIRST), spi_(nullptr), using_spi(false),
                                                                            shared_spi(false) {
    data=new uint8_t[cfg_.chip_amount];
    data_length=cfg_.chip_amount;
}

SEFL::HC595_Driver::~HC595_Driver()
{
    if(!shared_spi && using_spi){
        if (spi_ != nullptr){
            delete spi_;
        }
    }
    delete [] data;
}

void SEFL::HC595_Driver::setDataItem(unsigned int position, bool state)
{
    if (position >= this->cfg_.chip_amount * 8)
    {
        return;
    }
    if (state) {
        this->data[position / 8] |= 1UL << (position -(position / 8) * 8);
    }
    else{
        this->data[position / 8] &= ~(1UL << (position - (position / 8)*8));
    }
}

void SEFL::HC595_Driver::cleanData()
{
    for (int i = 0; i < this->cfg_.chip_amount; i++)
    {
        this->data[i] = 0;
    }
}

void SEFL::HC595_Driver::setDataFrame(uint16_t position, uint8_t *data_frame, uint16_t length)
{
    if ((position + length) > this->cfg_.chip_amount)
    {
        return;
    }
    memcpy(this->data + position, data_frame, length);
}

void SEFL::HC595_Driver::sendData()
{

    pinMode(cfg_.clock_pin, OUTPUT);
    pinMode(cfg_.latch_pin, OUTPUT);
    pinMode(cfg_.data_pin, OUTPUT);
    digitalWrite(this->cfg_.latch_pin, LOW);
    if(using_spi){
        for(int i=0;i<data_length;i++)
        spi_->transfer(this->data[i]);
    }else {
        shiftOut(data_length);
    }
    digitalWrite(this->cfg_.latch_pin, HIGH);
}

SEFL::HC595_Driver::HC595_Driver(SEFL::UEXT_Config cfg, int16_t chip_amount) : using_spi(true), shared_spi(false) ,order_(MSBFIRST){
    spi_=new SPIClass(cfg.MOSI,cfg.MISO,cfg.SSEL);
    cfg_.clock_pin=cfg.SSEL;
    cfg_.chip_amount=chip_amount;
    data_length=chip_amount;
    data=new uint8_t[data_length];
    cfg_.latch_pin=cfg.SCLK;
    cfg_.data_pin=cfg.MOSI;
    spi_->begin();
}

SEFL::HC595_Driver::HC595_Driver(SPIClass& spi,HC595_cfg cfg):using_spi(true), shared_spi(true),order_(MSBFIRST),spi_(&spi){
    cfg_.clock_pin=cfg.clock_pin;
    cfg_.chip_amount=cfg.chip_amount;
    data_length=cfg.chip_amount;
    data=new uint8_t[data_length];
    cfg_.latch_pin=cfg.latch_pin;
    cfg_.data_pin=cfg.data_pin;
}

void SEFL::HC595_Driver::setDataByte(int16_t position, uint8_t state) {
    if (position >= data_length)
    {
        return;
    }
    this->data[position] =state;
}

uint8_t *SEFL::HC595_Driver::getData() const {
    return data;
}

int16_t SEFL::HC595_Driver::getDataLength() const {
    return data_length;
}
