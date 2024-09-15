# MechaPwn

[![Twitter](https://img.shields.io/twitter/url/https/twitter.com/cloudposse.svg?style=flat&logo=Twitter&label=Follow%20%40balika011)](https://twitter.com/balika011) [![Discord](https://img.shields.io/discord/652861436992946216?style=flat&logo=Discord)](https://discord.gg/CVFUa9xh6B)

Any issue opened that is not an actual issue, but a question will be closed.

> [!WARNING]
> Deckard[^3] users: Disclaimer: if you use FMCB, ensure that you have installed the **cross-region** version.

> [!WARNING] 
> non-Deckard (FAT[^5] and 70k[^7]) users: Disclaimer: bad quality disks can damage your laser (with or without MechaPwn, doesn't matter). More info [here](#dsp-limitations). :warning:

Real DEX (non-QA) flags have not been added to the public version of MechaPwn for your safety.

The authors hold no responsibility should you break/damage your PlayStation 2 console using this software.

- This tool can be used to change the Mechacon region and configuration flags for Dragon[^2] units.
- This tool can be used to change the OSD region/language set and DVD player region for 70k[^7] and Deckard[^3] units.
- This tool can be used to change the PS2 Disk region and PS1 Disk region for Deckard[^3] units.

Older[^1] consoles do NOT use a Dragon-based mechacon and therefore are not supported. No support is planned for those in the future. But you will get a valid NVRAM backup and basic information about your Mechacon chip and console.

## FMCB and DVD player

- Deckard[^3]-only: FMCB uses `system update exploit` depending on `rom0:ROMVER` region letter. On Deckard[^3] units, MechaPwn can change the region letter: the DEX option will install `A` region for any selected region, CEX option will use a region-specific letter. To avoid broken FMCB, install the FMCB cross-region before using MechaPwn.
- CEX option only: FMCB 1.8 and older are region locked. It will stop working if you change the region under the CEX option. To avoid this - use only the DEX option or upgrade to FMCB 1.9
- CEX option only: DVD player on Pre-Deckard[^4] models is region locked. It (and DVD player-based exploits like FreeDVDBoot) will stop working if you change the region under the CEX option. To avoid this - use only the DEX option or install a DVD player update to the memory card.
- Force Unlock will disable the DVD player
- Deckard[^3]-only, PAL and Japan units only: FMCB, dvdplayer hack from krHACKen, gameplay history, etc will change their location. If you don't want to lose functionality, ensure that all `B?EXEC-` and `B?SYSTEM-` folders have their content copied into respective `BAEXEC-` and `BASYSTEM-` folders.

## How to use it?

0) Make sure if you are using FMCB **you have "CROSS-REGION" installed**.

1) Run MechaPwn once to install the exploit patch/payload. An initial backup of your mechacon EEPROM will be made to the USB mass storage device (keep it safe!), then power off the console by disconnecting it from the mains when asked (mechacon is on, even when the ps2 CPUs is off, so you MUST disconnect the power cord!)
2) Run MechaPwn again to choose which region you want.
3) Choose whether you want to use: keep the current patch from your mechacon backup, restore factory default or install force unlock. The safest option - keep the current patch.

_You will need to reinstall the exploit patch to change your region again_

### Explanations of the app screens

MechaPwn logo will wait until the USB device is ready.

Backup nvram. MechaPwn will backup your console NVRAM data to the USB device. This will work on any console, so the app can be used for dumping NVRAM.

Information screen. Will show data about your console: Serial number, model ID, real model name, mechacon firmware version and timestamp. If your console is untested or unsupported, MechaPwn will block itself. If you see a message "please report ..." feel free to open an issue and your console data will be tested for compatibility with MechaPwn.

FMCB check. For slim consoles, MechaPwn will check that FMCB is installed for the US region. Again, please use FMCB cross-region install. If you have a compatible FMCB, this screen will be skipped.

The next screen will allow you to change the region or restore the saved NVRAM backup from the USB.

Console type screen. DEX will set a QA Flagged DEX configuration/region and clear all common region flags. This allows mechacon to read discs from all regions as well as masterdiscs (the retail option does not). This also allows executing all-region kelfs. On Deckard[^3], this also forces NTSC video mode and forces PS2 and PS1 titles to run in the NA region. On FAT[^5] CEX option is disabled as confirmed to be too dangerous. On slims, CEX (Retail) will just set the region flag of your choosing. This option also restricts other regions from running.

