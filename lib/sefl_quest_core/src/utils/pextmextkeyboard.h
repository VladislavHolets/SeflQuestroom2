//
// Created by vladi on 18.10.2022.
//

#ifndef SEFLQUESTROOM2_PEXTMEXTKEYBOARD_H
#define SEFLQUESTROOM2_PEXTMEXTKEYBOARD_H
#include <sefl_quest_core.h>
namespace SEFL {

    enum PextMextKeyboardEventTypes{
        KB_NONE =0,KB_KEYUP=1,KB_KEYDOWN=2,KB_TEXT=3,KB_MODE=4
    };
    enum PextMextKeyboardEventModes{
        KB_MODE_NONE=-1,KM_MODE_LOWER=0,KM_MODE_UPPER=1,KM_MODE_NUM=2
    };
    struct PextMextKeyboardEvent {
        byte type;
        char c;
    };

    class PextMextKeyboard {
    private:
        uint8_t * pext_pins;
        uint8_t * mext_pins;
        uint8_t pext_pins_size;
        uint8_t mext_pins_size;
        char  **key_map;
        uint8_t keymap_row_size,keymap_coll_size;
        int state;
    public:


    public:
        PextMextKeyboardEvent event;

        virtual ~PextMextKeyboard();

        PextMextKeyboard();

        void setPextPins(const uint8_t *pextPins, uint8_t pextPinsSize);
        void setMextPins(const uint8_t *mextPins, uint8_t mextPinsSize);
        void setKeymap(const char *keymap, uint8_t keymapRowSize, uint8_t keymapCollSize);
        int scanKeys();
        bool pollEvent();
        char getKey();

    };

} // SEFL

#endif //SEFLQUESTROOM2_PEXTMEXTKEYBOARD_H
