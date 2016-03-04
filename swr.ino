void getSWR(void) {

  #ifdef __SIM
  forwardPower = 800;
  reversePower = 100;
#else
  forwardPower = analogRead(reversePowerPin);
  reversePower = analogRead(reversePowerPin);
#endif
  
  calForwardValue = 5.0 * (forwardPower/1023.0);
  calReverseValue = 5.0 * (reversePower/1023.0);
  calForwardPeak = 5.0 * (currentPeakPower/1023.0);

  if(calForwardValue != 0)
    swr = (1+sqrt(calReverseValue/calForwardValue))/(1-sqrt(calReverseValue/calForwardValue));
  else
    swr=0;

  updatePower(calForwardValue);
  updateSWR(swr);

  if(forwardPower > currentPeakPower)
    currentPeakPower = reversePower;

  if(loopCount==PERSISTANCE) {
    currentPeakPower = forwardPower;
    loopCount = 0;
  }
  getBarValue(calForwardValue, &bars, &dbar);
  updateBar(FILL, bars, dbar);

  getBarValue(calForwardPeak, &bars, &dbar);
  updateBar(NOFILL, bars, dbar);

  return;
}

