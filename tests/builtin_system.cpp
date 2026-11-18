#include "BuiltinSystem.h"
#include <cassert>
#include <filesystem>
#include <fstream>
#include <vector>
using namespace BuiltinSystem;
namespace fs=std::filesystem;
static unsigned crc(const uint8_t* p,size_t n,unsigned v) {
    while(n--) {v^=*p++;for(int i=0;i<8;i++)v=(v>>1)^((v&1)?0xA001:0);}
    return v;
}
static unsigned word(const uint8_t* p) {return p[0]|(p[1]<<8);}
int main(int argc,char** argv) {
    assert(argc==2);fs::path dir=argv[1];fs::create_directories(dir);
    assert(EnsureFiles(dir.c_str())==1);
    assert(fs::file_size(dir/"bios7.bin")==0x4000);
    assert(fs::file_size(dir/"bios9.bin")==0x1000);
    assert(EnsureFiles(dir.c_str())==1); // replacement recognized on later launches
    auto f=GenerateFirmware();assert(f.size()==0x20000);
    assert(word(&f[0x20])*8==0x1FE00);
    assert(word(&f[0x2A])==crc(&f[0x2C],word(&f[0x2C]),0));
    for(size_t base:{0x1FA00,0x1FB00,0x1FC00})
        assert(word(&f[base+0xFE])==crc(&f[base],0xFE,0));
    for(size_t base:{0x1FE00,0x1FF00}) {
        assert(word(&f[base+0x72])==crc(&f[base],0x70,0xFFFF));
        assert(word(&f[base+0x5E])==255*16 && f[base+0x63]==191);
    }
    // Valid custom files survive, without being replaced by generated ones.
    for(auto name:{"bios7.bin","bios9.bin","firmware.bin"}) {
        std::fstream file(dir/name,std::ios::in|std::ios::out|std::ios::binary);
        file.seekp(0);file.put('X');
        if(std::string(name)=="firmware.bin") {file.seekp(8);file.write("USER",4);}
    }
    assert(EnsureFiles(dir.c_str())==0);
    {std::ifstream file(dir/"bios7.bin",std::ios::binary);assert(file.get()=='X');}
    // Malformed files produce an error; never silently overwrite them.
    {std::ofstream file(dir/"bios9.bin",std::ios::binary|std::ios::trunc);file<<"bad";}
    assert(EnsureFiles(dir.c_str())==-1);assert(fs::file_size(dir/"bios9.bin")==3);
    assert(EnsureFiles((dir/"does-not-exist").c_str())==-1);
}