The next screen will allow choosing between predefined region sets on slims (70k[^7] and Deckard[^3]). It will change OSD behavior (for example, language set) and DVD Player region. Keep in mind that the DEX type will use NTSC-U for PS2 and PS1 titles in any region.

Patch menu. This menu will allow you to keep the current patch, install factory defaults and install force unlock. Factory Defaults will allow you to uninstall Force Unlock.

### How does it work?

The Dragon[^2]-based MechaCon store configuration flags and patches were encrypted in their EEPROM, and the patch DES key was eventually brute-forced, thus allowing code execution on those units and for the full keystore to be dumped.

Normally, the patch area is write-protected and cannot be written to at runtime except while using PMAP in TEST mode (this requires soldering). Furthermore, the configuration area can only be written when it is empty.

This is done to prevent an attacker or anyone outside of Sony's factory from overwriting the mechacon configuration.

However, an exploitable bug was found in the WriteConfig function that allows writing arbitrary data to the patch area. This allows writing a mechacon patch that disables the write protection on mechacon configuration bits and thus sets specific regions and flags to mechacon.

### DSP limitations

1. Burning coils. It is NOT MECHAPWN fault!!! It is a hardware bug, you are risking every time you put the burned disk into FAT[^5] and 70k[^7]. Affects any FAT[^5] and 70k[^7] consoles. If a disc has bad ECC (error correction) data on the error correction area of each sector, there's a chance it may crash the DSP during a read, given that the coils are driven using PWM (pulse width modulation) there's a 50% chance it may crash while the coil is energized. The coil is not designed to receive constant current so it can be damaged. It doesn't crash with silver discs (without scrapes), it **may** crash with burnt discs, it only depends if the ECC data is bogus or not. While this problem is not caused by MechaPwn, our team understands that after MechaPwn you will have more reasons to put burned disks inside. You are warned! The only working fix for preventing this - is `Matrix PIC fix` that shut down the system in case of an ECC error. All other passive fixes are not working. The other way for fixing this: replace the DSP chip with some DTL DSP chip (that is marked -1). [More details about Matrix PIC fix](docs/PICfix.md).
2. DSP does not store disk keys (16 bytes) inside its registers. This only affects retail units, real DTL units do not have this `bug?/feature?` cause DTL units have another DSP chip revision. The mechacon copies disc ID data from the masterdisc sector into the DSP registers and later in CDVDMAN on DTL units. Retail DSP always read back from the disc. This will block MasterPatched disks from running directly from OSD.

## Program Advantages

DEX option allows the following:

On all units

