/*
 * File:   xplanedata.h
 * Author: Madison Treat <madison.treat@tamu.edu>
 *
 * Created on July 27, 2015, 01:30 PM
 *
 * These strings are defined for the XPlane 10.40+ Dataref requests via the RREF
 * UDP request.
 */

#ifndef XPLANEDREF_H
#define XPLANEDREF_H

//const char* XPDR_AC_TYPE = "";
 const int NUM_AC_TAIL_NUMS = 40;
static const char* XPDR_AC_TAIL_NUM_X     = "sim/aircraft/view/acf_tailnum[__X__]";
static const char* XPDR_AC_TAIL_NUM_1     = "sim/aircraft/view/acf_tailnum[1]";
static const char* XPDR_AC_ENGINE_TYPE    = "sim/aircraft/prop/acf_en_type";
static const char* XPDR_AC_NUM_ENGINES    = "sim/aircraft/engine/acf_num_engines";

// Radios
static const char* XPDR_RADIO_COM1_FREQ   = "sim/cockpit/radios/com1_freq_hz";
static const char* XPDR_RADIO_COM1_STDBY  = "sim/cockpit/radios/com1_stdby_freq_hz";
static const char* XPDR_RADIO_COM2_FREQ   = "sim/cockpit/radios/com2_freq_hz";
static const char* XPDR_RADIO_COM2_STDBY  = "sim/cockpit/radios/com2_stdby_freq_hz";

static const char* XPDR_RADIO_NAV1_FREQ   = "sim/cockpit/radios/nav1_freq_hz";
static const char* XPDR_RADIO_NAV1_STDBY  = "sim/cockpit/radios/nav1_stdby_freq_hz";
static const char* XPDR_RADIO_NAV2_FREQ   = "sim/cockpit/radios/nav2_freq_hz";
static const char* XPDR_RADIO_NAV2_STDBY  = "sim/cockpit/radios/nav2_stdby_freq_hz";

const int NUM_FUEL_TANKS = 9;
static const char* XPDR_AC_FUEL_QTY_X     = "sim/cockpit2/fuel/fuel_quantity[__X__]";

/*
static const char* XPDR_AC_ = "sim/";
static const char* XPDR_AC_ = "sim/";
static const char* XPDR_AC_ = "sim/";
static const char* XPDR_AC_ = "sim/";
static const char* XPDR_AC_ = "sim/";
static const char* XPDR_AC_ = "sim/";
static const char* XPDR_AC_ = "sim/";
static const char* XPDR_AC_ = "sim/";
// */

#endif
