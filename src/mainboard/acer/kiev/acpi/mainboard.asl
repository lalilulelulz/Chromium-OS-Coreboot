/*
 * This file is part of the coreboot project.
 *
 * Copyright (C) 2011 Google Inc.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; version 2 of
 * the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston,
 * MA 02110-1301 USA
 */

Scope (\_SB) {
	Device (PWRB)
	{
		Name(_HID, EisaId("PNP0C0C"))

		// LPC_PME# connected to GPIO13
		Name(_PRW, Package(){ 29, 5 })
	}
}

/*
 * USB 3 card attached to PCI Express root port 5
 */
Scope (\_SB.PCI0.RP05) {
	Device (XHCI) {
		Name (_ADR, 0x00000000)

		/* USB3 GPIO_WAKE# connected to GPIO8 */
		Name (_PRW, Package(){ 24, 3 })

		/* Leave USB ports on to wake from S3 */
		Method (_S3D, 0)
		{
			Return (2)
		}

		/* Leave USB ports on to wake from S4 */
		Method (_S4D, 0)
		{
			Return (2)
		}

		/* Leave USB ports on to wake from S4 */
		Device (XHCH)
		{
			Name (_ADR, 0x00000000)
			Device (PRT1) { Name (_ADR, 1) } /* USB3 Port 1 */
			Device (PRT2) { Name (_ADR, 2) } /* USB3 Port 2 */
		}
	}
}
