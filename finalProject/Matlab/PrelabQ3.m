norm = int16(165);
G = int16(74);
adc_in = int16(83);
adc_norm = int16(adc_in*G/norm)

adc_norm = int16(floor(adc_in/norm));
adc_scale = int16(adc_norm*G)

adc = 83*74/165