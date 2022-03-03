#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "hardware/i2c.h"
#include "hardware/dma.h"
#include "hardware/pio.h"
#include "hardware/timer.h"
#include "hardware/watchdog.h"
#include "hardware/sync.h"
#include "hardware/irq.h"
#include "hardware/clocks.h"
#include "hardware_config.h"
#include "parallelio.pio.h"

// SPI Defines
// We are going to use SPI 0, and allocate it to the following GPIO pins
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments
#define SPI_PORT spi0
#define PIN_MISO 16
#define PIN_CS   17
#define PIN_SCK  18
#define PIN_MOSI 19

// I2C defines
// This example will use I2C0 on GPIO8 (SDA) and GPIO9 (SCL) running at 400KHz.
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments
#define I2C_PORT i2c0
#define I2C_SDA 8
#define I2C_SCL 9

int64_t alarm_callback(alarm_id_t id, void *user_data) {
    // Put your timeout handler code in here
    return 0;
}

void on_parallelio()
{
  if (pio0_hw->irq & 1)
  {
    pio0_hw->irq = 1;
    // PIO0 IRQ0 fired
  }
  else if (pio0_hw->irq & 2)
  {
    pio0_hw->irq = 2;

    // PIO0 IRQ1 fired
  }

  uint32_t interrupts = save_and_disable_interrupts();

  PIO pio = pio0;
  uint32_t c = parallelio_program_getc(pio, 0);
  //printf("%04x\n", c);

  uint8_t addr = c & 0xFF;
  uint8_t data = (c >> 8) & 0xFF;

  printf("%02x %02x\n", addr, data);

  restore_interrupts(interrupts);
}


int rxtask(){
    PIO pio = pio0;
  uint offset = pio_add_program(pio, &parallelio_program);
  uint sm = pio_claim_unused_sm(pio, true);
  parallelio_program_init(pio, sm, offset, 0);

  //pio_set_irq0_source_enabled(pio, pis_interrupt0, true);

  irq_add_shared_handler(PIO0_IRQ_0, on_parallelio, PICO_SHARED_IRQ_HANDLER_DEFAULT_ORDER_PRIORITY);

  irq_set_enabled(PIO0_IRQ_0, true);

  pio0_hw->inte0 = PIO_IRQ0_INTE_SM0_BITS | PIO_IRQ0_INTE_SM1_BITS;

}

int main()
{
    stdio_init_all();
    rxtask();

    // SPI initialisation. This example will use SPI at 1MHz.
    /*
    spi_init(SPI_PORT, 1000*1000);
    gpio_set_function(PIN_MISO, GPIO_FUNC_SPI);
    gpio_set_function(PIN_CS,   GPIO_FUNC_SIO);
    gpio_set_function(PIN_SCK,  GPIO_FUNC_SPI);
    gpio_set_function(PIN_MOSI, GPIO_FUNC_SPI);

    // Chip select is active-low, so we'll initialise it to a driven-high state
    gpio_set_dir(PIN_CS, GPIO_OUT);
    gpio_put(PIN_CS, 1);

    */

    // I2C Initialisation. Using it at 400Khz.
    /*
    i2c_init(I2C_PORT, 400*1000);
    
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);
    */

    // Timer example code - This example fires off the callback after 2000ms
    // add_alarm_in_ms(2000, alarm_callback, NULL, false);

    // Set all 3 8-bit level shifters to push-pull mode (set to 0 for tristate)
    for(int pin=PIN_OE0; pin <= PIN_OE3; pin++) {
        gpio_init(pin);
        gpio_set_dir(pin, GPIO_OUT);
        gpio_put(pin, 1);
    }

    while(1){
        printf("%d\n", gpio_get(PIN_GPIO_MIN));
        sleep_ms(100);
    }
}
