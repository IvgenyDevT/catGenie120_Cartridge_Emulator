#include "config.h"
#include "types.h"




/*prototypes*/
void i2c_interrupt_handler();
void flash_write_counter(uint8_t value);
uint8_t flash_read_counter();
void set_error_indicator();





/*variables*/
static i2c_state_t state;
static bool first_comm = true;
static uint8_t counter;




/* SRIX4K-A4S_1GE memory emulator */
uint8_t BLOCK_0[] = { 0x04, 0x00, 0x00, 0x00, 0x00 };
uint8_t BLOCK_1[] = { 0x04, 0x78, 0x00, 0x00, 0x00 };
uint8_t BLOCK_2[] = { 0x04, 0x01, 0x00, 0x78, 0x00 };
uint8_t BLOCK_3[] = { 0x04, 0x04, 0x0F, 0x00, 0x00 };
uint8_t BLOCK_4[] = { 0x04, 0x1F, 0x7E, 0x30, 0x2A };
uint8_t BLOCK_5[] = { 0x04, 0x04, 0x0F, 0x2B, 0xFC };

uint8_t* BLOCKS[] = {BLOCK_0, BLOCK_0, BLOCK_0, BLOCK_0,
                              BLOCK_0, BLOCK_1, BLOCK_1, BLOCK_0,
                              BLOCK_2, BLOCK_2, BLOCK_2, BLOCK_3,
                              BLOCK_3, BLOCK_4, BLOCK_0, BLOCK_5};


uint8_t NODE_ID_RESPONSE[] = { 0x01, 0x3C };

uint8_t UID_RESPONSE[] = { 0x08, 0xA3, 0x6E, 0x6A, 0x73, 0x09, 0x33, 0x02, 0xD0 };





int main() {
    /* - - - - - - - - - - - - Boot loader - - - - - - - - - - - - */


    /*init LEDs GPIOss and set GPIO direction*/
     gpio_init(RED_PIN);
     gpio_set_dir(RED_PIN, GPIO_OUT);

     gpio_init(GREEN_PIN);
     gpio_set_dir(GREEN_PIN, GPIO_IN);

     gpio_init(BLUE_PIN);
     gpio_set_dir(BLUE_PIN, GPIO_IN);



     //enable i2c hardware in block 0
     i2c_init(i2c0,(uint)100000);

     //mux gpio to I2C
     gpio_set_function(I2C0_CLK, GPIO_FUNC_I2C);
     gpio_set_function(I2C0_SDA, GPIO_FUNC_I2C);

     //set I2C lines pull-ups resistors
     gpio_pull_up(I2C0_SDA);
     gpio_pull_up(I2C0_CLK);

     //set I2C0 block as slave - address 0x50 (80 in base 10)
     i2c_set_slave_mode(i2c0, true, I2C_ADDR);


     //set required interrupts
     i2c0->hw->intr_mask =
     I2C_IC_INTR_MASK_M_RD_REQ_BITS |
     I2C_IC_INTR_MASK_M_RX_FULL_BITS |
     I2C_IC_INTR_MASK_M_STOP_DET_BITS;

     //clear interrupts before use - maybe not neccessary
     i2c0->hw->clr_rd_req;
     i2c0->hw->clr_stop_det;

     //register interrupt handler for I2c0
     irq_set_exclusive_handler(I2C0_IRQ, i2c_interrupt_handler);

     //enable i2c block 0 interrupts
     irq_set_enabled(I2C0_IRQ, true);

     //read the last saved counter from flash memory
     counter = flash_read_counter();


     //0xFF -> counter not initialized, 0x00 -> counter reached 0, set counter at 120.
     if (counter == 0xFF || counter == 0) {
         counter = 120;
         //Update the counter in flash memory
         flash_write_counter(counter);
     }

     //set the counter value in BLOCK_1 in RAM
     BLOCK_1[1] = counter;




     /* - - - - - - - -  - - - - - - - - main loop - - - - - - - - - - - -*/
      //Runs until getting interrupt
     while(true) {

         sleep_ms(100);

         /*check every 100ms if i2c master updated counter in RAM,
          *if it does, update the new value in flash memory.
          **/
         if (BLOCK_1[1] != counter) {
             //update counter value
             counter = BLOCK_1[1];
             //Update counter in flash memory
             flash_write_counter(counter);

         }
     }
 }




