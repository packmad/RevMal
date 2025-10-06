import argparse
import time
import sys
import os
from os.path import isfile

import frida  # pip install frida-tools


try:
    sys.stdout.reconfigure(line_buffering=True)
except Exception:
    pass


def on_message(msg, data):
    if msg.get('type') == 'send':
        payload = msg['payload']
        if payload.get('type') == 'call':
            fn = payload.get('fn')
            mod = payload.get('mod')
            addr = payload.get('addr')
            line = f"[CALL] {fn} @ {addr} ({mod})"
            if 'hKey' in payload:
                line += f" hKey={payload['hKey']}"
            if 'subKey' in payload and payload['subKey']:
                line += f" subKey=\"{payload['subKey']}\""
            if 'valueName' in payload and payload['valueName']:
                line += f" valueName=\"{payload['valueName']}\""
            print(line, flush=True)
        elif payload.get('type') == 'status':
            print(f"[*] {payload['msg']}", flush=True)
    elif msg.get('type') == 'log':
        print(msg['payload'], flush=True)
    else:
        print(msg, flush=True)
        

def main(exe_path: str, js_source: str):
  device = frida.get_local_device()
  pid = device.spawn([exe_path])
  session = device.attach(pid)
  script = session.create_script(js_source)
  script.on('message', on_message)
  script.load()
  device.resume(pid)
  print(f"[*] Spawned and resumed PID {pid}. Waiting for events...")
  try:
    while True:
      time.sleep(0.4)
  except KeyboardInterrupt:
    pass


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description="Runs an exe instrumented with Frida.")
    parser.add_argument("-e", "--exe", required=True, help="Path to the executable")
    parser.add_argument("-j", "--js", required=True, help="Path to the JavaScript source file")
    args = parser.parse_args()
    assert isfile(args.exe)
    with open(args.js, "r", encoding="utf-8") as f:
      js_code = f.read()
      main(args.exe, js_code)
