#ifndef __MAINBOARD_ONBOARD_H
#define __MAINBOARD_ONBOARD_H

#ifndef __ACPI__
void lan_init(void);
#endif

/* defines for programming the MAC address */
#define ZAKO_NIC_VENDOR_ID		0x10EC
#define ZAKO_NIC_DEVICE_ID		0x8168

/* 0x00: White LINK LED and Amber ACTIVE LED */
#define ZAKO_NIC_LED_MODE		0x00

/* NIC wake is GPIO 8 */
#define ZAKO_NIC_WAKE_GPIO		8

/* WLAN wake is GPIO 10 */
#define ZAKO_WLAN_WAKE_GPIO		10

#endif
