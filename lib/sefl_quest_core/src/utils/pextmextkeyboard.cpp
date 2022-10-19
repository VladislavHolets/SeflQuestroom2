//
// Created by vladi on 18.10.2022.
//

#include "pextmextkeyboard.h"

namespace SEFL {
    PextMextKeyboard::PextMextKeyboard() {}

    void PextMextKeyboard::setPextPins(uint8_t *pextPins,uint8_t pextPinsSize) {
        if (pext_pins!= nullptr){
            delete pext_pins;
        }
        pext_pins_size=pextPinsSize;
        pext_pins=new uint8_t [pext_pins_size];
        for(int i=0;i<pext_pins_size;i++){
            pext_pins[i]=pextPins[i];
        }
    }

    void PextMextKeyboard::setMextPins(uint8_t *mextPins,uint8_t mextPinsSize) {
        if (mext_pins!= nullptr){
            delete mext_pins;
        }
        mext_pins_size=mextPinsSize;
        mext_pins=new uint8_t [mext_pins_size];
        for(int i=0;i<mext_pins_size;i++){
            mext_pins[i]=mextPins[i];
        }

    }

    void PextMextKeyboard::setKeymap(char **keyMap, uint8_t keymapRowSize,uint8_t keymapCollSize) {
        PextMextKeyboard::key_map = keyMap;
        if (key_map!= nullptr){

            for(int i=0;i<keymap_row_size;i++){
                delete key_map[i];
            }
            delete key_map;
        }
        keymap_row_size = keymapRowSize;
        keymap_coll_size = keymapCollSize;
        key_map=new char*[keymap_row_size];
        for(int i=0;i<keymap_row_size;i++){
            key_map[i] =new char [keymap_coll_size];
            for(int j=0;j<keymap_coll_size;j++){
                key_map[i][j]=keyMap[i][j];
            }
        }

    }

    int PextMextKeyboard::scanKeys() {
        int temp_state = 0;
            pinMode(Mext.getCi(), INPUT_PULLUP);
        for (int c = pext_pins_size - 1; c >= 0; c--) {
            Pext.digitalWrite(pext_pins[c], LOW);
            for (int r = mext_pins_size - 1; r >= 0; r--) {
                temp_state = (temp_state << 1) | (!Mext.digitalRead(mext_pins[r]) & 1);
            }
            Pext.digitalWrite(pext_pins[c], HIGH);
        }
        return temp_state;
    }

    bool PextMextKeyboard::pollEvent() {
        int temp_state = scanKeys();
        int lastState = state;
        int i = 0;
        for(int r=0;r<keymap_row_size;r++){
            for(int c=0;c<keymap_coll_size;c++){
                if (temp_state & 1 && !(lastState & 1)) {
                    event.type = KB_KEYDOWN;
                    event.c = key_map[r][c];
                    state |= 1 << i;
                    return true;
                } else if (!(temp_state & 1) && lastState & 1) {
                    event.type = KB_KEYUP;
                    event.c = key_map[r][c];
                    state &= ~(1 << i);
                    return true;
                }
                temp_state = temp_state >> 1;
                lastState = lastState >> 1;
            }
        }
        event.type = KB_NONE;
        event.c = 0;
        return false;
    }

    char PextMextKeyboard::getKey() {
        if (pollEvent() && event.type == KB_KEYDOWN) {
            return event.c;
        }
        return 0;
    }


} // SEFL