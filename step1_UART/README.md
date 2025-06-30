# Step 1: UART Output (Baremetal)

This is a minimal bare-metal program that outputs `Hello world` via UART on Raspberry Pi 3 Model B (AArch32).  
It verifies that UART (PL011) initialization works correctly.

---

## How to View UART Output

Use a serial terminal such as **Tera Term** on your PC to observe the UART output.

### Tera Term Settings

| Setting        | Value     |
|----------------|-----------|
| Baud rate      | 115200    |
| Data bits      | 8         |
| Parity         | None      |
| Stop bits      | 1         |
| Flow control   | None      |

Connect to the **serial port** exposed by your USB-to-TTL converter or UART header on the Pi.

---

## Build Instructions

```bash
cd src
make
```
This will produce kernel.img.
Copy the following files to the SD card boot partition:

- bootcode.bin ← required (official firmware)
- start.elf ← required (official firmware)
- config.txt ← provided in this repo
- kernel.img ← built via make

Then insert the SD card into the Pi and power it on.

## Expected Behavior
The following message should repeatedly appear over UART:

    Hello world  
    Hello world  
    Hello world 
    ... 

## Notes
- This example runs in AArch32 (32-bit) mode.  
- GPIO14/15 are configured for ALT0 function (TXD0/RXD0).  
- config.txt should contain at minimum:

    ```txt
    arm_64bit=0
    kernel=kernel.img
    enable_uart=1
    device_tree=
    ```
