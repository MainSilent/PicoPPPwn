#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "lib/exploit.h"

#define NET_SCK  2
#define NET_MOSI 3
#define NET_MISO 4
#define NET_CS   5
#define NET_RST  6

#define SPI_PORT spi0


static inline void cs_select() {
    asm volatile("nop \n nop \n nop");
    gpio_put(NET_CS, 0);
    asm volatile("nop \n nop \n nop");
}

static inline void cs_deselect() {
    asm volatile("nop \n nop \n nop");
    gpio_put(NET_CS, 1);
    asm volatile("nop \n nop \n nop");
}

static void wiznet_reset() {
    gpio_set_dir(NET_RST, GPIO_OUT);
    gpio_put(NET_RST, 0);
    sleep_ms(100);
    gpio_put(NET_RST, 1);
    sleep_ms(100);
}

uint8_t wiznet_read(void) {
    uint8_t rx = 0, tx = 0xFF;
    spi_read_blocking(SPI_PORT, tx,  &rx, 1);
    return rx;
}

static void wiznet_write(uint8_t reg) {
    spi_write_blocking(SPI_PORT, &reg, 1);
}

int main() {
    int8_t phy_link = 0;
    
    stdio_init_all();

    printf("PicoPPPwn started.\n");

    // Will use SPI0 at 10MHz.
    spi_init(SPI_PORT, 10 * 1000 * 1000);
    gpio_set_function(NET_MISO, GPIO_FUNC_SPI);
    gpio_set_function(NET_SCK, GPIO_FUNC_SPI);
    gpio_set_function(NET_MOSI, GPIO_FUNC_SPI);

    // Chip select is active-low, so we'll initialise it to a driven-high state
    gpio_init(NET_CS);
    gpio_set_dir(NET_CS, GPIO_OUT);
    gpio_put(NET_CS, 1);

    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    wiznet_reset();

    reg_wizchip_spi_cbfunc(wiznet_read, wiznet_write);
    reg_wizchip_cs_cbfunc(cs_select, cs_deselect);

    // Set BufferSize
    setSn_TXBUF_SIZE(SN, 16);
    setSn_RXBUF_SIZE(SN, 16);
    if (getSn_TXBUF_SIZE(SN) != 16 || getSn_RXBUF_SIZE(SN) != 16)
        printf("Error: Failed to set buffer size\n");

    printf("Waiting for PHY LINK.\n");
 
    do { //check phy status
        if (ctlwizchip(CW_GET_PHYLINK, &phy_link) == -1)
            sleep_ms(10);
    } while(phy_link == PHY_LINK_OFF);

    printf("PHY LINK established.\n");

    return run();
}
