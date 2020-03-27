const uint8_t outStr1[11] = { 0xA5, 0xA5, 0x01, 0x00, 0x30, 0x44, 0x00, 0xFE, 0x41, 0x0A, 0x0D };			// request inverter disconnect
const uint8_t outStr2[11] = { 0xA5, 0xA5, 0x01, 0x00, 0x30, 0x40, 0x00, 0xFE, 0x45, 0x0A, 0x0D };			// is anybody out there?
/* old inverter
const uint8_t outStr3[28] = { 0xA5, 0xA5, 0x01, 0x00, 0x30, 0x41, 0x11, 0x31, 0x35, 0x32, 0x32, 0x31, 0x33, 0x31, 0x31, 0x31,
												      0x30, 0x30, 0x30, 0x36, 0x20, 0x20, 0x20, 0x11, 0xFB, 0x3B, 0x0A, 0x0D };	// assign address 0x11
const uint8_t outStr4[11] = { 0xA5, 0xA5, 0x01, 0x11, 0x31, 0x42, 0x00, 0xFE, 0x31, 0x0A, 0x0D };			  // request data from 0x11
new inverter */
const uint8_t outStr3[28] = { 0xA5, 0xA5, 0x01, 0x00, 0x30, 0x41, 0x11, 0x31, 0x35, 0x31, 0x33, 0x31, 0x35, 0x30, 0x35, 0x31,
												      0x30, 0x30, 0x33, 0x35, 0x20, 0x20, 0x20, 0x01, 0xFB, 0x44, 0x0A, 0x0D };	// assign address 0x01
const uint8_t outStr4[11] = { 0xA5, 0xA5, 0x01, 0x01, 0x31, 0x42, 0x00, 0xFE, 0x41, 0x0A, 0x0D };			  // request data from 0x01
