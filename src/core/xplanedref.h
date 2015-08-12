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

// const int MAX_NUM_FUEL_TANKS = 9; // NOT DRY: see engine/engineconsts.h
static const char* XPDR_CP_FUEL_QTY_X     = "sim/cockpit2/fuel/fuel_quantity[__X__]";
static const char* XPDR_CP_FUEL_QTY_0     = "sim/cockpit2/fuel/fuel_quantity[0]";
static const char* XPDR_CP_FUEL_QTY_1     = "sim/cockpit2/fuel/fuel_quantity[1]";
static const char* XPDR_CP_FUEL_QTY_2     = "sim/cockpit2/fuel/fuel_quantity[2]";
static const char* XPDR_CP_FUEL_QTY_3     = "sim/cockpit2/fuel/fuel_quantity[3]";
static const char* XPDR_CP_FUEL_QTY_4     = "sim/cockpit2/fuel/fuel_quantity[4]";
static const char* XPDR_CP_FUEL_QTY_5     = "sim/cockpit2/fuel/fuel_quantity[5]";
static const char* XPDR_CP_FUEL_QTY_6     = "sim/cockpit2/fuel/fuel_quantity[6]";
static const char* XPDR_CP_FUEL_QTY_7     = "sim/cockpit2/fuel/fuel_quantity[7]";
static const char* XPDR_CP_FUEL_QTY_8     = "sim/cockpit2/fuel/fuel_quantity[8]";

// Timer
// static const char* XPDR_CP_TIMER_RUNNING = "sim/cockpit2/clock_timer/timer_running";
static const char* XPDR_CP_TIMER_RUNNING  = "sim/time/timer_is_running_sec";
static const char* XPDR_CP_TIMER_ELAPSED  = "sim/time/timer_elapsed_time_sec";

// Landing Gear
const int NUM_LANDING_GEARS = 10;
static const char* XPDR_GEAR_DEPLOY_X  = "sim/aircraft/parts/acf_gear_deploy[__X__]";
static const char* XPDR_GEAR_DEPLOY_0  = "sim/aircraft/parts/acf_gear_deploy[0]";
static const char* XPDR_GEAR_DEPLOY_1  = "sim/aircraft/parts/acf_gear_deploy[1]";
static const char* XPDR_GEAR_DEPLOY_2  = "sim/aircraft/parts/acf_gear_deploy[2]";
static const char* XPDR_GEAR_DEPLOY_3  = "sim/aircraft/parts/acf_gear_deploy[3]";
static const char* XPDR_GEAR_DEPLOY_4  = "sim/aircraft/parts/acf_gear_deploy[4]";
static const char* XPDR_GEAR_DEPLOY_5  = "sim/aircraft/parts/acf_gear_deploy[5]";
static const char* XPDR_GEAR_DEPLOY_6  = "sim/aircraft/parts/acf_gear_deploy[6]";
static const char* XPDR_GEAR_DEPLOY_7  = "sim/aircraft/parts/acf_gear_deploy[7]";
static const char* XPDR_GEAR_DEPLOY_8  = "sim/aircraft/parts/acf_gear_deploy[8]";
static const char* XPDR_GEAR_DEPLOY_9  = "sim/aircraft/parts/acf_gear_deploy[9]";

/*
static const char* XPDR_ = "sim/";
static const char* XPDR_ = "sim/";
static const char* XPDR_ = "sim/";
static const char* XPDR_ = "sim/";
static const char* XPDR_ = "sim/";
static const char* XPDR_ = "sim/";
// */

#endif
