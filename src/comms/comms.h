/* 
 * File:   comms.h
 * Author: Madison Treat <madison.treat@tamu.edu>
 *
 * Created on July 23, 2015, 12:46 PM
 */

#ifndef COMMS_H
#define COMMS_H


/*
 * The CommType enum allows specifying concisely which communications frequency
 * is being referred to, and whether its the active or standby frequency.
 * The STANDBY and ACTIVE parts can be bitwise-OR'ed with a particular frequency,
 * then retrieved by bitwise-AND.
 * Example:
 * 
 * COM1 | ACTIVE   <-- active com1
 * COM1 | STANDBY  <-- standby com1
 * 
 * CommType ct = COM2 | STANDBY;
 * if (ct & COM2) {
 *    // This is a com2 frequency (could be active or standby)
 * }
 * 
 * // Using the above ct value, this IF block will not be entered
 * if (ct & COM1) {
 * }
 * 
 * // If you want to know whether a value is STANDBY, test like this:
 * if (ct & STANDBY) {
 *    // ct above is a STANDBY frequency, so this block will be entered
 * }
 *
 * // You can also test like this:
 * if (ct == (COM1 | STANDBY)) {
 *    // ct above is the COM1 STANDBY frequency, so this block will be entered.
 * }
 * 
 */
enum CommType {
  STANDBY = 0,
  ACTIVE = 1,
  COM1 = 1 << 1, // 2
  COM2 = 1 << 2, // 4
  NAV1 = 1 << 3, // 8
  NAV2 = 1 << 4, // 16

  // Shortcuts
  COM1_ACTIVE  = COM1 | ACTIVE,
  COM1_STANDBY = COM1 | STANDBY,
  COM2_ACTIVE  = COM2 | ACTIVE,
  COM2_STANDBY = COM2 | STANDBY,
  NAV1_ACTIVE  = NAV1 | ACTIVE,
  NAV1_STANDBY = NAV1 | STANDBY,
  NAV2_ACTIVE  = NAV2 | ACTIVE,
  NAV2_STANDBY = NAV2 | STANDBY
};

#endif	/* COMMS_H */

