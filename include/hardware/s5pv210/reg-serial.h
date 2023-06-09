#ifndef __S5PV210_REG_SERIAL_H__
#define __S5PV210_REG_SERIAL_H__

/* uart 0 */
#define S5PV210_ULCON0	 	 	(0xE2900000)
#define S5PV210_UCON0	 	 	(0xE2900004)
#define S5PV210_UFCON0	 	 	(0xE2900008)
#define S5PV210_UMON0	 	 	(0xE290000C)
#define S5PV210_UTRSTAT0		(0xE2900010)
#define S5PV210_UERSTAT0		(0xE2900014)
#define S5PV210_UFSTAT0			(0xE2900018)
#define S5PV210_UMSTAT0			(0xE290001C)
#define S5PV210_UTXH0			(0xE2900020)
#define S5PV210_URXH0			(0xE2900024)
#define S5PV210_UBRDIV0			(0xE2900028)
#define S5PV210_UDIVSLOT0		(0xE290002C)
#define S5PV210_UINTP0			(0xE2900030)
#define S5PV210_UINTSP0			(0xE2900034)
#define S5PV210_UINTM0			(0xE2900038)


/* uart 1 */
#define S5PV210_ULCON1	 	 	(0xE2900400)
#define S5PV210_UCON1	 	 	(0xE2900404)
#define S5PV210_UFCON1	 	 	(0xE2900408)
#define S5PV210_UMON1	 	 	(0xE290040C)
#define S5PV210_UTRSTAT1		(0xE2900410)
#define S5PV210_UERSTAT1		(0xE2900414)
#define S5PV210_UFSTAT1			(0xE2900418)
#define S5PV210_UMSTAT1			(0xE290041C)
#define S5PV210_UTXH1			(0xE2900420)
#define S5PV210_URXH1			(0xE2900424)
#define S5PV210_UBRDIV1			(0xE2900428)
#define S5PV210_UDIVSLOT1		(0xE290042C)
#define S5PV210_UINTP1			(0xE2900430)
#define S5PV210_UINTSP1			(0xE2900434)
#define S5PV210_UINTM1			(0xE2900438)


/* uart 2 */
#define S5PV210_ULCON2	 	 	(0xE2900800)
#define S5PV210_UCON2	 	 	(0xE2900804)
#define S5PV210_UFCON2	 	 	(0xE2900808)
#define S5PV210_UTRSTAT2		(0xE2900810)
#define S5PV210_UERSTAT2		(0xE2900814)
#define S5PV210_UFSTAT2			(0xE2900818)
#define S5PV210_UTXH2			(0xE2900820)
#define S5PV210_URXH2			(0xE2900824)
#define S5PV210_UBRDIV2			(0xE2900828)
#define S5PV210_UDIVSLOT2		(0xE290082C)
#define S5PV210_UINTP2			(0xE2900830)
#define S5PV210_UINTSP2			(0xE2900834)
#define S5PV210_UINTM2			(0xE2900838)


/* uart 3 */
#define S5PV210_ULCON3	 	 	(0xE2900C00)
#define S5PV210_UCON3	 	 	(0xE2900C04)
#define S5PV210_UFCON3	 	 	(0xE2900C08)
#define S5PV210_UTRSTAT3		(0xE2900C10)
#define S5PV210_UERSTAT3		(0xE2900C14)
#define S5PV210_UFSTAT3			(0xE2900C18)
#define S5PV210_UTXH3			(0xE2900C20)
#define S5PV210_URXH3			(0xE2900C24)
#define S5PV210_UBRDIV3			(0xE2900C28)
#define S5PV210_UDIVSLOT3		(0xE2900C2C)
#define S5PV210_UINTP3			(0xE2900C30)
#define S5PV210_UINTSP3			(0xE2900C34)
#define S5PV210_UINTM3			(0xE2900C38)


#define S5PV210_UFSTAT_TXFULL	(1<<24)
#define S5PV210_UFSTAT_RXFULL	(1<<8)
#define S5PV210_UFSTAT_TXCOUNT	(0xFF<<16)
#define S5PV210_UFSTAT_RXCOUNT	(0xFF<<0)

#define S5PV210_UTRSTAT_TXE	  	(1<<2)
#define S5PV210_UTRSTAT_TXFE	(1<<1)
#define S5PV210_UTRSTAT_RXDR	(1<<0)

#define S5PV210_UERSTAT_OVERRUN	(1<<0)
#define S5PV210_UERSTAT_PARITY	(1<<1)
#define S5PV210_UERSTAT_FRAME	(1<<2)
#define S5PV210_UERSTAT_BREAK	(1<<3)

#define S5PV210_UMSTAT_CTS	  	(1<<0)
#define S5PV210_UMSTAT_DCTS		(1<<4)


#endif /* __S5PV210_REG_SERIAL_H__ */
