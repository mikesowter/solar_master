/*-------- display code ----------*/

void diagMess(const char* mess) {
  fd.print(dateStamp());
  fd.print(" ");
  fd.print(timeStamp());
  fd.println(mess);
}

void errMess(const char* mess) {
  fe.print(dateStamp());
  fe.print(" ");
  fe.print(timeStamp());
  fe.println(mess);
}

char* dateStamp() {
  // digital display of the date
  strcpy(dateStr,p2d(year()%100));
  strcat(dateStr,p2d(month()));
  strcat(dateStr,p2d(day()));
  return dateStr;
}

char* timeStamp() {
  // digital display of the time
  strcpy(timeStr,p2d(hour()));
  strcat(timeStr,":");
  strcat(timeStr,p2d(minute()));
  strcat(timeStr,":");
  strcat(timeStr,p2d(second()));
  strcat(timeStr," ");
  return timeStr;
}

// print byte as 2 HEX digits, then a space
void printHex(uint8_t X) {
  if (X<16) Serial.print("0");
  Serial.print(X,HEX);
  Serial.print(" ");
}

// convert integer into a 2 Hex string dd
char* p2h(byte b) {
  int hi=b/16;
  if (hi>9) d2Str[0] = hi +'A' -10;
  else d2Str[0] = hi +'0';
  int lo=b%16;
  if (lo>9) d2Str[1] = lo +'A' -10;
  else d2Str[1] = lo +'0';
  return d2Str;
}

// convert integer into a 2 Dec string dd
char* p2d(byte b) {
  d2Str[0]=b/10+'0';
  d2Str[1]=b%10+'0';
  return d2Str;
}

// convert float into an 8 char string ddddd.dd
char* p8d(float f) {
  if (f > 99999.99) {
    diagMess("p8d overflow");
    return (char*)"99999.99";
  }
  if (f < 0.0) f=-f;
  int w = (int)f;
  int d = 10000;
  byte ptr = 0;
  bool started = false;
  for ( int n=0;n<5;n++ ) {
    if ( w/d != 0 || n==4 ) started = true;
    if (started) d8Str[ptr++] = w/d +'0';
    w = w%d;
    d /= 10;
  }
  d8Str[ptr++] = '.';
  d = (int)(100.0*f)-100*(int)f;
  d8Str[ptr++]=d/10 +'0';
  d8Str[ptr++]=d%10 +'0';
  d8Str[ptr]='\0';
  return d8Str;
}
