
#!/usr/bin/env bash
# Release preparation script for HydraESP AI Edition
# Automates version bumping, changelog generation, and release preparation

set -euo pipefail

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Configuration
PROJECT_NAME="HydraESP AI Edition"
VERSION_FILE="include/version.h"
CHANGELOG_FILE="CHANGELOG.md"
BUILD_DIR=".pio/build"
RELEASE_DIR="release"

print_status() {
    echo -e "${BLUE}🔧 $1${NC}"
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

# Function to get current version
get_current_version() {
    if [ -f "$VERSION_FILE" ]; then
        grep -o 'VERSION.*".*"' $VERSION_FILE | cut -d'"' -f2
    else
        echo "1.0.0"
    fi
}

# Function to bump version
bump_version() {
    local current_version=$1
    local bump_type=${2:-patch}
    
    IFS='.' read -ra VERSION_PARTS <<< "$current_version"
    local major=${VERSION_PARTS[0]}
    local minor=${VERSION_PARTS[1]}
    local patch=${VERSION_PARTS[2]}
    
    case $bump_type in
        major)
            major=$((major + 1))
            minor=0
            patch=0
            ;;
        minor)
            minor=$((minor + 1))
            patch=0
            ;;
        patch)
            patch=$((patch + 1))
            ;;
        *)
            echo "$current_version"
            return
            ;;
    esac
    
    echo "$major.$minor.$patch"
}

# Function to update version file
update_version_file() {
    local new_version=$1
    local build_date=$(date '+%Y-%m-%d %H:%M:%S')
    local git_hash=$(git rev-parse --short HEAD 2>/dev/null || echo "unknown")
    
    cat > $VERSION_FILE << EOF
#ifndef VERSION_H
#define VERSION_H

#define FIRMWARE_VERSION "$new_version"
#define BUILD_DATE "$build_date"
#define GIT_HASH "$git_hash"
#define PROJECT_NAME "$PROJECT_NAME"

// Version components
#define VERSION_MAJOR ${new_version%%.*}
#define VERSION_MINOR $(echo $new_version | cut -d'.' -f2)
#define VERSION_PATCH $(echo $new_version | cut -d'.' -f3)

#endif // VERSION_H
EOF
}

# Function to generate changelog entry
generate_changelog_entry() {
    local version=$1
    local date=$(date '+%Y-%m-%d')
    
    # Get commits since last tag
    local last_tag=$(git describe --tags --abbrev=0 2>/dev/null || echo "")
    local commit_range=""
    
    if [ -n "$last_tag" ]; then
        commit_range="$last_tag..HEAD"
    else
        commit_range="HEAD"
    fi
    
    echo "## [$version] - $date"
    echo ""
    
    # Categorize commits
    echo "### Added"
    git log $commit_range --oneline --grep="feat\|add" --pretty=format:"- %s" 2>/dev/null || echo "- No new features"
    echo ""
    echo ""
    
    echo "### Changed"
    git log $commit_range --oneline --grep="change\|update\|refactor" --pretty=format:"- %s" 2>/dev/null || echo "- No changes"
    echo ""
    echo ""
    
    echo "### Fixed"
    git log $commit_range --oneline --grep="fix\|bug" --pretty=format:"- %s" 2>/dev/null || echo "- No fixes"
    echo ""
    echo ""
    
    echo "### Technical"
    git log $commit_range --oneline --grep="docs\|test\|ci\|build" --pretty=format:"- %s" 2>/dev/null || echo "- No technical changes"
    echo ""
}

# Function to build firmware
build_firmware() {
    print_status "Building firmware for release..."
    
    # Clean previous builds
    make clean
    
    # Build release version
    make release
    
    # Check if build was successful
    if [ ! -f ".pio/build/esp32-s3-devkitc-1/firmware.bin" ]; then
        print_error "Firmware build failed!"
        exit 1
    fi
    
    print_success "Firmware built successfully"
}

