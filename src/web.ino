#include <Arduino.h>

char htmlStr0[] =
"<!DOCTYPE html>"
"<html>"
  "<head>"
    "<!--Load the AJAX API-->"
    "<script type='text/javascript' src='https://www.gstatic.com/charts/loader.js'></script>"
  	"<script type='text/javascript'> google.charts.load('current', {'packages':['corechart']}); google.charts.setOnLoadCallback(drawChart);"
    " function drawChart() {"
        "var data = new google.visualization.DataTable();"
        "data.addColumn(";
char htmlStr1[] =
        ", 'Time');"
        "data.addColumn('number', 'High');"
        "data.addColumn('number', 'Average');"
        "data.addColumn('number', 'Low');"
        "data.addRows([";
char htmlStr2[] =
        "]);"
        "var options = {'title':'";
char htmlStr3[] =
        "             ','width':1000,"
        "              'height':700,"
        "                colors: ['red', 'orange', 'green']};"
        "var chart = new google.visualization.LineChart(document.getElementById('chart_div'));"
        "chart.draw(data, options);"
        "}"
      "</script>"
    "</head>"
  "<body>"
    "<div id='chart_div'></div>"
    "<HR>"
    "Recent messages:";
char htmlStr4[] =
    "<HR>"
  "</body>"
"</html>";

void handleRoot() {
  htmlStr[0]='\0';
  addCstring( htmlStr0 );
  addCstring( "'timeofday'" );
  addCstring( htmlStr1 );
  add3PowerData();
  addCstring( htmlStr2 );
  add3TitleData();
  addCstring( htmlStr3 );
  addErrMess();
  addCstring( htmlStr4 );
  server.send ( 200, "text/html", htmlStr );
  //Serial.println(htmlStr);
}

void handleDay() {
  htmlStr[0]='\0';
  addCstring( htmlStr0 );
  addCstring( "'timeofday'" );
  addCstring( htmlStr1 );
  add24PowerData();
  addCstring( htmlStr2 );
  add24TitleData();
  addCstring( htmlStr3 );
  addErrMess();
  addCstring( htmlStr4 );
  server.send ( 200, "text/html", htmlStr );
  //Serial.println(htmlStr);
}

void handleAvg() {
  htmlStr[0]='\0';
  addCstring("<!DOCTYPE html><html><body><P>");
  addAvgData();
  addCstring( htmlStr4 );
  server.send ( 200, "text/html", htmlStr );
  //Serial.println(htmlStr);
}

void handleMetric() {
  htmlStr[0]='\0';
//  addCstring("<!DOCTYPE html><html><body>");
//  addCstring(timeStamp());
  addCstring("# TYPE curPower guage" );
  addCstring("\ncurPower ");
  addCstring(p8d(power));
  addCstring("\n# TYPE minPower guage" );
  addCstring("\nminPower ");
  addCstring(p8d(minData[minPtr].lo));
  addCstring("\n# TYPE avgPower guage" );
  addCstring("\navgPower ");
  addCstring(p8d(minData[minPtr].av));
  addCstring("\n# TYPE maxPower guage" );
  addCstring("\nmaxPower ");
  addCstring(p8d(minData[minPtr].hi));
  addCstring("\n# TYPE T11Energy guage" );
  addCstring("\nT11Energy ");
  addCstring(p8d(T11Energy));
  addCstring("\n# TYPE T33Energy guage" );
  addCstring("\nT33Energy ");
  addCstring(p8d(T33Energy));
  addCstring( "\n" );
  server.send ( 200, "text/plain", htmlStr );
  //Serial.println(htmlStr);
}

void handleNotFound() {
  server.uri().toCharArray(userText, 14);
  Serial.print(timeStamp());
  Serial.println(userText);
  if (strncmp(userText,"/reset",6)==0) {
    errMessage("User requested restart");
    fd.close();
    fe.close();
    strcpy(outBuf,"<!DOCTYPE html><html><head><HR>User requested restart<HR></head></html>");
    server.send ( 200, "text/html", outBuf );
    ESP.restart();
  }
  else if (strncmp(userText,"/diags",6)==0) {
    listDiags();
  }
  else if (strncmp(userText,"/remdiags",9)==0) {
    SPIFFS.remove("/diags.txt");
    fd = SPIFFS.open("/diags.txt", "a+");
    fd.println(dateStamp());
    strcpy(outBuf,"<!DOCTYPE html><html><head><HR>Diags deleted<HR></head></html>");
    server.send ( 200, "text/html", outBuf );
  }
  else if (strncmp(userText,"/filesave",9)==0) {
    diagMess("User requested file save");
    uploadDay();
    uploadMonth();
    strcpy(outBuf,"<!DOCTYPE html><html><head><HR>Safe to Shutdown<HR></head></html>");
    server.send ( 200, "text/html", outBuf );
  }
  else if (SPIFFS.exists(userText)) {
    strcpy(outBuf,"<!DOCTYPE html><html><head><HR>Sending File: \"");
    strcat(outBuf,userText);
    strcat(outBuf,"\"<HR></head></html>");
    server.send ( 200, "text/html", outBuf );
    strcpy(fileName,userText);
    uploadFile();
    delay(5);
  }
  else if (strncmp(userText,"/favicon.ico",12)==0) {
  }
  else if (strncmp(userText,"/apple",6)==0) {
  }
  else if (strncmp(userText,"/dir",4)==0) {
    listFiles();
  }
  else {
    strcpy(outBuf,userText);
    strcat(outBuf," is not a valid option");
    diagMess(outBuf);
    helpPage();
  }
}

uint8_t listDiags() {
  char line[81];
  htmlStr[0]='\0';
  fd.seek(0UL,SeekSet);
  while (fd.available()) {
    int k=fd.readBytesUntil('\r',line,80);
    line[k]='\0';
    addCstring(line);
    addCstring("\n");
  }
  fd.print("length of diag list:");
  fd.println(htmlLen);
  server.send ( 200, "text/plain", htmlStr );
  return 1;
}
