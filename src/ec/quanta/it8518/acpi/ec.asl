/*
 * This file is part of the coreboot project.
 *
 * Copyright (C) 2011-2012 The Chromium OS Authors. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
 */

/*
 * The mainboard must define a PNOT method to handle power
 * state notifications and Notify CPU device objects to
 * re-evaluate their _PPC and _CST tables.
 */

Device (EC0)
{
	Name (_HID, EISAID ("PNP0C09"))
	Name (_UID, 1)
	Name (_GPE, EC_SCI)	// GPE for Runtime SCI

	// EC RAM fields
	OperationRegion(ERAM, EmbeddedControl, 0, 0xFF)
	Field (ERAM, ByteAcc, NoLock, Preserve)
	{
		Offset(0x00),   // [Configuration Space 0]
		DSBY, 1,        //   Disable Bay Monitor even if the bay table is loaded
		ENGA, 1,        //   Enable Global attention
		ENHY, 1,        //   Enable Hotkey function
		HFNE, 1,        //   Enable Sticky Fn Key
		DSEM, 1,        //   Disable embedded NumPad Emulation
		EN3R, 1,        //   Enable 3rd Host interface and TWR registers
		REBY, 1,        //   reserved / Disable Additional Bay Monitor even if the bay device table is loaded
		ENTM, 1,        //   Enable Thermal monitoring

		Offset(0x01),   // [Configuration Space 1]
		ENBK, 1,        //   Enable Panel backlight on/ off synchronized with LID state
		ENFP, 1,        //   Enable to turn off PortFino at suspend and at power off automatically-
		                //	(if this is 1, PortFino is turned off at suspend andturned on at resume when it is on before suspend)
		    , 1,        //   reserved
		IDMI, 1,        //   ignore DIM exit trigger key
		WPSW, 1,        //   warning if the power switch is depressed
		BYON, 1,        //   Bay Power On request
		ENBT, 1,        //   Enable to toggle the BlueTooth and BDC power state every when the button is pressed.
		NTKY, 1,        //   do not ignore any key while Fn key is held down

		Offset(0x02),   // [Configuration Space 2]
		DKON, 1,        //   DOCK_USB_ON
		DSSK, 1,        //   Slicer Speaker Disable.(Tokyo) 1:Disable/0:Enable
		MTES, 1,        //   Spk_Mute signal polarity.(Tokyo) 1:Hight/0:Low
		USBO, 1,        //   Turn on USB power in S3, S4 and S5 state while the AC adapter is attached
		DSMC, 1,        //   Disable Mic (write 1 only, cleared when the system is reset (LPC RESET))
		SNLC, 1,        //   Smart NumLock Enable. 1:Enable 0:Disable
		NLSF, 1,        //   NumLock Indicator Stay off. 1:Enable 0:Disable
		TNKB, 1,        //   ThinkVantage buttom function bit
		                //	0: Scan code (Enter key)
		                //	1: SMI/SCI attention

		Offset(0x03),   // [Configuration Space 3]
		DSHP, 1,        //   Disable to synchronize headphone state with the speaker mute
		IGPK, 1,        //   Ignore phantom keys
		CHGR, 1,        //   Charge/Stop Charge threshold selector
		                //	0: Battery Charge Threshold / 1: Stop Battery Charge Threshold
		    , 1,        //   reserved for a feature.
		CBAT, 1,        //   Change discharge/charge preference to discharge primary battery first and charge secondary battery first if possible
		ADO0, 1,        //   Audio button behavior mode 0
		ADO1, 1,        //   Audio button behavior mode 1
		    , 1,        //   reserved

		Offset(0x04),   // [Sound Mask 0]
		    , 1,        //   reserved
		CLBA, 1,        //   critical low battery alarm
		LWBA, 1,        //   low battery alarm (YELLOW -> RED)
		SUBE, 1,        //   suspend beep
		PUBE, 1,        //   partial suspend beep
		RSBE, 1,        //   resume beep
		DCBE, 1,        //   DC in/out beep
		PFBE, 1,        //   power off beep

		Offset(0x05),   // [Sound Mask 1]
		HSPA, 1,        //   power off alarm
		NHDD, 1,        //   no HDD alarm
		DEAD, 1,        //   dead
		B440, 1,        //   440 hz beep
		B315, 1,        //   315 hz beep
		T315, 1,        //   two 315 hz beep
		R315, 1,        //   three 315 hz beep
		BYAM, 1,        //   bay alarm

		Offset(0x06),   // [Sound ID (Write only)]
		HSUN, 8,        //   06 : Sound ID (Write Only)

		Offset(0x07),   // [Sound Repeat Interval (unit time 125ms)]
		HSRP, 8,        //   07 : Sount Repeat Interval (Unit time : 125ms )

		Offset(0x08),   // [Sound Mask 2]
		    , 1,        //
		DELY, 1,        // PC beep needs 600ms delay
		    , 6,        //

		Offset(0x09),   // [KBD ID]
		Offset(0x0A),   // [KMC command shadow]

		Offset(0x0B),   // [auxiliary device control]
		TPSE, 2,        //   01:auto detection
		                //   02:disable track point
		                //   03:enable track point

		Offset(0x0C),   // [LED On/Off/ Blinking Control (Write only)]
		HLCL, 4,        //   0: power LED
		                //   1: battery status 0
		                //   2: battery status 1
		                //   3: Additional Bay LED (Venice) / reserved (Toronto-4) / Slicer LED (Tokyo)
		                //   4: Bay LED
		                //   5: N/A / 2nd Battery LED (Tokyo)
		                //   6: N/A
		                //   7: Suspend LED
		                //   8: Dock LED 1
		                //   9: Dock LED 2
		                //   10: reserved
		                //   11: Security LED
		                //   12: ACDC LED
		                //   13: Think Vantage LED
		    , 2,        //   reserved
		BLIK, 1,        //   Blinking
		TONF, 1,        //   Turn on/off

		Offset(0x0D),   // [Peripheral control 3]
		UONE, 1,        //   Bit0   - Enable to supply power through USB in S3, S4 and S5 state.
		    , 1,        //   Bit1   - Enable to supply power through USB in S3, S4 and S5 state even when the AC adapter
		                //            is not attached. Otherwise power is supplied only while the AC adapter is attached.
		                //            This bit is valid only while bit 0 of this byte is 1.
		UONM, 2,        //   Bit3-4 - specify D+ and D- state of a USB port or enable a device to set them according to the
		                //            device attached while power is supplied through USB in S3, S4 and S5 state. This bit
		                //            is valid only while bit 0 of this byte is 1
		ECBK, 4,        //   Switch EC bank

		Offset(0x0E),   // [Configuration Space 2]
		HFNS, 2,        //   Bit[1, 0] : Fn Key Status
		                //      [0, 0] ... Unlock
		                //      [0, 1] ... Sticky
		                //      [1, 0] ... Lock
		                //      [1, 1] ... Reserved
		GSER, 1,        //   G-Sensor Diag Error. 1:Error / 0: No error
		PSCS, 1,        //   Peak Shift Charge Status. 1:Inhibt Charge/0:Allow Charge
		PSDS, 1,        //   Peak Shift Discharge Status. 1:Start Discharge/0:Stop Discharge
		GSUD, 1,        //   G-Sensor under Diag. 1: Diag is running/0: Diag comp(not running)
		GSID, 2,        //   Bit[7,6] G-Sensor type ID
		                //      [0,0] ... None
		                //	[0,1] ... STMicro
		                //	[1,0] ... Memsic
		                //	[1,1] ... ADI

		Offset(0x0F),   // [Peripheral status 5 (read only)]
		MBCG, 1,        //   Main battery charge inhibit. 0:allow / inhibit
		SBCG, 1,        //   Secondary battery charge inhibit. 0:allow / inhibit
		MBRF, 1,        //   Main battery refresh status 0:None / 1:Refresh
		SBRF, 1,        //   Secondary battery refresh status 0:None / 1:Refresh
		HDSU, 1,        //   HDD was detached in suspend (this bit is cleared when the system enters into suspend)
		BYSU, 1,        //   Bay HDD was detached in suspend (this bit is cleared when the system enters into suspend)
		    , 1,        //   reserved
		TMOD, 1,        //   Tablet mode switch

		Offset(0x10),   // [Attention Mask (00-127)]
		HAM0, 8,        // 10 : Attention Mask (00-07)
		HAM1, 8,        // 11 : Attention Mask (08-0F)
		HAM2, 8,        // 12 : Attention Mask (10-17)
		HAM3, 8,        // 13 : Attention Mask (18-1F)
		HAM4, 8,        // 14 : Attention Mask (20-27)
		HAM5, 8,        // 15 : Attention Mask (28-2F)
		HAM6, 8,        // 16 : Attention Mask (30-37)
		HAM7, 8,        // 17 : Attention Mask (38-3F)
		HAM8, 8,        // 18 : Attention Mask (40-47)
		HAM9, 8,        // 19 : Attention Mask (48-4F)
		HAMA, 8,        // 1A : Attention Mask (50-57)
		HAMB, 8,        // 1B : Attention Mask (58-5F)
		HAMC, 8,        // 1C : Attention Mask (60-67)
		HAMD, 8,        // 1D : Attention Mask (68-6F)
		HAME, 8,        // 1E : Attention Mask (70-77)
		HAMF, 8,        // 1F : Attention Mask (78-7F)

		Offset(0x20),   // [Thermal Monitor Status]
		HT00, 1,        //   bit 0 CPU level - low
		HT01, 1,        //   bit 1 CPU level - middle
		HT02, 1,        //   bit 2 CPU level - middle-high
		HT03, 1,        //   bit 3 CPU level - High
		HT10, 1,        //   bit 4 Total level (except CPU) - low
		HT11, 1,        //   bit 5 Total level - middle
		HT12, 1,        //   bit 6 Total level - middle-high
		HT13, 1,        //   bit 7 Total level - high

		Offset(0x21),   // [Inhibit Charge timer byte High (00h)] (timer is kicked when high byte is written)

		Offset(0x23),   // [Extended Command Byte (Write only)]
		EXCM, 8,        //   01H Reset Battery Charge Inhibit.
		                //   02H Peakshift Inhibit Charge
		                //   03H Stop "Peakshift discharge" and "Battery Refresh".
		                //   04H Peakshift Discharge
		                //   05H Inhibit charge of a Primary Battery
		                //   06H Inhibit charge of a secondary Battery.- Note: command 02h/05h/06h are exclusive.
		                //   07H Battery Refresh of primary battery
		                //	(Same as "Peakshift Discharge" except that detaching AC, does not break the condition)
		                //   08H Battery Refresh of Secondary battery (ditto.) - Note: command 04h/07h/08h are exclusive.
		                //   10H Start G-Sensor digag.

		Offset(0x24),   // [Battery Charge / Stop Charge Threshold Setting for Primary Battery] (Read/Write)
		Offset(0x25),   // [Battery Charge / Stop Charge Threshold Setting for Secondary Battery] (Read/Write)

		Offset(0x26),   // [Reserved]
		USP0, 1,        //   bit 0 USB0
		USP3, 1,        //   bit 1 USB3
		USP4, 1,        //   bit 2 USB4
		EHP0, 1,        //   bit 3 EHCI0
		EHP1, 1,        //   bit 4 EHCI1
		    , 3,        //   reserved

		Offset(0x27),   // [Passward Scan Code]

		Offset(0x28),   // [Device ID]
		ID00, 1,        //   bit 0 ID0
		ID01, 1,        //   bit 1 ID1
		ID02, 1,        //   bit 2 ID2
		ID03, 1,        //   bit 3 ID3
		    , 2,        //   reserved
		SEBT, 1,        //   Select bit reserved (Toronto4)
		                //	0:Bay 1:Additional Bay (Venice1/2)
		                //	0:Dock CS Bay 1:Dock ID (Tokyo)
		CMDS, 1,        //   Cmd/Status bit Write this bit and 0 cmd complete

		Offset(0x29),   // [DIM control byte]

		Offset(0x2A),   // [Attention Request]
		HATR, 8,        // 2A : Attention request
		HT0H, 8,        // 2B : MSB of Trip Point Capacity for Battery 0
		HT0L, 8,        // 2C : LSB of Trip Point Capacity for Battery 0
		HT1H, 8,        // 2D : MSB of Trip Point Capacity for Battery 1
		HT1L, 8,        // 2E : LSB of Trip Point Capacity for Battery 1

		Offset(0x2F),   // [Fan Speed Control]
		HFSP, 8,        //  bit 2-0: speed (0: stop, 7:highest speed)
		                //  bit 5-3: reserved (should be 0)
		                //  bit 6: max. speed (Monaco/Toronto/Sonoma)
		                //  bit 7: reserved (should be 0 if controlled by the value of bit 2-0)

		Offset(0x30),   // [Sound volume]
		    , 6,        //	bit 0-5 is used as volume value (0 - 31)
		SMUT, 1,        //	bit 6 is Mute
		    , 1,        //	MSB (bit 7) set to one when new volume is set

		Offset(0x31),   // [Periphral Control 2]
		FANS, 2,        //   bit 0,1 Fan selecter
		                //	 00: Fan 1, 01: Fan 2
		HUWB, 1,        //   UWB on
		ENS4, 1,        //   Enter S4 state
		DSEX, 1,        //   Disable Express Card
		AYID, 1,        //   Always-on Card identifyed
		MMUT, 1,        //   Mic mute (0:Unmute, 1:Mute)
		ODDP, 1,        //   ODD Power On/Off (1:On, 0:Off)

		Offset(0x32),   // [H8 Event Mask 0]
		HWPM, 1,        //  bit 0 - PME : Not used. PME# is connected to GPE directly.
		HWLB, 1,        //  bit 1 - Low Bat
		HWLO, 1,        //  bit 2 - Lid Open
		HWDK, 1,        //  bit 3 - Dock
		HWFN, 1,        //  bit 4 - FN key
		HWBT, 1,        //  bit 5 - Bluetooth
		HWRI, 1,        //  bit 6 - Ring Indicator (UART)
		HWBU, 1,        //  bit 7 - Bay Unlock

		Offset(0x33),   // [H8 Event Mask 1]

		Offset(0x34),   // [Peripheral status 2 (read only)]
		    , 7,        //
		HPLO, 1,        //  bit 7 - Power consumption warning (Geyservill low)

		Offset(0x35),   // [Peripheral status 3 (Read only)]
		Offset(0x36),   // [Copy register of H8 Event Status (0x4E)]
		Offset(0x37),   // [Copy register of H8 Event Status (0x4F)]

		Offset(0x38),   // [Battery 0 status (read only)]
		HB0S, 7,        //   bit 3-0 level
		                //	meaning of level
		                //	F: Unknown
		                //	2-n: battery level
		                //	1: low level
		                //	0: (critical low battery, supend/ hibernation)
		                //   bit 4 error
		                //   bit 5 charge
		                //   bit 6 discharge
		MBTS, 1,        //   bit 7 battery attached

		Offset(0x39),   // [Battery 1 status (read only)]

		Offset(0x3A),   // [Peripheral control 0]
		MUTE, 1,        //   Mute
		I2CS, 1,        //   I2C select ( 0:Dock EEPROM etc, 1:Thermal sensor etc )(Tokyo-2)
		PWRF, 1,        //   Power off
		WANO, 1,        //   H/W Override bit
		                //	(enable to control wireless devices even if the global WAN disable switch is ON)
		DCBD, 1,        //   Bluetooth On
		DCWL, 1,        //   Wireless Lan On
		DCWW, 1,        //   Wireless Wan On
		    , 1,        //   2nd Battery Inhibit (Tokyo)

		Offset(0x3B),   // [Peripheral control 1]
		SPKM, 1,        //   Speaker Mute
		KBLH, 1,        //   Keyboard Light
		    , 1,        //   reserved
		BTDH, 1,        //   Bluetooth detach
		USBN, 1,        //   USB On
		    , 2,		//   reserved
		S3FG, 1,        //   Notify EC enter S3.

		Offset(0x3C),   // [Resume reason (Read only)]
		Offset(0x3D),   // [Password Control byte]
		Offset(0x3E),   // [Password data (8 byte)~ offset:45h]

		Offset(0x41),   // [Password data (8 byte)~ offset:45h]
		    , 7,        //
		PFLG, 1,        //

		Offset(0x46),   // [sense status 0]
		FNKY, 1,        //   Fn key
		    , 1,        //   reserved
		HPLD, 1,        //   LID open
		PROF, 1,        //   Power off
		ACPW, 1,        //   External power (AC status)
		    , 2,        //   reserved
		CALR, 1,        //   Current Alert (Rome/Tokyo-2)

		Offset(0x47),   // [sense status 1]
		HPBU, 1,        //   Bay Unlock
		DKEV, 1,        //   Dock event
		BYNO, 1,        //   Bay is not Attached
		HDIB, 1,        //   HDD in the bay
		    , 4,        //   reserved

		Offset(0x48),   // [sense status 2]
		HPHI, 1,        //   Head Phone In
		GSTS, 1,        //   Global Wan Enable Switch
		    , 2,        //   reserved
		EXGC, 1,        //   External Graphic Chip
		DOKI, 1,        //   Dock attached
		HDDT, 1,        //   HDD detect
		    , 1,        //   reserved

		Offset(0x49),   // [sense status 3]
		    , 1,        //   reserved
		NUMK, 1,        //   NumLock state (0:unlock, 1:lock)
		    , 6,        //   reserved

		Offset(0x4A),   // [DIM Timer]
		Offset(0x4B),   // [LSB of DIM Timer]

		Offset(0x4C),   // [MSB of Event Timer]
		ETHB, 8,        //   bit[14:0]=timer counter, bit[15], 0:ms, 1:sec

		Offset(0x4D),   // [LSB of Event Timer]
		ETLB, 8,        //

		Offset(0x4E),   // [H8 Event Status 0]
		    , 1,        //
		ACOV, 1,        //   Notify EC to send AC Overloading event
		RMCS, 1,        //   Reach the MAX CPU stepings
		    , 1,        //
		T4E4, 1,        //   4 Cell Throttling rule : 1st point -50% throttling setting 58'c
		T4E5, 1,        //   4 Cell Throttling rule : 2nd point -87.5% throttling setting 63'c

		Offset(0x4F),   // [H8 Event Status 1]

		Offset(0x50),   // [SMB_PRTCL (protocol register)]
		SMPR, 8,        //	00: Controller Not In use
		                //	01: reserved
		                //	02: Write Quick command
		                //	03: Read Quick command
		                //	04: Send Quick command
		                //	05: Receive Byte
		                //	06: Write Byte
		                //	07: Read Byte
		                //	08: Write Word
		                //	09: Read Word
		                //	0A: Write block

		Offset(0x51),   // [SMB_STS (status register)]
		SMST, 8,        //	00: SMBus OK
		                //	07: SMBus Unknown Failure
		                //	10: SMBus Device Address Not Acknowldged
		                //	11: SMBus Device Error Detected
		                //	12: SMBus Device Command Access Denied
		                //	13: SMBus Unknown Error
		                //	17: SMBus Device Access Denied
		                //	18: SMBus Time out
		                //	19: SMBus Host Unsupported Protocol
		                //	1A: SMBus Busy

		Offset(0x52),   // [SMB_ADDR (address register)]
		SMAD, 8,        //

		Offset(0x53),   // [SMB_CMD (Command register)]
		SMCM, 8,        //

		Offset(0x54),   // [SMB_DATA (Data Register (32 bytes))~ offset:73h]
		SMD0, 100,      //

		Offset(0x74),   // [SMB_BCNT (Block count register)]
		BCNT, 8,        //

		Offset(0x75),   // [SMB_ALRM_ADDR (Alarm address register)]
		SMAA, 8,        //

		Offset(0x76),   // [SMB_ALRM_DATA (Alarm data register (2 bytes))]
		BATD, 16,       //

		Offset(0x78),   // [Temperature of thermal sensor 0-7 (centigrade)]
		TMP0, 8,        // 78 : Temperature of thermal sensor 0
		TMP1, 8,        // 79 : Temperature of thermal sensor 1
		TMP2, 8,        // 7A : Temperature of thermal sensor 2
		TMP3, 8,        // 7B : Temperature of thermal sensor 3
		TMP4, 8,        // 7C : Temperature of thermal sensor 4
		TMP5, 8,        // 7D : Temperature of thermal sensor 5
		TMP6, 8,        // 7E : Temperature of thermal sensor 6
		TMP7, 8,        // 7F : Temperature of thermal sensor 7

		Offset(0x80),   // [Attention control byte]
		    , 8,        //    to control attention and its queue, write following value
		                //    to this byte while this byte is 0;
		                //    01: Clear attention queue and enable attention if it is disabled temporary
		                //    02: Clear attention queue and disable all attentions temporary
		                //
		                // Note:
		                // This will become 0 when the function is completed.
		                // Understanding attention, it already informed to the system
		                // via SCI and status bit (SCI_EVENT) in status register,
		                // is not cleared (in other word, this is not in the queue)
		                // by above operation. while all attentions are disabled,
		                // bit 3 (attention is disabled temporary) of Peripheral
		                // status 3 (0x35) is one.
		                //

		Offset(0x81),   // [Battery information ID for 0xA0-0xAF]
		HIID, 8,        //   (this byte is depend on the interface, 62&66 and 1600&1604)

		Offset(0x82),   // [Fn Dual function - make time out time (100ms unit)]
		    , 8,        //

		Offset(0x83),   // [Fn Dual function ID]
		HFNI, 8,        //	0: none
		                //	1: Left Windows key
		                //	2: Right Windows key
		                //	3: Application key
		                //	4: ACPI Power
		                //	5: ACPI Sleep
		                //	6: ACPI Wake
		                //  7: Reserved

		Offset(0x84),   // [Fan Speed]
		    , 16,       //
		                //	(I/F Offset 3Bh bit5 => 0:Main Fan , 1:Second Fan)

		Offset(0x86),   // [password 0 - 7 status]
		Offset(0x87),   // [password 8 - 15 status]

		Offset(0x88),   // [Dock EEPROM Access Control Byte]
		SCRS, 1,        // Scramble mode Fan control status
		    , 2,
		CPUT, 1,        // CPU Turbo control handle
		    , 3,        // 88 : Dock EEPROM Access Control Byte
		DGTR, 1,        // dGPU throttling / release

		Offset(0x89),   // [offset in Dock EEPROM to write/read]
		HDEO, 8,        // 89 : offset in Dock EEPROM to write/read

		Offset(0x8A),   // [max. sound volume]

		Offset(0x8B),   //
		LOMD, 1,        //   Control LAN isoltae
		CBDE, 1,        //   CABLE Detect
		    , 6,        //   reserved

		Offset(0x8C),   // [Dothan themal workaround]
		NVPR, 1,        // 1:DGPU_PWR Enable 0:DGPU_PWR Disable
		TLLE, 1,        // Think Light LED 1:Enable 0:Disable
		    , 6,        // Reserved

		Offset(0x8D),   // [Interval of polling Always-on cards in minutes (0  127) (effective only in S3, andS4)]
		HDAA, 3,        //   Warning Delay Period
		HDAB, 3,        //   Stolen Delay Period
		HDAC, 2,        //   Sensitivity

		Offset(0x8E),   // [Key-number, whose scan code is sent to host at Nozomi button press.]
		Offset(0x8F),   // [H8 Internal Use for Fan Duty Table Creation]

		Offset(0x90),   // [Memry Access Command]
		ERMC, 8,        //  (0x90) Command
		    , 8,        //  (0x91) Reserved (Should be 0)
		AMSB, 8,        //  (0x92) Address of RAM, ROM or I/O port number
		ALSB, 8,        //  (0x93)
		DMSB, 8,        //  (0x94)
		DLSB, 8,        //  (0x95)

		Offset(0xA0),   // [Battery Information Area]~ offset:0AFh
		BAIF, 128,

		Offset(0xB0),   // [data to be written to EEPROM/ read data from EEPROM]~ offset: 0BFh
		HDEN, 32,       // B0 - B3 : Dock Serial number
		DBTS, 8,        // B4 Set Dual Mode Battery Setting
		    , 24,       // B5 - B7
		HDEM, 8,        // B8      : Dock Machine ID
		HDES, 8,        // B9      : Dock EEPROM checksum
		                // BA - BF : Dock EEPROM reserved
		                // C0-C7 : reserved

		Offset(0xC0),   // [Temperature of thermal sensor 8-15 (centigrade)~ offset:C7h]

		Offset(0xC1),   // @@ Mapping to old EC RAM for battery information
		MCUR, 16,

		Offset(0xC3),   // @@ Mapping to old EC RAM for battery information
		MBRM, 16,

		Offset(0xC5),   // @@ Mapping to old EC RAM for battery information
		MBVG, 16,

		Offset(0xC8),   // [Adaptive Thermal Management]
		ATMX, 8,        //  bit 7-4 - Thermal Table & bit 3-0 - Fan Speed Table

		Offset(0xC9),   // [Wattage of AC/DC]
		AC65, 8,        //

		Offset(0xCA),   // [Periodical timer value [100ms] (MW2-note)]

		Offset(0xCB),   // [Battery]
		BFUD, 1,        //   bit 0 - Battery FUD Flag
		    , 7,        //   bit 1~7 - Reserved

		Offset(0xCC),   //
		PWMH, 8,        // CC : AC Power Consumption (MSB)
		PWML, 8,        // CD : AC Power Consumption (LSB) - unit: 100mW
		SHSC, 8,        // CE : Show the latest scan code which is pressed
		HSID, 8,        // CF : Hand shaking byte of system Inf ID
		                // [Reserved]~ offset:0E7h
		                // E0-E7 : reserved

		Offset(0xE0),   // @@ Mapping to old EC RAM for battery information
		ECRC, 16,       // BAT1 Sys command [0] RC
		ECAC, 16,       // BAT1 Sys command [0] AV_I
		ECVO, 16,       // BAT1 Sys command [0] Voltage

		Offset(0xE8),   // [Version 0]
		Offset(0xE9),   // [Version 1]
		Offset(0xEA),   // [Machine ID]
		Offset(0xEB),   // [Function Specification Minor Version (05)]

		Offset(0xEC),   // [H8 capability 0]
		                //  bit 0 - Intelligent battery type 1
		                //  bit 1 - Intelligent battery type 2
		                //  bit 2 - Intelligent battery type 3 (Smart battery)
		                //  bit 3 - Reserved
		                //  bit 4 - Dum battery
		                //  bit 5 - Two battery
		                //  bit 6 - Refresh function
		                //  bit 7 - Reserved

		Offset(0xED),   // [H8 capability 1]
		    , 1,        //  bit 0 - this system has No Fan(FAN-less system)
		    , 1,        //  bit 1 - n level (0-0xE) of battery status (0x38, 0x39) is
		                //          supported and next byte (0xEE) has valid highest level
		    , 1,        //  bit 2 - Fn dual function key is supported
		    , 1,        //  bit 3 - Enhanced TWR/Access Bus Interface
		                //          (Version Information of the interface is supported)
		    , 1,        //  bit 4 - Always 0
		    , 1,        //  bit 5 - Adaptive Thermal Management is supportd
		    , 1,        //  bit 6 - Deep Smart Power Down Mode Support
		    , 1,        //  bit 7 - reserved(should be 0)

		Offset(0xEE),   // [highest battery level]
		MBTH, 4,        //   bit 3-0: battery 0 highest level
		SBTH, 4,        //   bit 7-4: battery 1 highest level
		                //   note: if highest level is 0 or 0xF, it means not defined
		                //         (in this case, use default hightest level, it is 6)

		Offset(0xEF),   // [Function Specification Major Version (03)]
		Offset(0xF0),   // [Build ID]~ offset:0F7h

		Offset(0xF8),   // [Build Date (F8: MSB, F9:LSB)]
		    , 4,        //   bit 3-0: day(1-31)
		    , 4,        //   bit 7-4: month(1-12)
		    , 8,        //   bit 15-8: year(2000 base)

		Offset(0xFA),   // [Build Time (in 2seconds)]
		                //   ex: when index FAh=5Ah and index FBh=ADh, Build Time is 5AADh .. 12:53:46

		Offset(0xFC),   // [reserved]~ offset:0FFh
	} // End of ERAM

	//
	// Battery Information ID : 00/10
	//
	Field (ERAM, ByteAcc, NoLock, Preserve)
	{
		Offset(0xA0),
		SBRC, 16,    // Remaining Capacity
		SBFC, 16,    // Fully Charged Capacity
		SBAE, 16,    // Average Time To Empty
		SBRS, 16,    // Relative State Of Charge
		SBAC, 16,    // Average Current
		SBVO, 16,    // Voltage
		SBAF, 16,    // Average Time To Full
		SBBS, 16,    // Battery State
	}

	//
	// Battery Information ID : 01/11
	//
	Field (ERAM, ByteAcc, NoLock, Preserve)
	{
		Offset(0xA0),
		             // Battery Mode(w)
		    , 15,
		SBCM, 1,     //  bit 15 - CAPACITY_MODE
		             //        0: Report in mA/mAh ; 1: Enabled
		SBMD, 16,    // Manufacture Data
		SBCC, 16,    // Cycle Count
	}

	//
	// Battery Information ID : 02/12
	//
	Field (ERAM, ByteAcc, NoLock, Preserve)
	{
		Offset(0xA0),
		SBDC, 16,    // Design Capacity
		SBDV, 16,    // Design Voltage
		SBOM, 16,    // Optional Mfg Function 1
		SBSI, 16,    // Specification Info
		SBDT, 16,    // Manufacture Date
		SBSN, 16,    // Serial Number
	}

	//
	// Battery Information ID : 04/14
	//
	Field (ERAM, ByteAcc, NoLock, Preserve)
	{
		Offset(0xA0),
		SBCH, 32,    // Device Checmistory (string)
	}

	//
	// Battery Information ID : 05/15
	//
	Field (ERAM, ByteAcc, NoLock, Preserve)
	{
		Offset(0xA0),
		SBMN, 128,   // Manufacture Name (s)
	}

	//
	// Battery Information ID : 06/16
	//
	Field (ERAM, ByteAcc, NoLock, Preserve)
	{
		Offset(0xA0),
		SBDN, 128,   // Device Name (s)
	}

	Method (_CRS, 0, NotSerialized)
	{
		Name (ECMD, ResourceTemplate()
		{
			IO (Decode16, 0x62, 0x62, 0, 1)
			IO (Decode16, 0x66, 0x66, 0, 1)
		})
		Return (ECMD)
	}

	Method (_REG, 2, NotSerialized)
	{
		// Initialize AC power state
		Store (ACPW, \PWRS)

		// Initialize LID switch state
		Store (NOT(HPLD), \LIDS)

		// Enable OS control of fan speed
		// TODO Store (One, FCOS)

		// Force a read of CPU temperature
		// TODO Which temperature corresponds to the CPU?
		Store (TMP0, Local0)
	}

/*
 * EC Query Responses
 * Hotkey 1 10h
 * Hotkey 2 11h
 * Sleep Button pressed 13h
 * Display toggle 16h
 * Decrease brightness event 17h
 * Increase brightness event 18h
 * Decrease volume event 1Ch
 * Increase volume event 1Dh
 * Volume Mute 1Eh
 * Critical Low battery	(Trigger to suspend/hibernation) 22h
 * Trip Point Capacity Event for Battery1 24h
 * AC adapter attached 26h
 * AC adapter detached 27h
 * Power off switch pressed 28h
 * Power off switch released (Power off) 29h
 * LID open 2Ah
 * LID close (suspend trigger) 2Bh
 * Update CS09 Dock/Undock Event 37h
 * GlobalSwtEvent (Global switch status changed) 41h
 * Mute state change (Fn + ESC) 43h
 * Battery Attach/Detach 4Ah
 * Battery State Change 4Bh
 * Enable/Disable WLAN, WWAN and bluetooth 64h
 * Enable/Disable Camera (Fn+F6) 65h
 * Hotkey 66h
 * Fn Sticky Make 70h
 * Fn Lock 72h
 * Fn Unlock 73h
 * Update Thermal for OS every 2 sec when CPU temp over 95h
 * 4 Cell Throttling EAh
 * 4 Cell Throttling ECh
 * AC Overloading throttling EBh
 * Release AC Overloading throttling EDh
 * cTDP control F0h
 * Disable CPU Turbo F1h
 * Enable CPU Turbo F2h
 * DGPU Throttling F5h
 * Enable DGPU Turbo F6h
 */

	// Battery at critical low state
	Method (_Q22)
	{
		Notify (BATX, 0x80)
	}

	// AC Power Connected
	Method (_Q26, 0, NotSerialized)
	{
		Store (One, \PWRS)
		Notify (AC, 0x80)
		\PNOT ()
	}

	// AC Power Removed
	Method (_Q27, 0, NotSerialized)
	{
		Store (Zero, \PWRS)
		Notify (AC, 0x80)
		\PNOT ()
	}

	// LID Open
	Method (_Q2A)
	{
		Store (One, \LIDS)
		Notify( \_SB.LID0, 0x80 )
	}

	// LID Close (Suspend Trigger)
	Method (_Q2B)
	{
		Store (Zero, \LIDS)
		Notify( \_SB.LID0, 0x80 )
	}

	// Communication Device Disable/Enable Event
	Method(_Q64, 0, NotSerialized)
	{
		IF (DCWL) //if Wlan exist
		{
			//TODO Store(WLAT, LANE)
		}
	}

	#include "ac.asl"
	#include "battery.asl"
	// TODO #include "fan.asl"
}
