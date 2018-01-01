void minProc() {
  oldMin = minute();
	if (sampleCount == 0) pvMinuteAvg = 0;
	else pvMinuteAvg = pvSum / sampleCount;
  pvMinuteMax = pvMax;
  pvMinuteMin = pvMin;

	Serial.print(timeStamp());
	printFloat("Temp = ", pvInvTemp);
	printFloat("PV1 = ", pvVolts1);
	printFloat("PV2 = ", pvVolts2);
	printFloat("PA1 = ", pvAmps1);
	printFloat("PA2 = ", pvAmps2);
	printFloat("Pmin = ", pvMinuteMin);
	printFloat("Pavg = ", pvMinuteAvg);
	printFloat("Pmax = ", pvMinuteMax);
	printFloat("E today = ", pvEnergyToday);
  printFloat("E annual = ", pvEnergyAnnual);
  printFloat("Vac = ", acVolts);
//  printFloat("Fac = ", acFrequency);
	printFloat("samples = ", sampleCount);
  Serial.println();

  if (pvMinuteMax > pvQtrMax) pvQtrMax = pvMinuteMax;
  if (pvMinuteMin < pvQtrMin) pvQtrMin = pvMinuteMin;

  pvMax = 0.0;
	pvMin = 9999.0;
	pvSum = 0.0;
	sampleCount = 0;
  if (oldMin/15 == oldQtr) return;
  storeData();
  oldQtr=oldMin/15;
  oldHour=hour();
  pvQtrMax = 0.0;
  pvQtrMin = 9999.0;
  if (oldDay == day()) return;
  delay(30000);   //wait 30s to clear midNight reliably
  setupTime();
}
