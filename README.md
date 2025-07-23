# 🧠 HydraESP AI Edition

**ESP32-S3 Ponagotchi-Style AI Companion with Advanced Network Analysis**

A sophisticated ESP32-S3 firmware project that combines AI-driven behavioral analysis with real-time WiFi/BLE scanning, featuring an animated LVGL interface and FreeRTOS multitasking architecture.

---

## 🌟 Features

### Core Capabilities
- **🎭 Ponagotchi-Style AI Personality** - Animated facial expressions based on network activity
- **📡 Advanced Network Scanning** - Real-time WiFi and BLE device detection and analysis  
- **🧠 AI Behavioral Inference** - TinyML-based state machine with 8 distinct behavioral modes
- **🎨 LVGL Animated UI** - Smooth 30fps animations with emotional face expressions
- **⚡ FreeRTOS Multitasking** - Optimized task distribution across dual ESP32-S3 cores
- **💾 PSRAM Optimization** - Efficient memory management for complex operations
- **📊 System Health Monitoring** - Comprehensive diagnostics and resource management
- **🔄 OTA Update Support** - Over-the-air firmware updates with custom partitions

### AI Behavioral States
- **😊 Idle** - Default peaceful state during low activity
- **👃 Sniffing** - High WiFi network activity detected
- **👁️ Tracking** - Strong BLE devices in proximity
- **🧠 Learning** - Processing and analyzing captured data
- **🤩 Excited** - Discovered interesting networks or devices
- **😴 Sleeping** - Energy-saving mode during inactivity
- **💀 Error** - System diagnostics and error handling
- **🔄 Updating** - OTA or system maintenance mode

---

## 🛠️ Hardware Requirements

### Recommended Development Board
- **ESP32-S3-DevKitC-1** with 16MB Flash + 8MB PSRAM
- **ST7789 320x240 TFT Display** (CYD compatible)
- **Status LED** (GPIO 22)
- **SD Card Slot** (optional for data logging)

### Pin Configuration
```cpp
// Display (ST7789)
#define TFT_CS     15
#define TFT_DC     2  
#define TFT_BL     21
#define TFT_MOSI   13
#define TFT_CLK    14

// Status LED
#define STATUS_LED_PIN 22

// SD Card (optional)
#define SD_CS      5
#define SD_MOSI    23
#define SD_MISO    19
#define SD_CLK     18
```

---

## 🚀 Quick Start

### Prerequisites
- **PlatformIO** IDE or CLI
- **ESP32-S3** development board
- **Compatible display** (ST7789 320x240)
- **USB-C cable** for programming

### Installation
```bash
# Clone the repository
git clone https://github.com/your-org/hydraesp-ai-edition.git
cd hydraesp-ai-edition

# Install dependencies
make install

# Build the project
make build

# Flash to ESP32-S3
make flash

# Monitor serial output
make monitor
```

### Alternative PlatformIO Commands
```bash
# Using PlatformIO directly
pio run --environment esp32-s3-devkitc-1
pio run --target upload
pio device monitor --baud 115200
```

---

## 🏗️ Architecture Overview

### System Architecture
```
┌─────────────────────────────────────────┐
│              ESP32-S3 Dual Core         │
├─────────────────────┬───────────────────┤
│     Core 1 (APP)    │    Core 0 (PRO)   │
│                     │                   │
│  ┌─────────────┐   │  ┌─────────────┐  │
│  │  UI Task    │   │  │  AI Task    │  │
│  │ (Priority 2)│   │  │(Priority 3) │  │
│  │             │   │  │             │  │
│  │ • LVGL      │   │  │ • Inference │  │
│  │ • Animation │   │  │ • State Mgmt│  │
│  │ • Display   │   │  │ • Behavior  │  │
│  └─────────────┘   │  └─────────────┘  │
│                     │                   │
│                     │  ┌─────────────┐  │
│                     │  │ Scan Task   │  │
│                     │  │(Priority 1) │  │
│                     │  │             │  │
│                     │  │ • WiFi Scan │  │
│                     │  │ • BLE Scan  │  │
│                     │  │ • Analysis  │  │
│                     │  └─────────────┘  │
│                     │                   │
│                     │  ┌─────────────┐  │
│                     │  │System Task  │  │
│                     │  │(Priority 1) │  │
│                     │  │             │  │
│                     │  │ • Monitoring│  │
│                     │  │ • Diagnostics│ │
│                     │  │ • Health    │  │
│                     │  └─────────────┘  │
└─────────────────────┴───────────────────┘
```

