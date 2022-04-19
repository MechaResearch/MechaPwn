# MechaPwn

> For questions @[balika on twitter.com](https://twitter.com/balika011).

Any issue opened that is not an actual issue, but a question will be closed.

> :warning: Deckard[^Deckard] users: Disclaimer: if you use FMCB, ensure that you have installed the cross-region version. :warning:

Real DEX (non-QA) flags have not been added to the public version of MechaPwn for your safety.

The authors hold no responsibility should you break/damage your PlayStation 2 console using this software.

- This tool can be used to change the Mechacon region and configuration flags for Dragon[^Dragon] units.
- This tool can be used to change OSD region and language set and DVD player region for 70k[^70k] and Deckard[^Deckard] units.
- This tool can be used to change PS2 Disk region and PS1 Disk region for Deckard[^Deckard] units.

Older[^Pre-Dragon] consoles do NOT use a Dragon-based mechacon and therefore are not supported. No support is planned for those in the future.

### FMCB and DVD player

- Deckard[^Deckard]-only: FMCB uses `systemupdate exploit` that depends on `rom0:ROMVER` region letter. On Deckard[^Deckard] units MechaPwn can change the region letter: DEX option will install `A` region for any selected region, CEX option will use region-specific letter. To avoid broken FMCB, install FMCB cross-region before using MechaPwn.
- CEX option only: FMCB 1.8 and older are region locked. It will stop to work if you change region under CEX option. To avoid this - use only DEX option or upgrade to FMCB 1.9
- CEX option only: DVD player on Pre-Deckard[^Pre-Deckard] models is region locked. It (and DVD player based exploits like FreeDVDBoot) will stop to work if you change region under CEX option. To avoid this - use only DEX option or install DVD player update to memory card.
- Force Unlock will disable DVD player

## How to use?

0) Make sure if you are using FMCB **you have "cross-region" installed**.

1) Run MechaPwn once to install the exploit patch/payload. An initial backup of your mechacon EEPROM will be made to the USB mass storage device (keep it safe!), then power off the console by disconnecting it from the mains when asked (mechacon is on, even when the ps2 CPUs is off, so you MUST disconnect the power cord!)
2) Run MechaPwn again to choose which region you want.
3) Choose whether you want to use: keep the current patch from your mechacon backup, restore factory default or install force unlock. The safest option - keep the current patch.

_You will need to reinstall the exploit patch to change your region again_

### Explanations of the menu options

Retail-DEX (Debug) will set a QA Flagged DEX configuration/region and clear all common region flags. This allows mechacon to read discs from all regions as well as masterdiscs (the retail option does not). This also allows executing all-region kelfs. On Deckard[^Deckard], this also forces NTSC video mode and forces PS2 and PS1 titles to run in the NA region.

CEX (Retail) will just set the region flag of your choosing. This option also restricts other regions from running. Be careful - on FAT[^Fat] units installing CEX->USA on SCPH-50004 will result in that European disks will not boot due to mechacon region beeing NTSC, and North American disks will not boot due to OSD ps2logo checking for PAL.

The next screen will allow choosing between predefined region sets on slims (70k[^70k] and Deckard[^Deckard]). It will change OSD behavior (for example, language set) and DVD Player region.

### How does it work?

The Dragon[^Dragon]-based MechaCon store configuration flags and patches encrypted in their EEPROM, the patch DES key was eventually bruteforced that allows code execution on those units and for the full keystore to be dumped.

Normally, the patch area is write-protected and cannot be written to at runtime except while using PMAP in TEST mode (this requires soldering). Furthermore, the configuration area can only be written to when it is empty.

This is done to prevent an attacker or anyone outside of Sony's factory from overwriting the mechacon configuration.

However, an exploitable bug was found in the WriteConfig function that allows writing arbitrary data to the patch area. This allows writing a mechacon patch that disables the write protection on mechacon configuration bits and thus sets specific regions and flags to mechacon.

DEX option allows the following:

On all units

- Disable disc region checks (ps1 and ps2 discs from all-region as well as masterdiscs mount with data accessible)
- Change the region the console reports as, as well as change the disc/KELF region that mechacon allows
- Boot burned PS1 backups from the console's original region directly from OSD
- Boot masterpatched PS2 backups from the console's original region by skipping the logo check (for example, by loading a disc using [wlaunchELF](https://github.com/ps2homebrew/wLaunchELF))
- Boot burned PS1 backups, original PS1 and PS2 disks from all-region by skipping the logo check (for example, by loading a disc using [wlaunchELF](https://github.com/ps2homebrew/wLaunchELF))
- 70k[^70k] and Deckard[^Deckard] only: Change OSD and DVD region
- Deckard[^Deckard] only: force NTSC-U region and video mode for PS2/PS1 titles

On all-region Deckard[^Deckard] units, FAT[^Fat]/70k[^70k] units from regions North America or Asia additionally

- Boot burned PS1 backups, original PS1 and PS2 disks from all-region directly from OSD

### FAQ

#### Why do my screen get noisy and blurry?

Your TV doesnt support NTSC or PAL signal. MechaPwn can change default video mode, for example DEX on Deckard[^Deckard] units will force NTSC video mode. Choose CEX option or use different TV.

#### Why do PAL/NTSC-J consoles do not play NTSC/PAL discs (on SCPH-70000 and earlier)?

The IOP ROM on those consoles has enforced strict logo decryption checks both in the PS1 and PS2 BOOTROM that the NTSC-U (also used in ASIA consoles) BOOTROM does not have.

#### Why do consoles not run masterdiscs directly from the OSD?

Additional protection exists on the DSP, which is different in retail and debug consoles, the debug one allows mechacon to store the masterdisc XOR key in its registers, the retail one does not.
Bypassing the logo check bypasses this protection (this can be done using [wlaunchELF](https://github.com/ps2homebrew/wLaunchELF) or a future cdvdman patch using a ps2 homebrew)

#### My PS2 has a modchip. Will it work for me?

We don't know. This depends on what modchip you are using. It has been reported that some revisions of the modbo 4.0 chip do not truly disable when "disabled" causing conflicts/compatibility issues with MechaPwn region changes, whereas we have seen it work with a modbo 5.0 with disabling that does work as intended (with all modchip features still working when enabled on the converted console).

#### What is Force Unlock?

Force-Unlock is a very much needed feature for homebrew applications, like Neo Geo emulation, PS2 Linux for reading CD and DVD-ROM discs.
What it's meant to do is unlock the disk drive for access.

> :warning: Important note: If you enable Force Unlock every CD and DVD will be detected as a PS2 disc, meaning it breaks PS1 games and DVD-Video**

#### I have a message "Unknown ***, please report"?

That means that your console setup was untested. Please provide [PS2Ident](https://github.com/ps2homebrew/PS2Ident/releases/tag/stable) dump for further research.

#### How to create master patched disk?

Master patch creation isnt covered by this FAQ. Try to search for *PSX/PS2 Disc Patcher v3.0* or official SONY utilites.

### Playstation 2 models description

[^Pre-Dragon]: SCPH-1x000 - SCPH-3900x models.
[^Dragon]: SCPH-5xxxx - SCPH-9xxxx + PSX DESR + Sony Bravia KDL-22PX300 tvcombo. Dragon subdivided into:
[^Deckard]: SCPH-750xx - SCPH-900xx models + Sony Bravia KDL-22PX300 tvcombo
[^Pre-Deckard]: All the above:
[^Fat]: SCPH-5xxxx models
[^DESR]: PSX DVR recorders, Japan only, DESR-5xxx and DESR-7xxx.
[^70k]: SCPH-700xx models
