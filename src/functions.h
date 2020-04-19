#include <WiFiClient.h>

// main
void ISRwatchDog ();
// network
void joinNet();
unsigned long getTime();
void sendNTPrequest(IPAddress& address);
unsigned long getNTPreply();
// OTA
uint8_t init_OTA();
// diags
void diagMess(const char* mess);
void errMess(const char* mess);
char* dateStamp();
char* timeStamp();
void printHex(uint8_t X);
char* i2sh(uint8_t b);
char* i2sd(uint8_t b);
char* f8ds(float f);
// main
void watchWait(uint32_t timer);
// setupTime
void setupTime();
void dayCheck();
// setupInv
void setupInv();
// queryInv
void queryInv();
void readBytes(bool HexOut);
void addCstring(const char* s);
bool goodCheckSum(uint8_t len);
// updateTotal
bool readTotal();
bool updateTotal();
// dirList
void handleDir();
void listFile();
void helpPage();
// web
void handleMetrics();
void handleNotFound();
// minProc
void minProc();
// setupServer
void setupServer();
// setupSPIFFS
void setupSPIFFS ();
// writeFiles
uint8_t storeData();
uint8_t openFile(const char* s);
void WriteQtr();
void checkScan();