### Task Communication
- **FreeRTOS Queues** for inter-task messaging
- **Mutexes** for thread-safe data access
- **Semaphores** for resource synchronization
- **Event Groups** for system coordination

### Memory Management
- **PSRAM** for large buffers (display, AI models)
- **Internal RAM** for critical real-time operations
- **Heap monitoring** with automatic cleanup
- **Stack overflow protection** for all tasks

---

## 📁 Project Structure

```
hydraesp-ai-edition/
├── platformio.ini          # PlatformIO configuration
├── partitions.csv          # Custom ESP32 partitions
├── Makefile               # Build automation
├── README.md              # This file
├── include/               # Header files
│   ├── config.h          # Hardware and system config
│   ├── ai_states.h       # AI behavior definitions
│   └── system_monitor.h  # System health monitoring
├── src/                  # Source code
│   ├── main.cpp          # Main entry point
│   ├── ai_states.cpp     # AI inference implementation
│   ├── drivers/          # Hardware drivers
│   │   └── display_driver.cpp
│   └── tasks/            # FreeRTOS task implementations
│       ├── ui_task.cpp   # UI and animation
│       ├── ai_task.cpp   # AI behavioral inference
│       ├── scan_task.cpp # Network scanning
│       └── system_task.cpp # System monitoring
└── docs/                 # Documentation
```

---

## 🎮 Usage Guide

### First Boot
1. **Power on** the ESP32-S3 with connected display
2. **Watch the face** animate through different expressions
3. **Monitor serial output** for detailed logging
4. **Observe behavior** changes based on WiFi/BLE activity

### Understanding AI Behaviors
- **😊 Idle State** - No significant network activity
- **👃 Sniffing** - Multiple WiFi networks detected (>10)
- **👁️ Tracking** - Strong BLE signals nearby (>-50dBm) 
- **🧠 Learning** - Processing detected network patterns
- **🤩 Excited** - Found interesting/unusual networks
- **😴 Sleeping** - Extended period of low activity
- **💀 Error** - System issues (low memory, hardware failure)
- **🔄 Updating** - Performing system maintenance

### Serial Monitor Output
```
🧠 HydraESP AI Edition v2.0
ESP32-S3 Ponagotchi-Style AI Companion
==================================================

🔧 Initializing hardware components...
✅ SPI initialized
✅ I2C initialized  
✅ WiFi initialized in station mode
✅ Bluetooth initialized
✅ PSRAM initialized: 8192 KB available

💾 Initializing storage systems...
✅ SPIFFS initialized: 1536 KB total, 64 KB used
✅ SD Card initialized: 32768MB

🚀 Creating FreeRTOS tasks...
✅ UI Task created on Core 1
✅ AI Task created on Core 0
✅ Scan Task created on Core 0
✅ System Task created on Core 0

✅ HydraESP AI Edition initialized successfully!
🚀 All systems operational
```

---

## 🔧 Configuration

### Hardware Configuration (config.h)
```cpp
// Display settings
#define TFT_WIDTH  320
#define TFT_HEIGHT 240

// Task stack sizes (bytes)
#define UI_TASK_STACK_SIZE     8192
#define AI_TASK_STACK_SIZE     6144
#define SCAN_TASK_STACK_SIZE   4096
#define SYSTEM_TASK_STACK_SIZE 4096

// Update intervals (milliseconds)
#define UI_UPDATE_INTERVAL     30   // 33fps
#define AI_UPDATE_INTERVAL     200  // 5Hz
#define SCAN_INTERVAL          5000 // Every 5 seconds

// AI behavior thresholds
#define HIGH_WIFI_ACTIVITY_THRESHOLD  10
#define STRONG_BLE_SIGNAL_THRESHOLD   -50
#define LOW_MEMORY_THRESHOLD          10240
```

