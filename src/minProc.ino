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

	pvPowerMax = 0;
	pvPowerMin = 9999;
	pvPowerAvg = 0;
	sampleCount = 0;
}
