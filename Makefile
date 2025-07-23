# Makefile for HydraESP AI Edition ESP32-S3 Project
# Uses PlatformIO for ESP32 development

SHELL := /bin/bash
.DEFAULT_GOAL := help

# Project Configuration
PROJECT_NAME := hydraesp-ai
BOARD := esp32-s3-devkitc-1
FRAMEWORK := arduino

# PlatformIO Commands
PIO := pio
PIO_ENV := --environment $(BOARD)

# Serial Monitor Settings
MONITOR_PORT := /dev/ttyUSB0
MONITOR_SPEED := 115200

.PHONY: help
help: ## Show this help message
	@echo "🧠 HydraESP AI Edition - ESP32-S3 Build System"
	@echo "================================================"
	@grep -E '^[a-zA-Z_-]+:.*?## ' $(MAKEFILE_LIST) | \
		awk 'BEGIN {FS = ":.*?## "}; {printf "\033[36m%-15s\033[0m %s\n", $$1, $$2}'
	@echo ""

.PHONY: install
install: ## Install PlatformIO dependencies
	@echo "📦 Installing PlatformIO dependencies..."
	$(PIO) pkg install $(PIO_ENV)

.PHONY: build
build: ## Build the project
	@echo "🔨 Building HydraESP AI Edition..."
	$(PIO) run $(PIO_ENV)

.PHONY: upload
upload: ## Upload firmware to ESP32-S3
	@echo "⬆️  Uploading firmware to ESP32-S3..."
	$(PIO) run $(PIO_ENV) --target upload

.PHONY: monitor
monitor: ## Start serial monitor
	@echo "📺 Starting serial monitor..."
	$(PIO) device monitor --port $(MONITOR_PORT) --baud $(MONITOR_SPEED)

.PHONY: flash
flash: build upload ## Build and upload firmware
	@echo "✅ Firmware flashed successfully!"

.PHONY: clean
clean: ## Clean build files
	@echo "🧹 Cleaning build files..."
	$(PIO) run $(PIO_ENV) --target clean
	rm -rf .pio/build

.PHONY: test
test: ## Run unit tests
	@echo "🧪 Running unit tests..."
	$(PIO) test $(PIO_ENV)

.PHONY: check
check: ## Static code analysis
	@echo "🔍 Running static code analysis..."
	$(PIO) check $(PIO_ENV) --severity=medium

.PHONY: format
format: ## Format code with clang-format
	@echo "🎨 Formatting code..."
	find src include -name "*.cpp" -o -name "*.h" | xargs clang-format -i

.PHONY: size
size: build ## Show firmware size information
	@echo "📏 Firmware size analysis:"
	$(PIO) run $(PIO_ENV) --target size

.PHONY: erase
erase: ## Erase ESP32 flash memory
	@echo "🗑️  Erasing ESP32 flash memory..."
	$(PIO) run $(PIO_ENV) --target erase

.PHONY: bootloader
bootloader: ## Upload bootloader
	@echo "🥾 Uploading bootloader..."
	$(PIO) run $(PIO_ENV) --target bootloader

.PHONY: partition
partition: ## Upload partition table
	@echo "📊 Uploading partition table..."
	$(PIO) run $(PIO_ENV) --target uploadfs

.PHONY: ota
ota: ## Over-the-air update via WiFi
	@echo "📡 Starting OTA update..."
	$(PIO) run $(PIO_ENV) --target upload --upload-port $(OTA_HOST)

.PHONY: debug
debug: ## Start debugging session
	@echo "🐛 Starting debug session..."
	$(PIO) debug $(PIO_ENV)

.PHONY: docs
docs: ## Generate documentation
	@echo "📚 Generating documentation..."
	doxygen Doxyfile

.PHONY: setup
setup: ## Initial project setup
	@echo "⚙️  Setting up HydraESP AI Edition project..."
	$(PIO) project init --board $(BOARD)
	@echo "✅ Project setup complete!"

.PHONY: update
update: ## Update PlatformIO libraries
	@echo "🔄 Updating libraries..."
	$(PIO) pkg update $(PIO_ENV)

.PHONY: info
info: ## Show project information
	@echo "ℹ️  Project Information:"
	@echo "========================"
	@echo "Project: $(PROJECT_NAME)"
	@echo "Board: $(BOARD)"
	@echo "Framework: $(FRAMEWORK)"
	$(PIO) system info

.PHONY: lint
lint: ## Lint code with cppcheck
	@echo "📝 Linting code..."
	cppcheck --enable=all --inconclusive --std=c++11 src/ include/

.PHONY: install-tools
install-tools: ## Install development tools
	@echo "🛠️  Installing development tools..."
	pip install platformio
	sudo apt-get install clang-format cppcheck doxygen

.PHONY: continuous
continuous: ## Continuous build on file changes
	@echo "👀 Watching for file changes..."
	while inotifywait -r -e modify src/ include/; do make build; done

# Environment-specific targets
.PHONY: dev
dev: ## Development build with debug symbols
	$(PIO) run --environment $(BOARD) --build-flags="-DDEBUG_LEVEL=5"

.PHONY: release
release: ## Release build with optimizations
	$(PIO) run --environment $(BOARD) --build-flags="-O2 -DNDEBUG"

.PHONY: profile
profile: ## Profile build for performance analysis
	$(PIO) run --environment $(BOARD) --build-flags="-DPROFILE_ENABLED"