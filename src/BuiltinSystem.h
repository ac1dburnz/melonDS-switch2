// SPDX-License-Identifier: GPL-3.0-or-later
#pragma once
#include <array>
#include <vector>
#include <cstdint>
#include <cstddef>
namespace BuiltinSystem {
extern std::array<uint8_t, 0x4000> bios_arm7_bin;
extern std::array<uint8_t, 0x1000> bios_arm9_bin;
constexpr size_t FirmwareSize = 0x20000;
std::vector<uint8_t> GenerateFirmware();
// Creates only missing system files. Returns -1 on invalid existing files or I/O
// failure, 1 when replacement software requires direct boot, otherwise 0.
int EnsureFiles(const char* directory);
}
