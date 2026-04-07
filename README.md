# 🐱 SaniSolution 120 Cartridge Emulator (CatGenie 120)

## 📌 Overview
This project implements an emulator for the RFID-based SaniSolution cartridge used in the CatGenie 120 system.

The goal is to allow reuse of refilled cartridges by emulating the original RFID tag behavior, which normally enforces a limited number of cleaning cycles.

The emulator is built using a RP2040-based Tiny2040 board, acting as a replacement for the original RFID tag.

---

## ⚠️ Disclaimer
This project is for educational and research purposes only.  
Use at your own risk. Modifying or emulating hardware may void warranties or violate local regulations.

---

## 🧠 Background

The CatGenie system uses RFID technology to track cartridge usage:

- Each SaniSolution cartridge contains an RFID tag  
- The tag stores two countdown counters  
- Each cleaning cycle decrements the counters  
- When counters reach zero → cartridge is considered empty  
- The counters are decrement-only and cannot be reset  

---

## 📡 Hardware Information

- RFID Reader Chip: CR14 RFID reader  
- RFID Tag: SRIX4K-A4S_1GE  

---

## 🎯 Project Goal

To emulate:
1. The RFID tag behavior  
2. The communication protocol between the CatGenie and the tag  

So that:
- Refilled cartridges can be reused  
- The system continues to operate normally  

---

## 🧩 Hardware Setup

This project uses a Tiny2040 (RP2040) board.

### 🔌 Connections

| Tiny2040 Pin | Function    |
|-------------|------------|
| GPIO 0      | Data       |
| GPIO 1      | Clock      |
| 5V          | Power      |
| GND         | Ground     |

---

## ⚙️ How It Works

- The Tiny2040 emulates an RFID tag  
- It communicates with the CatGenie reader using the expected protocol  
- It maintains internal state to simulate:
  - Remaining cycles  
  - Counter behavior  

### ✍️ Write Operations

The system requires:
- 4 write operations in total  
to correctly emulate cartridge behavior  

---

## 🚀 Features

- RFID communication emulation  
- Counter simulation  
- Compatible with CatGenie 120  
- Simple hardware setup  

---


## 🛠️ Build & Flash

### Requirements
- Pico SDK  
- ARM toolchain (arm-none-eabi-gcc)  
- CMake + Ninja  

### Build
- mkdir build
- cd build
- cmake ..
- make

### Flash
- Hold BOOT button on Tiny2040  
- Connect via USB  
- Copy the .uf2 file to the device  


---


## 📜 License

MIT License

---

## 👨‍💻 Author

Ivgeny Tokarzhevsky  
Embedded Software Engineer | IoT Developer  
