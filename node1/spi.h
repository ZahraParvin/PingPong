/*
 * spi.h
 *
 * Created: 27.10.2023 16:17:26
 *  Author: jovm
 */ 


#ifndef SPI_H_
#define SPI_H_

#define DDR_SPI DDRB
#define DD_SS PB4
#define DD_MOSI PB5
#define DD_MISO PB6
#define DD_SCK PB7

// Setter SS høy. Lar alt annet på PORTB være som det var.
void spi_set_ss();
// Setter SS lav. Lar alt annet på PORTB være som det var.
void spi_clear_ss();


void spi_master_init();

void spi_write(char cData);


uint8_t spi_read();




#endif /* SPI_H_ */
