/*
    tosh-superio.h:
    Definition of Machine-Specific Settings for the Superio
    Thomas Pinz <tom_p (at) gmx.de>, 2004
*/

#ifndef __tosh_superio_h
#define __tosh_superio_h

/*
-----------------------------------------------------------------------
Set your Machine-Type here:

Supported Types:
TOSH_SAT5200 - Satellite 5200
TOSH_SAT5100 - Satellite 5100
TOSH_SP2100 - Satellite Pro 2100
TOSH_SPM10 - Satellite Pro M10
TOSH_SATP10 - Satellite P10
TOSH_SATP20 - Satellite P20
TOSH_SATA30 - Satellite A30
*/

/* uncomment only one of the following lines */

#define TOSH_SAT5200
// #define TOSH_SAT5100
// #define TOSH_SP2100
// #define TOSH_SPM10
// #define TOSH_SATP10
// #define TOSH_SATP20
// #define TOSH_SATA30

/* ---------------------------------------------------------------------*/

/* Settings for Satellite 5200 */
#ifdef TOSH_SAT5200
#define MACHINE "Satellite 5200"
#define SIR_IO 0x3f8
#define FIR_IO 0x130
#define FIR_IRQ 3
#define FIR_DMA 3
#endif

/* Settings for Satellite 5100 */
#ifdef TOSH_SAT5100
#define MACHINE "Satellite 5100"
#define SIR_IO 0x3f8
#define FIR_IO 0x130
#define FIR_IRQ 3
#define FIR_DMA 3
#endif

/* Settings for Satellite Pro 2100 */
#ifdef TOSH_SP2100
#define MACHINE "Satellite Pro 2100"
#define SIR_IO 0x3f8
#define FIR_IO 0x130
#define FIR_IRQ 4
#define FIR_DMA 3
#endif

/* Settings for Satellite M10 */
#ifdef TOSH_SPM10
#define MACHINE "Satellite Pro M10"
#define SIR_IO 0x2f8
#define FIR_IO 0x130
#define FIR_IRQ 3
#define FIR_DMA 1
#endif

/* Settings for Satellite P10 */
#ifdef TOSH_SATP10
#define MACHINE "Satellite P10"
#define SIR_IO 0x3f8
#define FIR_IO 0x6f8
#define FIR_IRQ 3
#define FIR_DMA 3
#endif

/* Settings for Satellite P20 */
#ifdef TOSH_SATP20
#define MACHINE "Satellite P20"
#define SIR_IO 0x3f8
#define FIR_IO 0x6f8
#define FIR_IRQ 3
#define FIR_DMA 3
#endif

/* Settings for Satellite A30 */
#ifdef TOSH_SATA30
#define MACHINE "Satellite A30"
#define SIR_IO 0x3f8
#define FIR_IO 0x6f8
#define FIR_IRQ 3
#define FIR_DMA 3
#endif

#endif

