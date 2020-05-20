#ifndef NRF_H
#define NRF_H

#define F_CPU 1000000UL // 1 MHz
#include <stdbool.h>
#include <avr/io.h>
#include <util/delay.h>

// simple enum for tx/rx readability
typedef enum {tx = 0, rx = 1} nrf_state_t;

/* List of defines for interfacing nrf24l01 */
// commands
#define NRF_MASK_R_REGISTER 0b00000000
#define NRF_MASK_W_REGISTER 0b00100000
#define NRF_R_RX_PAYLOAD 0b01100001
#define NRF_W_TX_PAYLOAD 0b10100000
#define NRF_FLUSH_TX 0b11100001
#define NRF_FLUSH_RX 0b11100010
#define NRF_REUSE_TX_PL 0b11100011
#define NRF_R_RX_PL_WID 0b01100000
#define NRF_W_TX_PAYLOAD_NOACK 0b10110000
#define NRF_NOP 0b11111111

// config register
#define NRF_CONFIG 0
#define NRF_CONFIG_MASK_RX_DR 6
#define NRF_CONFIG_MASK_TX_DS 5
#define NRF_CONFIG_MASK_MAX_RT 4
#define NRF_CONFIG_EN_CRC 3
#define NRF_CONFIG_CRCO 2
#define NRF_CONFIG_PWR_UP 1
#define NRF_CONFIG_PRIM_RX 0

// enhanced shockburst register
#define NRF_EN_AA 1

// enabled rx addresses
#define NRF_EN_RXADDR 2

// setup address widths
#define NRF_SETUP_AW 3

// setup auto retx
#define NRF_SETUP_RETR 4

// change frequency channel for + model
#define NRF_RF_CH 5

// setup rf
#define NRF_RF_SETUP 6
#define NRF_RF_SETUP_CONT_WAVE 7
#define NRF_RF_SETUP_RF_DR_LOW 5
#define NRF_RF_SETUP_RF_DR_HIGH 3
#define NRF_RF_SETUP_RF_PWR 1

// status
#define NRF_STATUS 7
#define NRF_STATUS_RX_DR 6
#define NRF_STATUS_TX_DS 5
#define NRF_STATUS_MAX_RT 4
#define NRF_STATUS_RX_P_NO 1
#define NRF_TX_FULL 0

// rx addresses
#define NRF_RX_ADDR_P0 10
#define NRF_RX_ADDR_P1 11
#define NRF_RX_ADDR_P2 12
#define NRF_RX_ADDR_P3 13
#define NRF_RX_ADDR_P4 14
#define NRF_RX_ADDR_P5 15

// tx address
#define NRF_TX_ADDR 16

// rx payload sizes
#define NRF_RX_PW_P0 17
#define NRF_RX_PW_P1 18
#define NRF_RX_PW_P2 19
#define NRF_RX_PW_P3 20
#define NRF_RX_PW_P4 21
#define NRF_RX_PW_P5 22

// fifo status
#define NRF_FIFO_STATUS 23
#define NRF_FIFO_STATUS_TX_REUSE 6
#define NRF_FIFO_STATUS_TX_FULL 5
#define NRF_FIFO_STATUS_TX_EMPTY 4
#define NRF_FIFO_STATUS_RX_FULL 1
#define NRF_FIFO_STATUS_RX_EMPTY 0

// dynamic payload
#define NRF_DYNPD 28

// feature register
#define NRF_FEATURE 29
#define NRF_FEATURE_EN_DPL 2
#define NRF_FEATURE_EN_ACK_PAY 1
#define NRF_FEATURE_EN_DYN_ACK 0

/* Function prototypes, comments in nrf.c */
void SPI_MasterInit(void);
uint8_t SPI_MasterTransmit(uint8_t cData);
uint8_t nrfNop(void);
uint8_t nrfWriteReg(uint8_t reg, uint8_t data);
uint8_t nrfReadReg(uint8_t reg);
uint8_t nrfTransmit(uint8_t* data, uint8_t len);
uint8_t nrfReceive(uint8_t* data, uint8_t len);
bool nrfPowerUp(void);
bool nrfSetupRF(void);
bool nrfSetChannel(uint8_t channel);
void nrfSetup(void);
void nrfSetMode(nrf_state_t state);
void blinkBinary(uint8_t data);

#endif

