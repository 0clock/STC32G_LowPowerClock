#ifndef SHT41_H
#define SHT41_H

//#include <stdint.h>

#define SHT41_I2C_ADDRESS 0x44

// SHT41 命令
#define SHT41_MEASURE_HIGHREP   0xFD
#define SHT41_MEASURE_MEDREP    0xF6
#define SHT41_MEASURE_LOWREP    0xE0

typedef enum {
    SHT41_IDLE,
    SHT41_MEASURING,
    SHT41_DATA_READY
} sht41_state_t;

typedef struct {
    sht41_state_t state;
    float temperature;
    float humidity;
} sht41_data_t;

void sht41_init(void);
void sht41_start_measurement(void);
int sht41_read_data(sht41_data_t *data_buff);

#endif // SHT41_H


#if 0

void SHT41_Init();

void SHT41_R_High();

void SHT41_falg_0004();


#endif