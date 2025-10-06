'use strict';

const TARGET_FUNCS = new Set([
  'RegSetValueExW','RegSetValueExA',
  'RegSetValueW','RegSetValueA',
  'RegGetValueW','RegGetValueA'
]);

const HKEY_NAMES = new Map([
  [ptr('0x80000000'), 'HKEY_CLASSES_ROOT'],
  [ptr('0x80000001'), 'HKEY_CURRENT_USER'],
  [ptr('0x80000002'), 'HKEY_LOCAL_MACHINE'],
  [ptr('0x80000003'), 'HKEY_USERS'],
  [ptr('0x80000004'), 'HKEY_PERFORMANCE_DATA'],
  [ptr('0x80000005'), 'HKEY_CURRENT_CONFIG'],
  [ptr('0x80000006'), 'HKEY_DYN_DATA']
]);

function hkeyToString(h) {
  const name = HKEY_NAMES.get(h);
  return name ? name : h.toString();
}

function safeReadAnsi(p) {
  try { return p.isNull() ? null : p.readCString(); } catch (_) { return null; }
}
function safeReadUtf16(p) {
  try { return p.isNull() ? null : p.readUtf16String(); } catch (_) { return null; }
}

const attached = new Set();

function attachAt(addr, fnName, modName) {
  const key = addr.toString();
  if (attached.has(key)) return;
  attached.add(key);

  Interceptor.attach(addr, {
    onEnter(args) {
      let details = { fn: fnName, mod: modName, addr: addr.toString() };

      try {
        switch (fnName) {
          case 'RegSetValueExW':
          case 'RegSetValueExA': {
            details.hKey = hkeyToString(args[0]);
            details.valueName = fnName.endsWith('W') ? safeReadUtf16(args[1]) : safeReadAnsi(args[1]);
            break;
          }
          case 'RegSetValueW':
          case 'RegSetValueA': {
            details.hKey = hkeyToString(args[0]);
            details.subKey = fnName.endsWith('W') ? safeReadUtf16(args[1]) : safeReadAnsi(args[1]);
            break;
          }
          case 'RegGetValueW':
          case 'RegGetValueA': {
            details.hKey = hkeyToString(args[0]);
            details.subKey = fnName.endsWith('W') ? safeReadUtf16(args[1]) : safeReadAnsi(args[1]);
            details.valueName = fnName.endsWith('W') ? safeReadUtf16(args[2]) : safeReadAnsi(args[2]);
            break;
          }
        }
      } catch (_) {}

      send({ type: 'call', ...details });
    }
  });
}

function instrumentModule(mod) {
  try {
    for (const exp of mod.enumerateExports()) {
      if (exp.type !== 'function') continue;
      if (!TARGET_FUNCS.has(exp.name)) continue;
      attachAt(exp.address, exp.name, mod.name);
    }
  } catch (_) {}
}

function instrumentAllLoadedModules() {
  for (const m of Process.enumerateModules()) {
    instrumentModule(m);
  }
}

function hookLoader() {
  let ntdll;
  try { ntdll = Process.getModuleByName('ntdll.dll'); } catch (_) { return false; }

  let ldr;
  try { ldr = ntdll.getExportByName('LdrLoadDll'); } catch (_) { return false; }

  Interceptor.attach(ldr, {
    onEnter(args) {
      this.outModuleHandlePtr = args[3];
    },
    onLeave(retval) {
      if (retval.toInt32() >= 0 && this.outModuleHandlePtr && !this.outModuleHandlePtr.isNull()) {
        try {
          const base = this.outModuleHandlePtr.readPointer();
          if (!base.isNull()) {
            const mod = Process.getModuleByAddress(base);
            instrumentModule(mod);
          }
        } catch (_) {}
      }
    }
  });
  return true;
}

(function main() {
  instrumentAllLoadedModules();
  hookLoader();
  send({ type: 'status', msg: 'Registry call instrumentation ready' });
})();