# Experimental melonDS Switch 2 port

Based on Hydr8gon/melonDS `switch` commit 8fb3ae19af5a768a2e4fe6fc8fbacdc857b83103
(melonDS 0.8.3 frontend). GPL-3.0-or-later; see LICENSE and upstream notices.

This is an experimental Nintendo DS emulator port for Nintendo Switch 2.
A user has confirmed Pokémon Black 2 starts after correcting the save database
and save size. Broader game compatibility and performance remain unverified.

This is an unofficial port, based on the older Switch frontend. It is not an
official melonDS release. See [upstream documentation](README.upstream.md).

Changes:
- ounce-devkit/libounce build rules and Switch 2 CPU flags.
- OunceVK EGL/OpenGL instead of the Switch 1 Nouveau libraries.
- Current pad/touch API adapter, current thread creation signatures.
- Explicit OpenGL 3.3 core context.
- Removed Switch 1 clock-profile calls.

This older frontend uses the interpreter, not a kaiHEN JIT. Expect performance limitations.
The newer RSDuck frontend requires a deko3d renderer replacement and JIT integration.

## Building

Clone this repository and [ounce-devkit](https://github.com/ouncebrew/ounce-devkit)
into the same parent directory. The tested ounce-devkit revision is
`75cea5538b3a57d8f2ecf53354274cf00a365d20`. From that parent directory:

```sh
docker build -t melonds-ounce-devkit ounce-devkit
docker run --rm -v "$PWD/melonds-switch2-port:/work/melonds" -w /work/melonds \
  melonds-ounce-devkit sh -c 'dkp-pacman -S --needed --noconfirm switch-glad && make -j4'
```

## Files for a hardware test

Copy **both** files into `/switch/melonds/`:

- `melonds-switch2.nro` from your build.
- `romlist.bin` from this repository.

The database is required for correct game save sizes. If it is missing, an
unknown game defaults to an 8 KB save; Pokémon Black 2 requires 512 KB. Installing
the database does not resize saves already created incorrectly. Back up those
saves before attempting repair. Saves are stored beside the game with the same
basename and a `.sav` extension. Do not load an old save state after repairing
a save file.

Generate a complete install ZIP after building with:

```sh
python3 tools/package-release.py
```
Put `.nds` game dumps in `/roms/nds/` and select one in the file browser.

Original DS BIOS/firmware dumps are optional. On launch, missing `bios7.bin`,
`bios9.bin`, and `firmware.bin` are created in `/switch/melonds/` from the open-source
replacement BIOS and generated settings. Existing valid system files are preserved;
malformed files report an error instead of being overwritten. Replacement mode forces
direct game boot; it cannot start the original DS menu. Generated firmware contains no
Nintendo boot code. This does not add DSi support or validate game compatibility.

Replacement BIOS: Gilead Kutnick's custom BIOS as distributed by melonDS commit
7897bd387bfd37615a049eba28d02dc23cfa5194. See FREEBIOS-LICENSE.txt; copyright and
BSD-style license are retained in src/FreeBIOS.cpp. Firmware calibration tables and
layout come from that melonDS revision's SPI_Firmware.cpp (GPL-3.0-or-later).

Keep existing saves backed up before testing an unverified emulator port.
Startup diagnostics: `/switch/melonds/switch2-startup.log`.

## Validation

Clean ounce-devkit cross-build passed. Host tests check generated file sizes,
firmware header, checksums and calibration, repeated startup, preservation of
existing files, malformed-file handling and filesystem errors. The user has confirmed the file browser and Pokémon Black 2 startup on
Switch 2. There has been no broad compatibility, sustained gameplay, or speed validation.

Host test (pass an empty temporary directory):

```sh
clang++ -std=c++17 -Isrc src/BuiltinSystem.cpp src/FreeBIOS.cpp \
  tests/builtin_system.cpp -o /tmp/melonds-builtin-test
/tmp/melonds-builtin-test /path/to/empty/test-directory
```

## Layout and controls

The existing Switch frontend provides screen layouts through its settings menu
(ZL + ZR by default). Translucent picture-in-picture and a dedicated instant
screen-swap control are planned; they are not implemented in this version.

## Credits

- melonDS contributors and Arisotura: emulator core.
- Hydr8gon: original Nintendo Switch frontend.
- ouncebrew contributors: Switch 2 toolchain and libraries.
- Gilead Kutnick: open-source replacement BIOS, with its retained BSD-style license.

The emulator is GPL-3.0-or-later; see `LICENSE`. The replacement BIOS has separate
notices in `FREEBIOS-LICENSE.txt` and `src/FreeBIOS.cpp`. No game dumps, original
Nintendo BIOS dumps, personal save files, or runtime logs are included.
