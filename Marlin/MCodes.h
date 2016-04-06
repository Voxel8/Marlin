/**
 * MCodes.h - Interface for Mcodes
 * Author: Dan Thompson (danthompson41@gmail.com)
 * Date: 4/6/2016
 * Allows access to MCodes. Refactor from keeping them in Marlin_Main.cpp
 * Copyright (C) 2016 Voxel8
 */

#ifndef M_CODES_H_
#define M_CODES_H_

//===========================================================================
//============================= Public Functions ============================
//===========================================================================


void gcode_G000(void); // void gcode_G0_G1();
void gcode_G001(void); // void gcode_G0_G1();
void gcode_G002(bool clockwise); // void gcode_G2_G3(bool clockwise);
void gcode_G003(bool clockwise); // void gcode_G2_G3(bool clockwise);
void gcode_G004(void); // void gcode_G04();
void gcode_G028(void); // void gcode_G28();
void gcode_G092(void); // void gcode_G92();

void gcode_M000(void); // void gcode_M0_M1();
void gcode_M001(void); // void gcode_M0_M1();
void gcode_M017(void); // void gcode_M17();
void gcode_M018(void); // void gcode_M18_M84();
void gcode_M020(void); // void gcode_M20();
void gcode_M021(void); // void gcode_M21();
void gcode_M022(void); // void gcode_M22();
void gcode_M023(void); // void gcode_M23();
void gcode_M024(void); // void gcode_M24();
void gcode_M025(void); // void gcode_M25();
void gcode_M026(void); // void gcode_M26();
void gcode_M027(void); // void gcode_M27();
void gcode_M028(void); // void gcode_M28();
void gcode_M029(void); // void gcode_M29();
void gcode_M030(void); // void gcode_M30();
void gcode_M031(void); // void gcode_M31();
void gcode_M032(void); // void gcode_M32();
void gcode_M033(void); // void gcode_M33();
void gcode_M042(void); // void gcode_M42();
void gcode_M048(void); // void gcode_M48();
void gcode_M080(void); // void gcode_M80();
void gcode_M081(void); // void gcode_M81();
void gcode_M082(void); // void gcode_M82();
void gcode_M083(void); // void gcode_M83();
void gcode_M084(void); // void gcode_M18_M84();
void gcode_M085(void); // void gcode_M85();
void gcode_M092(void); // void gcode_M92();
void gcode_M104(void);
void gcode_M105(void);
void gcode_M106(void);
void gcode_M107(void);
void gcode_M109(void);
void gcode_M111(void);
void gcode_M112(void);
void gcode_M114(void);
void gcode_M115(void);
void gcode_M117(void);
void gcode_M119(void);
void gcode_M120(void);
void gcode_M121(void);
void gcode_M125(void);
void gcode_M126(void);
void gcode_M127(void);
void gcode_M128(void);
void gcode_M129(void);
void gcode_M140(void);
void gcode_M145(void);
void gcode_M150(void);
void gcode_M190(void);
void gcode_M200(void);
void gcode_M201(void);
void gcode_M203(void);
void gcode_M204(void);
void gcode_M205(void);
void gcode_M206(void);
void gcode_M207(void);
void gcode_M208(void);
void gcode_M209(void);
void gcode_M211(void);
void gcode_M218(void);
void gcode_M220(void);
void gcode_M221(void);
void gcode_M226(void);
void gcode_M234(void);
void gcode_M235(uint8_t power);
void gcode_M236(void);
uint16_t gcode_M238(uint8_t power);
void gcode_M239(void);
void gcode_M240(void);
void gcode_M241(long num_milliseconds);
void gcode_M242(void);
void gcode_M243(void);
void gcode_M244(void);
void gcode_M245(void);
void gcode_M250(void);
void gcode_M280(void);
void gcode_M300(void);
void gcode_M301(void);
void gcode_M303(void);
void gcode_M304(void);
void gcode_M350(void);
void gcode_M351(void);
bool gcode_M360(void);
bool gcode_M361(void);
bool gcode_M362(void);
bool gcode_M363(void);
bool gcode_M364(void);
void gcode_M365(void);
void gcode_M380(void);
void gcode_M381(void);
void gcode_M399(void);
void gcode_M400(void);
void gcode_M402(void);
void gcode_M404(void);
void gcode_M405(void);
void gcode_M406(void);
void gcode_M407(void);
void gcode_M410(void);
void gcode_M420(void);
void gcode_M421(void);
void gcode_M428(void);
void gcode_M500(void);
void gcode_M501(void);
void gcode_M502(void);
void gcode_M503(void);
void gcode_M540(void);
void gcode_M600(void);
void gcode_M605(void);
void gcode_M665(void);
void gcode_M666(void);
void gcode_M852(void);
void gcode_M907(void);
void gcode_M908(void);
void gcode_M928(void);
void gcode_M999(void);

#endif  // M_CODES_H_