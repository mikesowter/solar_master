void minProc() {
	if (sampleCount < 10) return;
  oldMin = minute();
	pvMinuteAvg = pvSum / sampleCount;
  pvMinuteMax = pvMax;
  pvMinuteMin = pvMin;
  // flush fault files
  fd.flush();
  fe.flush();

  if (pvMinuteMax > pvQtrMax) pvQtrMax = pvMinuteMax;
  if (pvMinuteMin < pvQtrMin) pvQtrMin = pvMinuteMin;

  pvMax = 0.0;
	pvMin = 9999.0;
	pvSum = 0.0;
	sampleCount = 0;
  if (oldMin/15 == oldQtr) return;
  if (pvQtrMin == 0) pvQtrMin = pvMinuteMin;
  storeData();
  oldQtr = oldMin/15;
  oldHour = hour();
  pvQtrMax = 0.0;
  pvQtrMin = 9999.0;
  return;
}