### Build Configuration (platformio.ini)
```ini
[env:esp32-s3-devkitc-1]
platform = espressif32
board = esp32-s3-devkitc-1
framework = arduino

build_flags = 
    -DCONFIG_SPIRAM_USE_CAPS_ALLOC=1
    -DLV_CONF_INCLUDE_SIMPLE
    -DCORE_DEBUG_LEVEL=3

lib_deps = 
    lvgl/lvgl@^8.3.0
    bodmer/TFT_eSPI@^2.5.0
    ESP Async WebServer@^1.2.3
```

---

## 🧪 Development

### Building and Testing
```bash
# Development build with debug symbols
make dev

# Release build with optimizations  
make release

# Run static analysis
make check

# Format code
make format

# Clean build files
make clean

# Show firmware size
make size
```

### Code Quality
- **Static analysis** with cppcheck
- **Code formatting** with clang-format
- **Pre-commit hooks** for quality assurance
- **Comprehensive logging** for debugging
- **Unit tests** for core functions
- **Memory leak detection** with heap monitoring

### Debugging
```bash
# Start debugging session
make debug

# Monitor serial output with filtering
make monitor | grep "🧠\|❌\|⚠️"

# Profile memory usage
make profile
```

---

## 📊 Performance Metrics

### System Performance
- **Boot time**: ~3-5 seconds
- **UI frame rate**: 30fps stable
- **AI inference**: 5Hz (200ms intervals)
- **Network scan rate**: Every 5 seconds
- **Memory usage**: <60% with PSRAM
- **Power consumption**: ~150-200mA @ 3.3V

### Memory Usage
```
Component           | Internal RAM | PSRAM    | Stack
--------------------|-------------|----------|--------
UI Task (LVGL)      | 32KB        | 128KB    | 8KB
AI Inference        | 16KB        | 64KB     | 6KB  
Network Scanning    | 24KB        | 32KB     | 4KB
System Monitoring   | 8KB         | 16KB     | 4KB
Display Buffers     | -           | 256KB    | -
Total               | ~80KB       | ~496KB   | 22KB
```

---

## 🤝 Contributing

### Development Setup
1. **Fork** the repository
2. **Install** PlatformIO and dependencies
3. **Create** feature branch (`git checkout -b feature/amazing-feature`)
4. **Install** pre-commit hooks (`cp pre-commit.sh .git/hooks/pre-commit`)
5. **Make changes** with comprehensive testing
6. **Commit** with conventional commit messages
7. **Push** to branch and create pull request

### Code Standards
- **C++17** standard compliance
- **Google C++ Style Guide** for formatting
- **Comprehensive documentation** with Doxygen
- **Unit tests** for all public functions
- **Error handling** with proper logging
- **Memory safety** with RAII principles

### Commit Message Format
```
type(scope): description

feat(ai): add new behavioral state for tracking
fix(display): resolve animation flickering issue
docs(readme): update installation instructions
test(scan): add unit tests for BLE scanning
```

---

## 📄 License

This project is licensed under the **MIT License** - see the [LICENSE](LICENSE) file for details.

---

## 🙏 Acknowledgments

- **Espressif Systems** for the ESP32-S3 platform
- **LVGL Team** for the graphics library
- **PlatformIO** for the development environment
- **Community contributors** for testing and feedback
- **Ponagotchi Project** for inspiration

---

## 📞 Support

- **Issues**: [GitHub Issues](https://github.com/your-org/hydraesp-ai-edition/issues)
- **Discussions**: [GitHub Discussions](https://github.com/your-org/hydraesp-ai-edition/discussions)
- **Discord**: [HydraESP Community](https://discord.gg/hydraesp)
- **Email**: support@hydraesp.com

---

**🚀 Happy hacking with HydraESP AI Edition!**