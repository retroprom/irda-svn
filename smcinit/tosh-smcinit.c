/*
 * tosh-smcinit.c
 *
 * modified by Thomas Pinz <tom_p (at) gmx.de>
 *
 * IrDA configurator for laptops with  SMSC 47N227 SuperIO,
 * smc-ircc and not initializing BIOS (tested on Toshiba Satellite 5100
 * and Toshiba Tecra 9100).
 * Copyright (C) 2003, Rob Miller <rob@janerob.com>
 *
 * http://www.janerob.com/rob/ts5100/
 *
 *
 * Cleanups, small fixes by Claudiu Costin <claudiuc@kde.org>
 * Expansions for serveral modes by Thomas Pinz <tom_p@gmx.de>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc., 59 Temple
 * Place, Suite 330, Boston, MA  02111-1307  USA
 */
 
#include <sys/io.h>
#include <sys/types.h>
#include <errno.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pci/pci.h>
#include "config.h"

/* (*) */
#include "tosh-superio.h"

#define PROGNAME "tosh-smcinit"
#define AUTHOR "Thomas Pinz"
#define AUTHOR_EMAIL "<tom_p (at) gmx.net>"

/* lspci on 5100 says */
/* 00:1f.0 ISA bridge: Intel Corp. 82801CAM ISA Bridge (LPC) (rev 02) */
    
#define BUS_LPC 0x00
#define LPC_DEV 0x1f
#define LPC_FUNC 0x00

/* (*)  */
/* Bitshifters */
#define SMC_SIR_IO ( (SIR_IO & 0xFF8) >> 2 )	/* CR25, p158: shift 2 down */
#define	SMC_FIR_IO ( (FIR_IO & 0xFF8) >> 3 )	/* CR2B, p162: shift 3 down */
#define SMC_FIR_IRQ FIR_IRQ	/* CR28, p160: equal */
#define	SMC_FIR_DMA FIR_DMA	/* CR2C, p162: equal */    


/* 82801 registers */
#define VID 0x00
#define DID 0x02
#define PIRQA_ROUT 0x60
#define PCI_DMA_C 0x90
#define COM_DEC 0xe0
#define LPC_EN 0xe6
#define GEN2_DEC 0xec
    
/* SMSC 47N227 registers */ 
#define SMC_BASE 0x2e
    
/* 47N227 UART base offsets 
 *  xxx enable fifo ?
 */
#define RCV base
#define IER base+1
#define IIR base+2
#define FCR base+2
#define LCR base+3
#define MCR base+4
#define LSR base+5
#define MSR base+6
#define SPR base+7
#define DLSB base
#define DMSB base+1

struct option options[] = {
    {"version", no_argument, NULL, 'V'},
    {"help", no_argument, NULL, 'h'},
    {"verbose", no_argument, NULL, 'v'},
    {NULL, 0, NULL, 0}
};

static char *options_explications[] = {
    "\t\tshow program version",
    "\t\tshow this help",
    "\t\tbe verbose, show the compiled-in settings.",
    NULL
};

static char *short_options = "Vhv";


void print_usage(struct option *options_array, char **options_explications_array)
{
    struct option *option;
    char *option_explication = NULL;
    int i;

    printf("%s %s %s\nUsage: %s [options]\nOptions:\n", PROGNAME, VERSION,
           AUTHOR, PROGNAME);
    i = 0;
    option = &options_array[0];
    option_explication = options_explications_array[0];
    while (option->name != NULL) {
        printf("-%c, --%s%s", option->val, option->name,
               (option->has_arg ? "=VALUE" : ""));
        if (options_explications_array[i] != NULL)
            printf("%s\n", options_explications_array[i]);
        else
            puts("\n");
        i++;
        option = &options[i];
        option_explication = options_explications_array[i];
    }
    printf("Report suggestions and bugs to: %s\n", AUTHOR " " AUTHOR_EMAIL);
}

void print_version()
{
    printf("%s %s\n", PROGNAME, VERSION);
}

void print_settings()
{
    print_version();
    printf("Compiled for: %s\n", MACHINE);
    printf("SIR ioport: 0x%x\n", SIR_IO);
    printf("FIR ioport: 0x%x\n", FIR_IO);
    printf("FIR interupt: %d\n", FIR_IRQ);
    printf("FIR DMA: %d\n", FIR_DMA);
}

