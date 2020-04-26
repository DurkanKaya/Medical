//DATE: April-2020
//Author:Durkan KAYA
#include "nrf.h"
/*
   SPI Pinleri
*/
#define MOSI_PIN    3
#define MISO_PIN    2
#define SCK_PIN     6
#define CS_PIN      7
/*
   buffers
*/
#define NUM_WORDS_BUFFER    1 // arabellekteki 'kelime' sayisi
#define NUM_BYTES_WORD   5 //kelime basin bayt sayisi

typedef struct SPIArrayList {
  uint8_t buf[NUM_BYTES_WORD];
} SPIArrayList_type;

SPIArrayList_type SPI_RX[NUM_WORDS_BUFFER];
SPIArrayList_type SPI_TX[NUM_WORDS_BUFFER];

void blockingSpiTransfer() {
  NRF_GPIO->OUTCLR = (1 << CS_PIN); // cihaza dinlemeye baslamasini söyler (aktif düsük)

  NRF_SPIM0->EVENTS_STARTED = 0; //temizle
  NRF_SPIM0->EVENTS_END = 0;
  NRF_SPIM0->TASKS_START = 1; // Basla 

  // now SPI hardware is pulling from the TX and pushing into the RX buffers

  while (NRF_SPIM0->EVENTS_STARTED == 0) {} // 4 baslangiç ??bekle
  while (NRF_SPIM0->EVENTS_END == 0) {} // isi yapmak için 4 spi modülünü bekleyin 

  NRF_GPIO->OUTSET = (1 << CS_PIN); 
}

int deger3() {
 
	blockingSpiTransfer();
  NRF_GPIO->DIRSET = (1 << CS_PIN);
  NRF_GPIO->DIRSET = (1 << SCK_PIN);
  NRF_GPIO->DIRSET = (1 << MOSI_PIN);
  NRF_GPIO->DIRSET = (0 << MISO_PIN);
  NRF_GPIO->PIN_CNF[MISO_PIN] = (GPIO_PIN_CNF_DIR_Input << GPIO_PIN_CNF_DIR_Pos) |
                                (GPIO_PIN_CNF_PULL_Pullup << GPIO_PIN_CNF_PULL_Pos);
  /*
     Enabling SPI, setting pins
  */
  NRF_SPIM0->ENABLE = (SPIM_ENABLE_ENABLE_Enabled << SPIM_ENABLE_ENABLE_Pos);
  NRF_SPIM0->PSEL.SCK = SCK_PIN;
  NRF_SPIM0->PSEL.MOSI = MOSI_PIN;
  NRF_SPIM0->PSEL.MISO = MISO_PIN;
  /*
     Frequency - device & electronics quality dependent. 
	I generally dial this up until it breaks, and then come down by 2/3
  */
  NRF_SPIM0->FREQUENCY = SPI_FREQUENCY_FREQUENCY_M1; // 1MBps
  // SPI_FREQUENCY_FREQUENCY_K250 // 250kB/s
  // SPI_FREQUENCY_FREQUENCY_K500 // 500kB/s
  // SPI_FREQUENCY_FREQUENCY_M1 // 1MB/s
  // SPI_FREQUENCY_FREQUENCY_M2 // 2MB/s
  /*s
     Yapilandirma - cihaza bagli, veri sayfasini dikkatlice okumak en iyisi
  */
  NRF_SPIM0->CONFIG = (SPIM_CONFIG_CPOL_Msk << SPIM_CONFIG_CPOL_ActiveHigh) | // or ActiveLow
                      (SPIM_CONFIG_CPHA_Msk << SPIM_CONFIG_CPHA_Leading) |    // or CPHA_Trailing
                      (SPIM_CONFIG_ORDER_Msk << SPIM_CONFIG_ORDER_LsbFirst);  // or MsbFirst
  /*
     DMA to Send Buffers
     MAXCNT, SPI modülünün gönderecegi bayt sayisidir
     PTR bu baytlara bir adres. yukaridaki küresel olarak tanimlanmis tamponlara bakin
  */
  NRF_SPIM0->TXD.MAXCNT = NUM_BYTES_WORD * NUM_WORDS_BUFFER;
  NRF_SPIM0->TXD.PTR = (uint32_t)&SPI_TX;
  NRF_SPIM0->RXD.MAXCNT = NUM_BYTES_WORD * NUM_WORDS_BUFFER;
  NRF_SPIM0->RXD.PTR = (uint32_t)&SPI_RX;

  NRF_GPIO->OUTSET = (1 << CS_PIN); // satir aktif degil (yüksek)
	
	while(1) {
		__WFE;
			/*
			*tamponunuzu ayarlayin,
			*/
			SPI_TX[0].buf[0] = 0x00;
			SPI_TX[0].buf[1] = 0x01;
			SPI_TX[0].buf[2] = 0x02;
			SPI_TX[0].buf[3] = 0x03;
			SPI_TX[0].buf[4] = 0x04;
   
  }
}
