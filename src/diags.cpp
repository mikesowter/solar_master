#include <arduino.h>
#include <fs.h>

char* dateStamp();
char* timeStamp();
void printHex(uint8_t X);
char* i2sh(uint8_t b);
char* i2sd(uint8_t b);
char* f8ds(float f);

extern File fd,fe;

char d2Str[] = "12";
char d8Str[] = "12345.78";

void diagMess(const char* mess) {
  fd.print(dateStamp());
  fd.print(" ");
  fd.print(timeStamp());
  Serial.println(mess);
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
  extern char dateStr[];
  strcpy(dateStr,i2sd(year()%100));
  strcat(dateStr,i2sd(month()));
  strcat(dateStr,i2sd(day()));
  return dateStr;
}

char* timeStamp() {
  // digital display of the time
  extern char timeStr[];
  strcpy(timeStr,i2sd(hour()));
  strcat(timeStr,":");
  strcat(timeStr,i2sd(minute()));
  strcat(timeStr,":");
  strcat(timeStr,i2sd(second()));
  strcat(timeStr," ");
  return timeStr;
}

// print uint8_t as 2 HEX digits, then a space
void printHex(uint8_t X) {
  if (X<16) Serial.print("0");
  Serial.print(X,HEX);
  Serial.print(" ");
}

// convert integer into a 2 Hex string dd
char* i2sh(uint8_t b) {
  int hi=b/16;
  if (hi>9) d2Str[0] = hi +'A' -10;
  else d2Str[0] = hi +'0';
  int lo=b%16;
  if (lo>9) d2Str[1] = lo +'A' -10;
  else d2Str[1] = lo +'0';
  return d2Str;
}

// convert integer into a 2 Dec string dd
char* i2sd(uint8_t b) {
  d2Str[0]=b/10+'0';
  d2Str[1]=b%10+'0';
  return d2Str;
}

// convert float into an 8 char string ddddd.dd
char* f8ds(float f) {
  if (f > 99999.99) {
    diagMess("f8ds overflow");
    return (char*)"99999.99";
  }
  if (f < 0.0) f=-f;
  int w = (int)f;
  int d = 10000;
  uint8_t ptr = 0;
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
