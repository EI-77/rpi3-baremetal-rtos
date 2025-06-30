# Raspberry Pi 3 Baremetal RTOS

This repository contains a full-scratch bare-metal RTOS project for **Raspberry Pi 3 Model B v1.2**, written in C and ARM32 assembly (AArch32).

## ✅ Step 1: UART
- Implements UART (PL011) output
- Prints `Hello world` over serial when booted

## 🛠️ Step 2: ARM Timer + IRQ
- Intended to generate periodic interrupts via the ARM Timer
- Currently **under development**

---

## 🔧 Build & Run Instructions

Each step (e.g., `step1_UART`, `step2_IRQ`) contains a `src/` directory.  
Run `make` inside the `src/` directory to build `kernel.img`.

To boot on a real Raspberry Pi 3 Model B:

1. Prepare an SD card with a FAT32 boot partition
2. Copy the following files into the SD card root directory:

- bootcode.bin ← required (official firmware)
- start.elf ← required (official firmware)
- config.txt ← provided in this repo
- kernel.img ← built via make


---

## ⚠️ Note on Firmware Files

- **`bootcode.bin`** and **`start.elf`** are proprietary firmware files provided by the Raspberry Pi Foundation.
- These files are **not included in this repository** due to licensing restrictions.
- You must obtain them yourself from the official firmware repository:

👉 https://github.com/raspberrypi/firmware/tree/master/boot

---
