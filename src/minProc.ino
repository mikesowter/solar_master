void minProc() {
  oldMin = minute();
	if (sampleCount == 0) pvPowerAvg = 0;
	else pvPowerAvg /= sampleCount;

	Serial.print(timeStamp());
	printFloat("Temp = ", pvInvTemp);
	printFloat("PV1 = ", pvVolts1);
	printFloat("PV2 = ", pvVolts2);
	printFloat("PA1 = ", pvAmps1);
	printFloat("PA2 = ", pvAmps2);
	printFloat("Pmin = ", pvPowerMin);
	printFloat("Pavg = ", pvPowerAvg);
	printFloat("Pmax = ", pvPowerMax);
	printFloat("E today = ", pvEnergyToday);
	printFloat("samples = ", sampleCount);
	Serial.println();

  if (pvPowerMax > qtrMax) qtrMax = pvPowerMax;
  if (pvPowerMin < qtrMin) qtrMin = pvPowerMin;

  pvPowerMax = 0.0;
	pvPowerMin = 9999.0;
	pvPowerAvg = 0.0;
	sampleCount = 0.0;
  if (oldMin/15 == oldQtr) return;
  storeData();
  oldQtr=oldMin/15;
  qtrMax = 0.0;
  qtrMin = 9999.0;
  if (oldDay == day()) return;
  delay(30000);   //wait 30s to clear midNight reliably
  setupTime();
}
