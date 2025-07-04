# Step 2: ARM Timer Interrupt (WIP)

This step implements periodic interrupt handling using the **ARM Timer** on Raspberry Pi 3 Model B (AArch32).  
⚠️ **Note:** This implementation is currently under debugging and may not behave as expected.

---

## Intended Behavior

- The ARM Timer is configured to fire **every 0.5 seconds** (500,000 ticks at 1MHz).
- When the timer interrupt fires:
  - The `irq_handler()` is invoked.
  - The handler sends a message via UART:  
    ```
    Interrupt
    ```
  - GPIO17 is toggled (High ↔ Low), allowing LED blinking if connected.

---

## Known Issue (Under Debugging)

- The timer initialization seems to complete (confirmed via UART).
- However, the `irq_handler()` is **not being called**.
- Personal hypothesis:  
  The issue may be related to **incorrect IRQ stack setup** in `start.S`.

---

## How to View UART Output
> ⚠️ **Note:** As of now, no UART output is displayed after boot.  

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

---
