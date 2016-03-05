/**********************************************************************
 * SWRduino - SWR meter with Maidenhead/GPS location
 * Copyright (C) 2016  Howard Graves (hgraves@hiwaay.net)
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ***********************************************************************/

void getSWR(void) {

	if(TESTMODE == 1) {
		forwardPower = 800;
		reversePower = 100;
	} else {
		forwardPower = analogRead(reversePowerPin);
		reversePower = analogRead(reversePowerPin);
	}

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

	if(persistanceCount==PERSISTANCE) {
		currentPeakPower = forwardPower;
		persistanceCount = 0;
	}
	getBarValue(calForwardValue, &bars, &dbar);
	updateBar(FILL, bars, dbar);

	getBarValue(calForwardPeak, &bars, &dbar);
	updateBar(NOFILL, bars, dbar);

	return;
}

