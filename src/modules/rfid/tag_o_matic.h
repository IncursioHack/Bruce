/**
 * @file tag_o_matic.h
 * @author Rennan Cockles (https://github.com/rennancockles)
 * @brief Read and Write RFID tags
 * @version 0.2
 * @date 2024-08-19
 */

#include "RFIDInterface.h"


class TagOMatic {
public:
    enum RFID_State {
        READ_MODE,
        CLONE_MODE,
        WRITE_MODE,
        WRITE_NDEF_MODE,
        ERASE_MODE,
        LOAD_MODE,
        SAVE_MODE
    };

    /////////////////////////////////////////////////////////////////////////////////////
    // Constructor
    /////////////////////////////////////////////////////////////////////////////////////
    TagOMatic();
    TagOMatic(RFID_State initial_state);
    ~TagOMatic();

    /////////////////////////////////////////////////////////////////////////////////////
    // Life Cycle and Setup
    /////////////////////////////////////////////////////////////////////////////////////
    void setup();
    void loop();
    void set_rfid_module();

private:
    RFIDInterface *_rfid;
    RFID_State _initial_state;
    bool _read_uid = false;
    bool _ndef_created = false;
    RFID_State current_state;

    /////////////////////////////////////////////////////////////////////////////////////
    // Display functions
    /////////////////////////////////////////////////////////////////////////////////////
    void display_banner();
    void dump_card_details();
    void dump_ndef_details();

    /////////////////////////////////////////////////////////////////////////////////////
    // State management
    /////////////////////////////////////////////////////////////////////////////////////
    void select_state();
    void set_state(RFID_State state);

    /////////////////////////////////////////////////////////////////////////////////////
    // Operations
    /////////////////////////////////////////////////////////////////////////////////////
    void read_card();
    void clone_card();
    void erase_card();
    void write_data();
    void write_ndef_data();
    void save_file();
    void load_file();

    /////////////////////////////////////////////////////////////////////////////////////
    // NDEF
    /////////////////////////////////////////////////////////////////////////////////////
    void create_ndef_message();
    void create_ndef_url();
    void create_ndef_text();
};
