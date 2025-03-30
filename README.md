# PicoPPPwn

Pico PPPwn with w5500 module without the need of a flash drive since goldhen is in internal flash memory and sent via ethernet connection.

### Upload

Connect Pi Pico while holding boot button, Pico will appear as an external drive, drag and drop the uf2 file provided in [Release Page](https://github.com/MainSilent/PicoPPPwn/releases/)

### Demo

[Youtube Video](https://www.youtube.com/watch?v=7L4v-r5ofKI)

### Build Instruction
Make sure the Pico SDK is setup properly
```sh
git clone https://github.com/MainSilent/PicoPPPwn.git
cd PicoPPPwn
# Move your stage1.bin and stage2.bin here
py gen_payload.py stage1.bin stage1.h # Rename the variable to 'stage1_bin'
py gen_payload.py stage2.bin stage2.h # Rename the variable to 'stage2_bin'
mkdir build
cd build
cmake ..
make
```

### Pins

| Pico | W5500 |
| ------------- | ------------- |
| GND  | GND  |
| 3.3v (out)  | 3.3v  |
| GP2  | SCLK  |
| GP3  | MOSI  |
| GP4  | MISO  |
| GP5  | SCS  |

### Warning

I have not tested the release file for firmware 11.00, Please let me know if it's working properly.
