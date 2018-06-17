unsigned long getTime() {

  unsigned long year2030 = 1893456000UL;
  unsigned long year2017 = 1483207200UL;

  while(1) {
    while (udp.parsePacket()!= NTP_PACKET_SIZE) {
      sendNTPrequest(timeServerIP);
      delay(1000);
    }
    startSeconds = getNTPreply();
    yield();
    if (startSeconds > year2017 && startSeconds < year2030) break;
  }
  return startSeconds;
}

// send an NTP request to the time server at the given address
void sendNTPrequest(IPAddress& address)
{
  memset(byteBuf, 0, BUFFER_SIZE);
  // Initialize values needed to form NTP request
  byteBuf[0] = 0b11100011;   // LI, Version, Mode
  byteBuf[1] = 0;     // Stratum, or type of clock
  byteBuf[2] = 6;     // Polling Interval
  byteBuf[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  byteBuf[12]  = 49;
  byteBuf[13]  = 0x4E;
  byteBuf[14]  = 49;
  byteBuf[15]  = 52;
  udp.beginPacket(address, 123); //NTP requests are to port 123
  udp.write(byteBuf, NTP_PACKET_SIZE);
  udp.endPacket();
}

unsigned long getNTPreply(){
  udp.read(byteBuf, NTP_PACKET_SIZE); // read the packet into the buffer
  unsigned long highWord = word(byteBuf[40], byteBuf[41]);
  unsigned long lowWord = word(byteBuf[42], byteBuf[43]);
  // this is NTP time :
  unsigned long secsSince1900 = highWord << 16 | lowWord;   // seconds since Jan 1 1900
  // now convert NTP time into Unix time:
  unsigned long GMT = secsSince1900 - 2208988800UL;         // seconds since Jan 1 1970
  //add timezone offset and return
  return GMT + TIME_ZONE*3600;
}
