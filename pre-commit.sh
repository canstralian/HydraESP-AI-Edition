#!/usr/bin/env bash
# Pre-commit hook for HydraESP AI Edition
# Ensures code quality and prevents bad commits

set -euo pipefail

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

echo -e "${BLUE}🧠 HydraESP AI Edition - Pre-commit Checks${NC}"
echo "=============================================="

# Function to print status messages
print_status() {
    echo -e "${BLUE}🔍 $1${NC}"
}

print_success() {
    echo -e "${GREEN}✅ $1${NC}"
}

print_warning() {
    echo -e "${YELLOW}⚠️  $1${NC}"
}

print_error() {
    echo -e "${RED}❌ $1${NC}"
}

# Check if PlatformIO is installed
if ! command -v pio &> /dev/null; then
    print_error "PlatformIO is not installed or not in PATH"
    echo "Please install PlatformIO: pip install platformio"
    exit 1
fi

# Get list of staged files
STAGED_FILES=$(git diff --cached --name-only --diff-filter=ACM)
CPP_FILES=$(echo "$STAGED_FILES" | grep -E '\.(cpp|h|ino)$' || true)

if [ -z "$CPP_FILES" ]; then
    print_success "No C++ files to check"
    exit 0
fi

print_status "Checking staged C++ files..."
echo "$CPP_FILES"

# 1. Code formatting check
print_status "Checking code formatting..."
if command -v clang-format &> /dev/null; then
    for file in $CPP_FILES; do
        if [ -f "$file" ]; then
            # Check if file needs formatting
            if ! clang-format --dry-run --Werror "$file" &> /dev/null; then
                print_error "File $file needs formatting"
                echo "Run: clang-format -i $file"
                exit 1
            fi
        fi
    done
    print_success "Code formatting is correct"
else
    print_warning "clang-format not found, skipping format check"
fi

# 2. Static analysis with cppcheck
print_status "Running static analysis..."
if command -v cppcheck &> /dev/null; then
    if ! cppcheck --quiet --error-exitcode=1 --enable=warning,style,performance,portability $CPP_FILES; then
        print_error "Static analysis failed"
        exit 1
    fi
    print_success "Static analysis passed"
else
    print_warning "cppcheck not found, skipping static analysis"
fi

# 3. Build check
print_status "Checking if project builds..."
if ! pio run --environment esp32-s3-devkitc-1 > /dev/null 2>&1; then
    print_error "Build failed"
    echo "Run 'make build' to see detailed error messages"
    exit 1
fi
print_success "Build successful"

# 4. Check for common issues
print_status "Checking for common issues..."

# Check for debug print statements that should be removed
if grep -n "Serial.print.*DEBUG\|cout.*DEBUG\|printf.*DEBUG" $CPP_FILES; then
    print_warning "Found debug print statements - consider removing for production"
fi

# Check for hardcoded WiFi credentials
if grep -n "wifi.*password\|ssid.*=\|password.*=" $CPP_FILES; then
    print_error "Possible hardcoded WiFi credentials found"
    echo "Please use configuration files or environment variables"
    exit 1
fi

# Check for TODO/FIXME comments
TODO_COUNT=$(grep -c "TODO\|FIXME" $CPP_FILES || true)
if [ "$TODO_COUNT" -gt 0 ]; then
    print_warning "Found $TODO_COUNT TODO/FIXME comments"
    grep -n "TODO\|FIXME" $CPP_FILES || true
fi

# 5. File size check
print_status "Checking file sizes..."
for file in $CPP_FILES; do
    if [ -f "$file" ]; then
        size=$(wc -l < "$file")
        if [ "$size" -gt 500 ]; then
            print_warning "File $file is large ($size lines) - consider splitting"
        fi
    fi
done

# 6. Check for proper header guards
print_status "Checking header guards..."
for file in $(echo "$CPP_FILES" | grep -E '\.h$' || true); do
    if [ -f "$file" ]; then
        if ! grep -q "#ifndef.*_H\|#pragma once" "$file"; then
            print_error "Header file $file missing header guard"
            exit 1
        fi
    fi
done

# 7. Memory usage check (basic)
print_status "Checking for potential memory issues..."
if grep -n "malloc\|free\|new\|delete" $CPP_FILES; then
    print_warning "Manual memory management detected - ensure proper cleanup"
fi

# 8. Check configuration consistency
print_status "Checking configuration consistency..."
if [ -f "include/config.h" ]; then
    # Check if all defined pins are used
    DEFINED_PINS=$(grep -o "#define.*_PIN.*[0-9]" include/config.h | wc -l || true)
    if [ "$DEFINED_PINS" -gt 0 ]; then
        print_success "Configuration file looks good"
    fi
fi

print_success "All pre-commit checks passed!"
echo -e "${GREEN}🚀 Ready to commit${NC}"
exit 0