int main(int argc, char **argv) 
{
    int i = 0, opt;
    
    /* setpci.c */ 
    struct pci_access *acc;
    struct pci_dev *dev;
    word twobyte;

    while ((opt =
             getopt_long(argc, argv, short_options, options,
                         NULL)) != -1) {
        switch (opt) {
        case 'V':
            {
                print_version();
                exit(0);
                break;
            }
        case 'h':
            {
                print_usage(options, options_explications);
                exit(0);
                break;
            }

	      case 'v':
	          {
		            print_settings();
		            break;
	          }
        default:
            break;
        }

    }

    if (getuid() != 0) {
        fprintf(stderr, "%s can only be used by root\n", PROGNAME);
        exit(1);
    }
    acc = pci_alloc();
    pci_init(acc);
    dev = pci_get_dev(acc, BUS_LPC, LPC_DEV, LPC_FUNC); /* 5100 also dev 1f */
    twobyte = pci_read_word(dev, VID);
    if (twobyte != 0x8086) {
        fprintf(stderr, "%s IO hub vendor %x not intel (0x8086)\n",
                 PROGNAME, twobyte);
        return 1;
    }
    twobyte = pci_read_word(dev, DID);
    if ( (twobyte != 0x24cc) & (twobyte != 0x248c) )  {
        fprintf(stderr, "%s IO hub device %x not 82801CAM (0x248c or 0x24cc)\n",
                 PROGNAME, twobyte);
        return 1;
    }
    
    pci_write_byte(dev, COM_DEC, 0x10); /* comb 2f8-2ff coma 3f8-3ff */
    twobyte = pci_read_word(dev, LPC_EN); /* LPC_EN register */
    twobyte &= 0xfffd;          /* wipe bit 1 */
    
    twobyte |= 0x0001;      /* set bit 0 : COMA addr range enable */
    pci_write_word(dev, LPC_EN, twobyte);
    twobyte = pci_read_word(dev, PCI_DMA_C); /* PCI_DMA register */
    twobyte &= 0xfff3;          /* wipe bits 2,3 */
    
    twobyte = 0xc0c0;       /* LPC I/F DMA on, channel 3  -- rtm (?? PCI DMA ?) */
    pci_write_word(dev, PCI_DMA_C, twobyte);
    pci_write_word(dev, GEN2_DEC, 0x131); /* LPC I/F 2nd decode range */
    
    pci_free_dev(dev);
    pci_cleanup(acc);
    
        /* setsmc.c */ 
    ioperm(SMC_BASE, 2, 1);
    outb(0x55, SMC_BASE);      // enter configuration state
    outb(0x0d, SMC_BASE);       // set for device id
    if (((i = inb(SMC_BASE + 1)) == 0x5a) | ((i = inb(SMC_BASE + 1)) == 0x7a) )
	// if SMC 47N227 or other
    {
	// ! sir-io !
        outb(0x24, SMC_BASE);  // select CR24 - UART1 base addr
        outb(0x00, SMC_BASE + 1); // disable UART1
        outb(0x25, SMC_BASE);  // select CR25 - UART2 base addr
        outb(SMC_SIR_IO, SMC_BASE + 1); // bits 2-9 of 0x3f8

	// ! fir-irq !
        outb(0x28, SMC_BASE);  // select CR28 - UART1,2 IRQ select
        i = inb(SMC_BASE + 1);  // get current setting for both
        outb((i & 0x00) | SMC_FIR_IRQ, SMC_BASE + 1); // low order bits 
        // but want no irq for serial uart1 ?

	// ! fir-io !
        outb(0x2B, SMC_BASE);  // CR2B - SCE (FIR) base addr
        outb(SMC_FIR_IO, SMC_BASE + 1); // io 

	// ! fir-dma !
        outb(0x2C, SMC_BASE);  // CR2C - SCE (FIR) DMA select
        outb(SMC_FIR_DMA, SMC_BASE + 1); // DMA 

        outb(0x0C, SMC_BASE);  // CR0C - UART mode
        i = inb(SMC_BASE + 1);  // whatever already there
        outb((i & 0xC7) | 0x88, SMC_BASE + 1); // enable IrDA (HPSIR) mode, high speed
        outb(0x07, SMC_BASE);  // CR07 - Auto Pwr Mgt/boot drive sel
        i = inb(SMC_BASE + 1);  // whatever already there 
        outb(i | 0x20, SMC_BASE + 1); // enable UART2 autopower down
        outb(0x0a, SMC_BASE);  // CR0a - ecp fifo / ir mux
        i = inb(SMC_BASE + 1);  // whatever already there 
        outb(i | 0x40, SMC_BASE + 1); // send active device to ir port
        outb(0x02, SMC_BASE);  // CR02 - UART 1,2 power
        i = inb(SMC_BASE + 1);  // whatever already there
        outb(0x80, SMC_BASE + 1); // UART2 power up mode, UART1 power down
        outb(0x00, SMC_BASE);  // CR00 - FDC Power/valid config cycle
        i = inb(SMC_BASE + 1);  // whatever already there
        outb(i | 0x80, SMC_BASE + 1); // valid config cycle done
        outb(0xaa, SMC_BASE);  // ?? twiggle config register ??

    } else {
        fprintf(stderr, "%s %x not SMC 47Nxxx (should be 0x5a or 0x7a)\n", PROGNAME, i);
        return 1;
    }
    return 0;
}
