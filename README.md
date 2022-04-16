# MechaPwn

> For questions @[balika on twitter.com](https://twitter.com/balika011).

Any issue opened that is not an actual issue, but a question will be closed.

> :warning: Disclaimer: DO NOT USE A ON A REAL DTL/DEX; This sets the QA flag that forces the use of the retail keystore. This would break memory card compatibility. :warning:
> :warning: Disclaimer: if you use FMCB, ensure that you have installed the cross-region version. :warning:

Real DEX (non-QA) flags have not been added to the public version of MechaPwn for your safety.

The authors hold no responsibility should you break/damage your PlayStation 2 console using this software

This tool can be used to change the region and configuration flags from Dragon (SCPH-5xxxx - SCPH-9xxxx + PSX DESR + Sony Bravia KDL-22PX300 tvcombo) based mechacon consoles.

Older PlayStation 2 units do NOT use a Dragon-based mechacon and therefore are not supported. No support is planned for those in the future.

## How to use?

0) Make sure if you are using FMCB **you have "cross-region" installed**.

1) Run MechaPwn once to install the exploit patch/payload. An initial backup of your mechacon EEPROM will be made to the USB mass storage device (keep it safe!), then power off the console by disconnecting it from the mains when asked (mechacon is on, even when the ps2 CPUs is off, so you MUST disconnect the power cord!)
2) Run MechaPwn again to choose which region you want.
3) Choose whether you want to use force unlock, restore the original patch set (and uninstall the exploit patch) or keep the patch from your mechacon backup.
_You will need to reinstall the exploit patch to change your region again_

### Explanations of the menu options

Retail-DEX (Debug) will set a QA Flagged DEX configuration/region and clear all common region flags. This allows mechacon to read discs from all regions as well as masterdiscs (the retail option does not). This also allows executing all-region kelfs. On slims, this also forces NTSC video mode and forces PS2 and PS1 titles to run in the NA region.

CEX (Retail) will just set the region flag of your choosing. This option also restricts other regions from running. Be careful - on FAT units installing CEX->USA on SCPH-50004 will result in that European disks will not boot due to mechacon region beeing NTSC, and North American disks will not boot due to OSD ps2logo checking for PAL.

The next screen will allow choosing between predefined region sets. On FATs after DEX all options have no effect. On slims, it will change OSD behavior (for example, language set) and DVD Player region.

### How does it work?

The Dragon-based MechaCon (SCPH-500xx and newer) store configuration flags and patches encrypted in their EEPROM, the patch DES key was eventually bruteforced that allows code execution on those units and for the full keystore to be dumped.

Normally, the patch area is write-protected and cannot be written to at runtime except while using PMAP in TEST mode (this requires soldering). Furthermore, the configuration area can only be written to when it is empty.

This is done to prevent an attacker or anyone outside of Sony's factory from overwriting the mechacon configuration.

However, an exploitable bug was found in the WriteConfig function that allows writing arbitrary data to the patch area. This allows writing a mechacon patch that disables the write protection on mechacon configuration bits and thus sets specific regions and flags to mechacon.

DEX-USA option allows the following:

On all units

- Disable disc region checks (ps1 and ps2 discs from all-region as well as masterdiscs mount with data accessible)
- Change the region the console reports as, as well as change the disc/KELF region that mechacon allows
- Boot burned PS1 backups from the console's original region directly from OSD
- Boot masterpatched PS2 backups from the console's original region by skipping the logo check (for example, by loading a disc using [wlaunchELF](https://github.com/ps2homebrew/wLaunchELF))
- Boot burned PS1 backups, original PS1 and PS2 disks from all-region by skipping the logo check (for example, by loading a disc using [wlaunchELF](https://github.com/ps2homebrew/wLaunchELF))

On all-region slim units and FAT units from regions North America or Asia additionally

- Boot burned PS1 backups, original PS1 and PS2 disks from all-region directly from OSD

FAQ:

### Why do PAL/NTSC-J FAT consoles do not play NTSC/PAL discs (on SCPH-70000 and earlier)?

The IOP ROM on those consoles has enforced strict logo decryption checks both in the PS1 and PS2 BOOTROM that the NTSC-U (also used in ASIA consoles) BOOTROM does not have.

### Why do consoles not run masterdiscs directly from the OSD?

Additional protection exists on the DSP, which is different in retail and debug consoles, the debug one allows mechacon to store the masterdisc xor key in its registers, the retail one does not.
Bypassing the logo check bypasses this protection (this can be done using [wlaunchELF](https://github.com/ps2homebrew/wLaunchELF) or a future cdvdman patch using a ps2 homebrew)

### My PS2 has a modchip. Will it work for me?

We don't know. This depends on what modchip you are using. It has been reported that some revisions of the modbo 4.0 chip do not truly disable when "disabled" causing conflicts/compatibility issues with MechaPwn region changes, whereas we have seen it work with a modbo 5.0 with disabling that does work as intended (with all modchip features still working when enabled on the converted console).

### Why did FreeMCBoot stop working?

Make sure you have the multi-region ("cross-region") version installed.

### What is Force Unlock?

Force-Unlock is a very much needed feature for homebrew applications, like Neo Geo emulation, PS2 Linux for reading CD and DVD-ROM discs.
What it's meant to do is unlock the disk drive for access.

> :warning: Important note: If you enable Force Unlock every CD and DVD will be detected as a PS2 disc, meaning it breaks PS1 games and DVD-Video**

### I have a message "Unknown ***, please report"?

That means that your console setup was untested. Please provide PS2Ident dump for further research.

### How to create master patched disk?

Not covered by this FAQ.
