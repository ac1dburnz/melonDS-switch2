"""Package the Switch 2 build with its required save database and notices."""
from pathlib import Path
import hashlib
import struct
import zipfile
root = Path(__file__).resolve().parents[1]
nro = root / "melonds-switch2.nro"
assert nro.read_bytes()[0x10:0x14] == b"NRO0", "Build the NRO first"
db = (root / "romlist.bin").read_bytes()
assert len(db) % 16 == 0
entries = [struct.unpack_from("<4I", db, i) for i in range(0, len(db), 16)]
assert next(e for e in entries if e[0] == int.from_bytes(b"IREO", "little"))[2] == 6
out = root / "dist" / "melonds-switch2-experimental.zip"
out.parent.mkdir(exist_ok=True)
files = ["melonds-switch2.nro", "romlist.bin", "LICENSE", "FREEBIOS-LICENSE.txt", "README.switch2.md"]
with zipfile.ZipFile(out, "w", zipfile.ZIP_DEFLATED) as z:
    for name in files:
        z.write(root / name, "switch/melonds/" + name)
with zipfile.ZipFile(out) as z:
    assert z.testzip() is None
print(out)
print("SHA256", hashlib.sha256(out.read_bytes()).hexdigest())
