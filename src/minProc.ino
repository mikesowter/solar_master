void minProc() {
  oldMin = minute();
	if (sampleCount == 0) pvMinuteAvg = 0;
	else pvMinuteAvg = pvSum / sampleCount;
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
  // check for end of day
  if (hour()>=16 && pvEnergyToday == pvETLast) updateTotal();
  pvETLast = pvEnergyToday;
  storeData();
  oldQtr=oldMin/15;
  oldHour=hour();
  pvQtrMax = 0.0;
  pvQtrMin = 9999.0;
  if (oldDay == day()) return;
  delay(6000);   //wait 6s to clear midNight reliably
  setupTime();
  dayStored=false;
  return;
}