# Function to create release package
create_release_package() {
    local version=$1
    
    print_status "Creating release package..."
    
    # Create release directory
    mkdir -p $RELEASE_DIR
    
    # Copy firmware binaries
    cp .pio/build/esp32-s3-devkitc-1/firmware.bin $RELEASE_DIR/hydraesp-ai-$version.bin
    cp .pio/build/esp32-s3-devkitc-1/bootloader.bin $RELEASE_DIR/bootloader-$version.bin
    cp .pio/build/esp32-s3-devkitc-1/partitions.bin $RELEASE_DIR/partitions-$version.bin
    
    # Create flash script
    cat > $RELEASE_DIR/flash-$version.sh << EOF
#!/bin/bash
# Flash script for HydraESP AI Edition v$version
# Usage: ./flash-$version.sh [PORT]

PORT=\${1:-/dev/ttyUSB0}

echo "🧠 Flashing HydraESP AI Edition v$version"
echo "Port: \$PORT"

esptool.py --chip esp32s3 --port \$PORT --baud 921600 \\
    --before default_reset --after hard_reset write_flash -z \\
    --flash_mode qio --flash_freq 80m --flash_size 16MB \\
    0x0 bootloader-$version.bin \\
    0x8000 partitions-$version.bin \\
    0x10000 hydraesp-ai-$version.bin

echo "✅ Flashing complete!"
EOF
    
    chmod +x $RELEASE_DIR/flash-$version.sh
    
    # Create release notes
    cat > $RELEASE_DIR/RELEASE_NOTES_$version.md << EOF
# HydraESP AI Edition v$version Release Notes

## Installation

### Prerequisites
- ESP32-S3 development board
- esptool.py installed
- USB cable for programming

### Quick Flash
\`\`\`bash
chmod +x flash-$version.sh
./flash-$version.sh /dev/ttyUSB0
\`\`\`

### Manual Flash
\`\`\`bash
esptool.py --chip esp32s3 --port /dev/ttyUSB0 --baud 921600 \\
    --before default_reset --after hard_reset write_flash -z \\
    --flash_mode qio --flash_freq 80m --flash_size 16MB \\
    0x0 bootloader-$version.bin \\
    0x8000 partitions-$version.bin \\
    0x10000 hydraesp-ai-$version.bin
\`\`\`

## Hardware Requirements
- ESP32-S3-DevKitC-1 with 16MB Flash + 8MB PSRAM
- ST7789 320x240 TFT Display
- Status LED (GPIO 22)

## What's New in v$version

$(generate_changelog_entry $version)

## Support
- Issues: GitHub Issues
- Documentation: README.md
- Community: Discord

Happy hacking with HydraESP AI Edition! 🚀
EOF
    
    # Create checksums
    cd $RELEASE_DIR
    sha256sum *.bin > checksums-$version.txt
    cd ..
    
    print_success "Release package created in $RELEASE_DIR/"
}

# Main script
main() {
    echo -e "${BLUE}🧠 $PROJECT_NAME - Release Preparation${NC}"
    echo "=============================================="
    
    # Check if we're in a git repository
    if ! git rev-parse --git-dir > /dev/null 2>&1; then
        print_warning "Not in a git repository. Continuing without git integration."
    fi
    
    # Get current version
    current_version=$(get_current_version)
    print_status "Current version: $current_version"
    
    # Ask for version bump type
    echo ""
    echo "Select version bump type:"
    echo "1) Patch (x.x.X) - Bug fixes"
    echo "2) Minor (x.X.x) - New features"
    echo "3) Major (X.x.x) - Breaking changes"
    echo "4) Custom version"
    echo ""
    read -p "Enter choice (1-4): " choice
    
    case $choice in
        1)
            new_version=$(bump_version $current_version patch)
            ;;
        2)
            new_version=$(bump_version $current_version minor)
            ;;
        3)
            new_version=$(bump_version $current_version major)
            ;;
        4)
            read -p "Enter custom version (x.y.z): " new_version
            ;;
        *)
            print_error "Invalid choice"
            exit 1
            ;;
    esac
    
    print_status "New version: $new_version"
    
    # Confirm release
    echo ""
    read -p "Proceed with release v$new_version? (y/N): " confirm
    if [[ ! $confirm =~ ^[Yy]$ ]]; then
        print_warning "Release cancelled"
        exit 0
    fi
    
    # Update version file
    print_status "Updating version file..."
    update_version_file $new_version
    print_success "Version file updated"
    
    # Update changelog
    print_status "Updating changelog..."
    if [ -f $CHANGELOG_FILE ]; then
        # Backup existing changelog
        cp $CHANGELOG_FILE ${CHANGELOG_FILE}.bak
        
        # Prepend new changelog entry
        {
            generate_changelog_entry $new_version
            echo ""
            cat ${CHANGELOG_FILE}.bak
        } > $CHANGELOG_FILE
        
        rm ${CHANGELOG_FILE}.bak
    else
        # Create new changelog
        cat > $CHANGELOG_FILE << EOF
# Changelog
All notable changes to HydraESP AI Edition will be documented in this file.

$(generate_changelog_entry $new_version)
EOF
    fi
    print_success "Changelog updated"
    
    # Build firmware
    build_firmware
    
    # Create release package
    create_release_package $new_version
    
    # Git operations (if in git repo)
    if git rev-parse --git-dir > /dev/null 2>&1; then
        print_status "Committing changes..."
        git add $VERSION_FILE $CHANGELOG_FILE
        git commit -m "chore: bump version to v$new_version"
        
        print_status "Creating git tag..."
        git tag -a "v$new_version" -m "Release v$new_version"
        
        print_success "Git tag v$new_version created"
        
        echo ""
        print_status "To push the release:"
        echo "git push origin main"
        echo "git push origin v$new_version"
    fi
    
    echo ""
    print_success "Release v$new_version prepared successfully!"
    echo ""
    echo "📦 Release files located in: $RELEASE_DIR/"
    echo "📝 Release notes: $RELEASE_DIR/RELEASE_NOTES_$new_version.md"
    echo "💾 Firmware binary: $RELEASE_DIR/hydraesp-ai-$new_version.bin"
    echo ""
}

# Run main function
main "$@"