void i2c_interrupt_handler() {

    //read status register
    uint32_t reg_status = i2c0->hw->intr_stat;


    /* - - - - - - - - Read data from master - - - - - - - -*/
    if ( reg_status & I2C_IC_RAW_INTR_STAT_RX_FULL_BITS) {//byte received from master

        //get bytes from received data buffer
        while (i2c_get_read_available(i2c0)) {
            if (state.readIndex < sizeof(state.readBuffer)) {
                state.readBuffer[state.readIndex++] = i2c_read_byte_raw(i2c0);
            }
            //read buffer full, skip entire bytes (in our case, unnecessary command)
            else {
                i2c_read_byte_raw(i2c0);
            }
        }
    }

    /* - - - - - - - - parse master data - - - - - - - -*/
    if ( reg_status & I2C_IC_RAW_INTR_STAT_STOP_DET_BITS) {//master finished sending bytes


        if (state.readIndex >= 3) {// at least 3 bytes are necessary for parsing a command

            //the command bits is the 4 lsb bits in byte 3
            uint8_t command = state.readBuffer[CATGENIE_COMMAND_BYTE] & MASK_4_BIT_LSB;



            switch (command) {
                case 0x06: // INITIATE, PCALL16, SLOT_MARKER
                    state.response = NODE_ID_RESPONSE;//set ID in response buffer
                state.respone_size = 2;//update response size
                break;

                case 0x08: // READ_BLOCK
                    if (state.readIndex >= 4 && state.readBuffer[CATGENIE_BLOCK_NUB_BYTE] < 16 ) {

                        //set the block context that master wants to read into the response buffer
                        state.response = BLOCKS[state.readBuffer[CATGENIE_BLOCK_NUB_BYTE]];
                        //update response buffer size
                        state.respone_size = 5;
                    }

                    //unhandled, set error led indicator
                    else {
                    set_error_indicator();
                    }
                break;

                case 0x09: // WRITE_BLOCK
                    if (state.readIndex>= 8  && state.readBuffer[CATGENIE_BLOCK_NUB_BYTE] < 16 ) {
                        //write the data from master to required block
                        uint8_t* block = BLOCKS[state.readBuffer[3]];
                        block[1] = state.readBuffer[4];
                        block[2] = state.readBuffer[5];
                        block[3] = state.readBuffer[6];
                        block[4] = state.readBuffer[7];
                        state.respone_size = 0;
                    }
                    //unhandled, set error led indicator
                    else {
                        set_error_indicator();
                    }

                break;

                case 0x0A: // AUTHENTICATE (ignored)
                    state.respone_size = 0;
                break;

                case 0x0B: // GET_UID
                    state.response = UID_RESPONSE;
                state.respone_size = 9;
                break;

                case 0x0C: // RESET_TO_INVENTORY
                    state.respone_size = 0;
                break;

                case 0x0E: // SELECT
                    state.response = NODE_ID_RESPONSE;
                state.respone_size = 2;
                break;

                case 0x0F: // COMPLETION
                    state.respone_size = 0;
                break;

                default:
                    //none
                        break;
            }

            //set response bytes pointer at start ( used for sending the response buffer
            state.responseIndex = 0;
        }
        else {

        }


        //reset read index because master finished writing
        state.readIndex = 0;

        //clear the interrupt
        i2c0->hw->clr_stop_det;
    }

    /* - - - - - - - - write response to master - - - - - - - -*/
    if (reg_status & I2C_IC_RAW_INTR_STAT_RD_REQ_BITS) {

        //send one byte from response buffer each read request interrupt
        if (state.responseIndex < state.respone_size) {
            //send a byte from response buffer
            i2c_write_byte_raw(i2c0, state.response[state.responseIndex++]);
            //clear the read request interrupt
            i2c0->hw->clr_rd_req;
        }
        else {
            //if master still asking for bytes when response buffer reached the end, send 0xFF
            i2c_write_byte_raw(i2c0, 0xFF);
            //clear read request interrupt
            i2c0->hw->clr_rd_req;
        }
    }

    /* - - - - - - - - set indicator if communication succeed al least once - - - - - - - -*/
    if (first_comm) {
        first_comm = false;
        gpio_set_dir(RED_PIN, GPIO_IN);
        gpio_set_dir(GREEN_PIN, GPIO_OUT);
    }
}



void set_error_indicator() {

    gpio_set_dir(GREEN_PIN, GPIO_IN);
    gpio_set_dir(RED_PIN, GPIO_OUT);

    //blink until reset
    while (true) {
        gpio_put(RED_PIN, true);
        sleep_ms(1000);
        gpio_put(RED_PIN, false);
        sleep_ms(1000);
    }
}





