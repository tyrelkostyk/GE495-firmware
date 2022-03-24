// defs.h
// Contains type definitions and function stubs.

#ifndef DEFS_H
#define DEFS_H

#include <stdint.h>
#include <string.h>

#define OK  (0x01)
#define ERR (0x00)
#define NOP (0xff)

typedef struct _message_t message_t;
typedef struct _command_t command_t;
typedef struct _update_t  update_t;

/******
* CAN *
******/

#define CAN_BAUDRATE (9600)
#define CAN_UP_RX (4)
#define CAN_UP_TX (5)

// Downstream CAN for Arduino now uses AltSoftSerial which requires RX 8, TX 9
// #define CAN_DOWN_TX 6
// #define CAN_DOWN_RX 7

#define CAN_FRAME_STD           (0x00)
#define CAN_FRAME_EXT           (0x01)

#define CAN_NO_REMOTE           (0x00)
#define CAN_REMOTE              (0x01)

#define CAN_DATA_LEN_MAX        (8)
#define CAN_DATA_LEN_CALIBRATE  (5)

// Enum for determining which CAN unit is desired (upstream or downstream)
typedef enum {
    Up,
    Down
} can_dir_t;

typedef can_dir_t dir_t;

typedef struct __attribute__ ((__packed__)) _message_t {
    uint32_t id;
    uint8_t length;
    uint8_t data[CAN_DATA_LEN_MAX];
} message_t;

uint8_t canInit (void);
uint8_t canSend(can_dir_t direction, message_t message);
uint8_t canReceive(can_dir_t direction, message_t *message);

inline float unpack754(unsigned long long i)
{
    unsigned bits = 32;
    unsigned expbits = 8;
    float result;
    long long shift;
    unsigned bias;
    unsigned significandbits = bits - expbits - 1; // -1 for sign bit

    if (i == 0) return 0.0;

    // pull the significand
    result = (i&((1LL<<significandbits)-1)); // mask
    result /= (1LL<<significandbits); // convert back to float
    result += 1.0f; // add the one back on

    // deal with the exponent
    bias = (1<<(expbits-1)) - 1;
    shift = ((i>>significandbits)&((1LL<<expbits)-1)) - bias;
    while(shift > 0) { result *= 2.0; shift--; }
    while(shift < 0) { result /= 2.0; shift++; }

    // sign it
    result *= (i>>(bits-1))&1? -1.0: 1.0;

    return result;
}

inline float unpackFloatFromData(uint8_t *data)
{
    uint32_t num = 0;
    for (int i = 0; i < 4; i++) {
        num |= data[i] << (8*i);
    }
    return unpack754((unsigned long long)num);
}

inline void packDataWithFloat(uint8_t *data, float f)
{
    union {
        float fl;
        uint32_t ui;
    } un = { .fl = f };
    uint32_t num = un.ui;
    for (int i = 0; i < 4; i++) {
        data[i] = (num >> (8*i)) & 0xff;
    }
    for (int i = 4; i < CAN_DATA_LEN_MAX; i++) {
        // data[i] = 0;
        data[i] = 0xa + (i-4);
    }
}

inline void packDataWithZero(uint8_t *data)
{
    memset(data, 0, CAN_DATA_LEN_MAX);
}

/*********
* SERIAL *
*********/

#define SER_BAUDRATE 19200

#define UART_BAUDRATE 19200

struct Message;
struct Command;
struct Update;

void uartInit(void);
uint8_t uartSend(dir_t dir, Message *message);
bool uartReceive(dir_t dir);
void uartGetMessage(dir_t dir, Message *message);

/****************
* COMMAND (CMD) *
****************/

uint8_t cmdSendUpstream (void);
uint8_t cmdReceiveDownstream (void);
uint8_t cmdParse (void);

typedef enum {
    Tare,
    Calibrate,
    Reset,
    Invalid
} cmd_type_t;

typedef struct _command_t {
    uint8_t target;
    cmd_type_t type;
    uint16_t arg;
    union {
        float mass;
        uint32_t val;
    } data;
} command_t;

#define PGN_POSITION    (8)
#define PGN_SIZE        (0x3ffff)
/*
 * PGN Format:
 * [28 ... 9   8   7   6   5   4   3   2   1   0  ]
 * [X... TARE|CAL|   CAL STEP    | X |  TANK  ID  ]
 */
#define PGN_CMD_TARGET_IDX  (0)
#define PGN_CMD_TYPE_IDX    (8)
#define PGN_CMD_ARG_IDX     (4)
#define PGN_CMD_TARGET      (0x7)
#define PGN_CMD_TYPE        (0x3)
#define PGN_CMD_ARG         (0xf)

#define PGN_UPDATE_TANK_IDX (0)
#define PGN_UPDATE_TANK     (0x7)

#define PGN_DEBUG_HANDSHAKE 0x01

#define PGN_TARE (0x20)

#define PGN_CALIBRATE   (0x10)
#define PGN_CAL_CONF_M1 (1<<0)
#define PGN_CAL_CONF_M2 (1<<1)
#define PGN_CAL_FINISH  (1<<2)

/*********
* UPDATE *
*********/

// TODO Deprecated?
#define UPDATE_MASS_LEN_MAX (8)
#define UPDATE_DELAY_MS (1000)

typedef struct _update_t {
    uint8_t tank;
    float mass;
} update_t;

uint8_t updateReceiveUpstream(void);
uint8_t updateSendDownstream(void);
uint8_t updateHandle(void);
void updateLoadCurrentData(double data);

/*******
* MASS *
*******/

#define SAMPLE_SIZE  (5)

#define MASS_NUM_BYTES (4)

void massGetCurrent(uint8_t *buffer);

extern double mass1;
extern double mass2;
extern int32_t voltage1;
extern int32_t voltage2;
extern double voltageToMassFactor; 

/******
* I2C *
******/

void applySCLK();
void setSCLKHigh();
void setSCLKLow();

/******
* ADC *
******/

extern int32_t dataOffset0;
extern int32_t dataOffset1;
extern int32_t dataOffset2;

int32_t retrieveADCData();
int32_t retrieveADCDataWithCal();
void setADCMux(uint8_t muxSelect);
void doADCPowerUpSequence();
void setADCSpeed(uint8_t sampleSpeed);
void blockingWaitForData();
int32_t readThreeLoadCells();
int32_t getNMeasurements(int32_t N);
int32_t getNRawMeasurements(uint8_t mux, int32_t N);

#define NUM_ADC_BITS   (24)
#define NUM_LOAD_CELLS (3)

/**************
* CALIBRATION *
**************/

void tare(uint8_t mux, int32_t offset);
void tareAllLoadCells();
void getCalMass1(double mass);
void getCalMass2(double mass);
void getVoltageToMassFactor(double mass1, int32_t voltage1, double mass2, int32_t voltage2);

/**********
* ARDUINO *
**********/

#define DATA_PIN  (12)
#define CLOCK_PIN (11)
#define POWER_PIN (10)

#define MUX_PIN0 (2)
#define MUX_PIN1 (3)

#define SPEED_PIN (7)

#endif  // DEFS_H
