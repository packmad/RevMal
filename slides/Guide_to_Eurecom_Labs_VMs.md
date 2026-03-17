# 🖥️ RevMal – Starting VMs in EURECOM's Linux Labs

Step-by-step guide to launching and connecting to VMware virtual machines in the EURECOM Linux labs for the **Reverse Engineering & Malware Analysis (RevMal)** course.

---

## Prerequisites

- Be logged into a **physical Linux lab machine** at EURECOM
- Have access to your home directory (`/home/Local_Data/`)

---

## Step 1 – Launch VMware

Open a terminal on the physical lab machine and run the following command:

```bash
umask 0000 && vmware
```

> **Why `umask 0000`?**  
> This sets default file permissions so that files created by VMware are accessible without permission issues in the shared lab environment.

---

## Step 2 – Open the RevMal VMs

Once VMware is open, navigate to the VM files as follows:

1. Go to **File → Open**
2. In the file browser that opens, click on **Other Locations** in the left sidebar
3. Click on **Computer**
4. Navigate to **home → Local_Data → RevMal**

Inside the `RevMal` folder you will find **two subfolders**, one for the **Linux VM** and one for the **Windows VM**. They need to be opened **one at a time**:

| VM | Folder name |
|---|---|
| Windows | `Windows11x64` |
| Linux | `GatewayUbuntu24.04x64` |

- Enter the respective subfolder
- Select the `.vmx` file
- VMware will load and start the virtual machine

Repeat for the other VM when needed.

---



## Step 3 – VMware Import Password

When opening the VMs for the first time, VMware will ask for a password to **import** them. Enter:

```
dudedude
```

Once inside the VM, the **guest OS login password** is the same for both the **Windows** and **Linux** VMs:

```
dude
```

---

## Step 4 – Connect to the VM via RDP (Remmina)

If you prefer to work through a remote desktop connection instead of the VMware window, you can use **Remmina** from the physical lab machine.

### 4.1 – Launch Remmina

From the terminal on the **physical machine** (not inside the VM), run:

```bash
remmina
```

### 4.2 – Configure the RDP Connection

1. In the top bar of Remmina, select the **RDP** protocol
2. Enter the VM's IP address:

```
192.168.255.102
```

3. Press **Enter** or click **Connect**

### 4.3 – RDP Credentials

| Field | Value |
|---|---|
| Username | `dude` |
| Password | `dude` |
| Domain | *(leave blank)* |

> The RDP credentials are the same as the direct login credentials for the VM. If Remmina asks for a **domain**, just leave that field empty.
