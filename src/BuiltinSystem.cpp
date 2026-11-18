// SPDX-License-Identifier: GPL-3.0-or-later
// Generated settings layout follows melonDS SPI_Firmware.cpp (GPL-3.0-or-later).
#include "BuiltinSystem.h"
#include <cstdio>
#include <cstring>
#include <string>
#include <cerrno>
namespace BuiltinSystem {
#include "FirmwareDefaults.inc"
static void put16(uint8_t* p, uint16_t x) { p[0] = x; p[1] = x >> 8; }
static uint16_t crc(const uint8_t* p, size_t size, uint16_t v) {
    for (size_t i = 0; i < size; ++i) {
        v ^= p[i];
        for (int j = 0; j < 8; ++j) v = (v >> 1) ^ ((v & 1) ? 0xA001 : 0);
    }
    return v;
}
std::vector<uint8_t> GenerateFirmware() {
    std::vector<uint8_t> f(FirmwareSize, 0xFF);
    std::memset(f.data(), 0, 0x1D);
    std::memcpy(f.data()+8, "MELN", 4);
    f[0x1D] = 0x20; // DS Lite
    put16(&f[0x20], 0x1FE00 >> 3);
    put16(&f[0x2C], 0x138);
    f[0x2E] = 0; f[0x2F] = 6;
    std::memcpy(&f[0x30], DEFAULT_UNUSED3, sizeof(DEFAULT_UNUSED3));
    const uint8_t mac[] = {0x00,0x09,0xBF,0x11,0x22,0x33};
    std::memcpy(&f[0x36], mac, sizeof(mac));
    put16(&f[0x3C], 0x3FFE);
    f[0x40]=3; f[0x41]=0x94; f[0x42]=0x29; f[0x43]=2;
    const uint16_t init[] = {2,0x17,0x26,0x1818,0x48,0x4840,0x58,0x42,0x146,0x8064,0xE6E6,0x2443,0xE,1,1,0x402};
    for (size_t i=0;i<16;++i) put16(&f[0x44+i*2],init[i]);
    std::memcpy(&f[0x64],BBINIT,sizeof(BBINIT));f[0xCD]=0;
    std::memcpy(&f[0xCE],RFINIT,sizeof(RFINIT));f[0xF7]=2;
    std::memcpy(&f[0xF8],CHANDATA,sizeof(CHANDATA));
    put16(&f[0x2A],crc(&f[0x2C],0x138,0));
    for (size_t base : {0x1FA00,0x1FB00,0x1FC00}) {
        std::memset(&f[base],0,0x100);
        f[base+0xE7]=0xFF; // unconfigured access point
        f[base+0xEF]=1;
        put16(&f[base+0xFE],crc(&f[base],0xFE,0));
    }
    for (size_t base : {0x1FE00,0x1FF00}) {
        std::memset(&f[base],0,0x74);
        put16(&f[base],5);f[base+3]=1;f[base+4]=1;
        const char* name="melonDS";
        for (size_t i=0;i<7;++i) put16(&f[base+6+i*2],name[i]);
        put16(&f[base+0x1A],7);
        put16(&f[base+0x5E],255<<4);put16(&f[base+0x60],191<<4);
        f[base+0x62]=255;f[base+0x63]=191;
        put16(&f[base+0x64],1 | (3<<4)); // English, brightest backlight
        put16(&f[base+0x72],crc(&f[base],0x70,0xFFFF));
    }
    return f;
}
static int ensure(const std::string& path, const uint8_t* data, size_t size, bool firmware) {
    FILE* file=std::fopen(path.c_str(),"rb");
    if (file) {
        std::fseek(file,0,SEEK_END);long length=std::ftell(file);std::rewind(file);
        bool valid=firmware ? (length==0x20000 || length==0x40000 || length==0x80000) : length==static_cast<long>(size);
        if (!valid) {std::fclose(file);return -1;}
        std::array<uint8_t,0x4000> buf{};
        size_t got=std::fread(buf.data(),1,firmware ? 12 : size,file);
        std::fclose(file);
        if (got!=(firmware ? 12 : size)) return -1;
        return firmware ? std::memcmp(buf.data()+8,"MELN",4)==0 : std::memcmp(buf.data(),data,size)==0;
    }
    if (errno!=ENOENT) return -1;
    const auto tmp=path+".generating";
    file=std::fopen(tmp.c_str(),"wb");
    if (!file) return -1;
    bool ok=std::fwrite(data,1,size,file)==size;
    if (std::fclose(file)!=0) ok=false;
    if (!ok || std::rename(tmp.c_str(),path.c_str())!=0) {std::remove(tmp.c_str());return -1;}
    return 1;
}
int EnsureFiles(const char* directory) {
    bool direct=false;
    for (int i=0;i<3;++i) {
        std::string path=std::string(directory)+"/";
        int result;
        if (i==0) result=ensure(path+"bios9.bin",bios_arm9_bin.data(),bios_arm9_bin.size(),false);
        else if (i==1) result=ensure(path+"bios7.bin",bios_arm7_bin.data(),bios_arm7_bin.size(),false);
        else {
            const auto firmware=GenerateFirmware();
            result=ensure(path+"firmware.bin",firmware.data(),firmware.size(),true);
        }
        if (result<0) return -1;
        direct=direct || result==1;
    }
    return direct ? 1 : 0;
}
}
