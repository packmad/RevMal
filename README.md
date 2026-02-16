# RevMal Course Material

Reverse Engineering Malware (RevMal) is a scientifically grounded course dedicated to the reverse engineering of Windows malware at the binary level. 
The course combines rigorous theoretical foundations with extensive hands-on practice, providing a structured and research-driven approach to understanding modern malicious software.

The course bridges low-level systems knowledge and practical malware analysis. 
Students learn how compiled programs are constructed, how operating systems execute them, and how to systematically reconstruct high-level semantics from low-level machine code. 
Emphasis is placed on methodological reasoning, technical depth, and analytical rigor. 

Taught with ♥️ by [Simone Aonzo](https://simoneaonzo.it/) 🇮🇹 at [Eurecom](https://www.eurecom.fr/) 🇫🇷

---

## Topics and Slides

[Course Presentation](https://docs.google.com/presentation/d/1sCjSs3GfRF56yY5U3VI8Lnw06KV7MuIl908g5Lx5CCs/)

### Part 0 – Malware
- [Malicious Software](https://docs.google.com/presentation/d/1T7vyQmwIlOiL8dRcer7bZTofJCsoLOgPhK2gLLXOCLU/)


### Part I – SRE Foundations [[pdf](slides/Foundations.pdf)]
- Introduction
  - Reverse Engineering
  - Software Reverse Engineering
- Legal Considerations
- The Compilation Process
- Computer Architecture



### Part II – Operating System Mechanisms and File Formats [[pdf](slides/OS_mechanisms.pdf)]
- Library and System Calls
- Processes and Threads
- Authorization
  - Linux
  - Windows
- Virtual Memory
  - Memory APIs
  - Address Space Layout Randomization (ASLR)
- Dealing with “exceptional” situations
  - Linux Signals
  - Windows Exception Handling
- File Formats [[ImHex](https://docs.google.com/presentation/d/1kE0q6CLBCI1r-qQgidsG04wvP4krTuVOKiRusPa3RTA/)]
  - Windows Portable Executable



### Part III – Instruction Set Architecture (ISA) [[pdf](slides/ISA.pdf)]
- Introduction
- Intel x86 Architecture and Instructions
- Application Binary Interface (ABI)
  - x86_32
  - x86_64
- System Calls


### Part IV – Static & Dynamic Analysis [[pdf](slides/StaticAndDynamic.pdf)]
- Static Analysis [[Ghidra](https://docs.google.com/presentation/d/1TxCvwWVlZOxtaPs5zwnFMZNAbWhTnScjuHQ2N4GCD6I/edit?usp=sharing) + [Scripting](Ghidra/ghidra.pdf)]
  - Equality and Similarity
  - Structural Characteristics
  - Disassembling
  - Call/Control/Data Graphs
  - Lifting
  - Decompiling
- Dynamic Analysis [[x64dbg](https://docs.google.com/presentation/d/1VEpso-rWsI_PT2iIKwAxJNlxXVLx37o2dVoGGw8-lKg/), [Process Monitor](https://docs.google.com/presentation/d/1u4bPfGMd1jmidV23zGPB_SgXQHZ8Ga1p7EaD1awJ_EI/), [CAPEv2]()]
  - In-guest
  - Out-of-guest 
  - Sandbox



### Part V – Surreptitious Software [[pdf](slides/Surreptitious.pdf)]
- Obfuscation
  - Data
  - Control-flow
  - Anti-Disassembly
  - Packing
- Tamperproofing
- Watermarking
- Evasion
  - Anti-Debugging
  - Anti-Instrumentation
  - Runtime Environment detection
  - Timing

---

## Virtual Machines

### Passwords

Credentials (Windows and Ubuntu) are always:
- Username: `dude`
- Password: `dude`

While the password of archives with malware is always: 
`infected`

---


### Windows 11x64

SRE tools
- [Capa](https://github.com/mandiant/capa/)
- [CyberChef](https://gchq.github.io/CyberChef/)
- [DetectItEasy](https://github.com/horsicq/Detect-It-Easy)
- [ImHex](https://imhex.werwolv.net/)
- [ProcessExplorer](https://learn.microsoft.com/en-us/sysinternals/downloads/process-explorer)
- [ProcessMonitor](https://learn.microsoft.com/en-us/sysinternals/downloads/procmon)
- [Wireshark](https://www.wireshark.org/)
- [Yara](https://virustotal.github.io/yara/)
- [x64dbg](https://x64dbg.com/)
- [Ghidra](https://github.com/NationalSecurityAgency/ghidra)
  - [ret-sync](https://github.com/bootleg/ret-sync) with x64dbg
  - [Capa explorer](https://github.com/mandiant/capa/tree/master/capa/ghidra/plugin)


Development
- Visual Studio Code
- Visual Studio 2022

In order to update this project, open PowerShell
```
cd C:\Users\dude\Desktop\RevMal
git pull
```

---


### Gateway (Ubuntu 24.04)

Fake networks services by [FakeNet-NG](https://github.com/mandiant/flare-fakenet-ng)

`/home/dude/RevMal/gwScripts` is in the PATH environment variable, so this folder is included in the list of directories the shell searches for executable programs.

Scripts:
- `smbShare.sh {start|stop}` enables/disables (default disabled) the shared folder `UbuGwShare` between the host and Windows. 
- `fakenetStart.sh` starts the FakeNet service. All log files are created in the current folder, so it is worth creating a suitable one. To restore the NATted connection, restart the VM.

---

## License

The source code in this repository is licensed under the MIT License. 

The slides (PDFs and Google Slides) are licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License (CC BY-NC-ND 4.0).

---