- Disable disc region checks (ps1 and ps2 discs from all-region as well as masterdiscs mount with data accessible)
- Change the region the console reports as, as well as change the disc/KELF region that mechacon allows
- Boot burned PS1 backups from the console's original region directly from OSD
- Boot burned PS2 CD disks from the console's original region directly from OSD (without masterpatch)
- Boot masterpatched PS2 backups and original PS2 disks from all-region by skipping the logo check (for example, by loading a disc using [wlaunchELF](https://github.com/ps2homebrew/wLaunchELF))
- 70k[^7] and Deckard[^3] only: Change OSD and DVD region
- Deckard[^3] only: force NTSC-U region and video mode for PS2/PS1 titles

On all-region Deckard[^3] units, FAT[^5]/70k[^7] units from regions North America or Asia additionally

- Boot burned PS1 backups, original PS1 and PS2 disks from all-region directly from OSD

### FAQ

#### How to update from MechaPwn 2.0 to MechaPwn 3.0?

Do not use the *Restore Backup* option with MechaPwn 2.0 until you are sure that the NVRAM backup is not corrupted.
MechaPwn 3.0 cannot fix your model name, so you need to restore it with MechaPwn 2.0. With MechaPwn 2.0 install *CEX* option for your model.  With MechaPwn 3.0 install whatever you want. If you want to get rid of *Force Unlock*, choose *Restore Factory _Defaults_ in the Patches menu.

#### I have a message "Unknown ***, please report"?

That means that your console setup was untested. Please provide an NVRAM backup and sticker photo for further research.

#### Why does my screen get noisy and blurry?

Your TV doesn't support NTSC or PAL signal. MechaPwn can change the default video mode, for example, DEX on Deckard[^3] units will force NTSC video mode. Choose the CEX option or use different TV.

#### Why do PAL/NTSC-J consoles not play NTSC/PAL discs (on FAT[^5] and 70k[^7])?

The IOP ROM on those consoles has enforced strict logo decryption checks both in the PS1 and PS2 BOOTROM that the NTSC-U (also used in ASIA consoles) BOOTROM does not have.

#### PS1 game has issues, stretched screen, abnormal behavior, etc.

If PS1 games are acting incorrectly, we advise using [DKWDRV](https://github.com/wisi-w/DKWDRV) which has many fixes for the PS1 mode.

#### How to play PS1 games from the different regions on PAL/NTSC-J FAT[^5] and 70k[^7] (original and backups)?

You can do this only via the swap trick. For example, you can burn *ImportPlayer _Lite 2.1_ for either PAL or Japan (according to your console's original region) and follow the instructions for disk swap in the program. Keep in mind that game compatibility will not apply and video mode should be set with *ImportPlayer _Lite 2.1_.

#### Why do consoles not run masterdiscs directly from the OSD?

Additional protection exists on the DSP, which is different in retail and debug consoles, the debug one allows mechacon to store the masterdisc XOR key in its registers, but the retail one does not.
Bypassing the logo check bypasses this protection (this can be done using [wlaunchELF](https://github.com/ps2homebrew/wLaunchELF) or a future cdvdman patch using a ps2 homebrew)

#### My PS2 has a modchip. Will it work for me?

We don't know. This depends on what modchip you are using. It has been reported that some revisions of the modbo 4.0 chip do not truly disable when "disabled" causing conflicts/compatibility issues with MechaPwn region changes, whereas we have seen it work with a modbo 5.0 with disabling that does work as intended (with all modchip features still working when enabled on the converted console).

#### What is Force Unlock?

Force-Unlock is a feature for homebrew applications, like Neo Geo emulation, PS2 Linux for reading CD and DVD-ROM discs.
What it's meant to do is unlock the disk drive for access.

> :warning: Important note: If you enable Force Unlock every CD and DVD will be detected as a PS2 disc, meaning it breaks PS1 games and DVD-Video**

#### How to create a master patched disk?

Master patch creation isnt covered by this FAQ. Try to search for *PSX/PS2 Disc Patcher v3.0* or official SONY utilities.

#### What are the advantages of use on PSX[^6] DVR units?

PSX DVR units mostly have the same limitations as Japan FAT[^5] consoles, but the DEX option in MechaPwn will allow PSX DVR to play PS2 disks from any region directly from XMB. PS1 titles still require the Japanese sector on the disk. Note, that you need a functional burning laser even if you do not plan to record disks to play PS2/PS1 titles.

## TODO / Known bugs and limitations / Planned work

70k[^7] consoles with mechacon 6.0 and 6.2 will break DVD Player support (so exploits like FreeDVDPlayer will stop working). 70k[^7] consoles with mechacon 6.4 are not affected by this behavior.
FMCB will bootloop with Force Unlock and PS2DVD.
Check NVRAM lens area, for better laser calibration.
Collect all possible Model IDs + sticker photo.
Develop an app for restoring nvram based on sticker photos.
Improve Force Unlock or implement other patch exploits.

## How does it work

More info about mechacon flags [here](https://playstationdev.wiki/ps2devwiki/index.php/MechaCon#Region_code). Currently, Mechacon Region Code is set to 0x00130000 (bit 16, 17, 20) + region bit that matches the console or selected settings.
Patch area, that allows to play with mechacon, described [here](https://playstationdev.wiki/ps2devwiki/index.php/MechaCon#Rom_patch). This area contains sets of encrypted assembler instructions that mechacon applies to itself on console boot.
Region parameters, that are changed on slims, are described [here](https://playstationdev.wiki/ps2devwiki/index.php/MechaCon#Region_params).

## Credits

[Contributors](CONTRIBUTORS.md)

### Playstation 2 models description

[^1]: Pre-Dragon: SCPH-1x000 - SCPH-3900x models.
[^2]: Dragon: SCPH-5xxxx - SCPH-9xxxx + PSX DESR + Sony Bravia KDL-22PX300 tvcombo. Dragon subdivided into:
[^3]: Deckard: SCPH-750xx - SCPH-900xx models + Sony Bravia KDL-22PX300 tvcombo
[^4]: Pre-Deckard: All the above:
[^5]: FAT: SCPH-5xxxx models
[^6]: DESR: PSX DVR recorders, Japan only, DESR-5xxx and DESR-7xxx.
[^7]: 70k: SCPH-700xx models
