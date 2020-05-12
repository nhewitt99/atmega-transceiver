typedef enum {
standby1, standby2, standby3,
txOn, txOff, rx, ledOn, ledOff
} machineState;

void f_standby1(void);
void f_standby2(void);
void f_standby3(void);
void f_txOn(void);
void f_txOff(void);
void f_rx(void);
void f_ledOn(void);
void f_ledOff(void);

