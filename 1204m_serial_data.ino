/*  Example requesting Voltage and Temperature from a Clone of Curtis 1204M - no clue if this works on legitimate Curtis controllers.  
 *  Sniffed packets while connected to Curtis motor control software to get 'reqeuest' values.
*/

// set this to the hardware serial port you wish to use
#define HWSERIAL Serial1
byte request_voltage[] = {0x01, 0x00, 0x01, 0x00, 0x4b, 0x00, 0x4d};
double voltage = -1;
int temperature = -1;
double current = -1;
byte request_temperature[] = {0x01, 0x00, 0x01, 0x00, 0x4c, 0x00, 0x4e};
byte incomingBytes[4];

//LEM Analog outputs
int vrefPin = A0;
int voutPin = A1;

void setup() {
	Serial.begin(9600);
	HWSERIAL.begin(9600);
}

void loop() {
  Serial.print("Voltage = ");  
  Serial.print(requestVoltage());
  Serial.print(", Temperature = ");  
  Serial.print(requestTemperature());
  Serial.print(", Current = ");  
  Serial.println(readCurrentSensor());
  delay(100);
}

double requestVoltage() {
  if (HWSERIAL.available() > 0) {
    HWSERIAL.write(request_voltage, sizeof(request_voltage));
    HWSERIAL.readBytes(incomingBytes, 4);
    voltage = incomingBytes[2] / 255.00 * 100.00;
    //Serial.print(incomingBytes[0]); Serial.print(", "); 
    //Serial.print(incomingBytes[1]); Serial.print(", ");
    //Serial.print(incomingBytes[2]); Serial.print(", ");
    //Serial.println(incomingBytes[3]);
    HWSERIAL.flush();
    delay(10);
    return voltage;
  }
  return -1;
}


int requestTemperature() {
  if (HWSERIAL.available() > 0) {
    HWSERIAL.write(request_temperature, sizeof(request_temperature));
    HWSERIAL.readBytes(incomingBytes, 4);
    temperature = incomingBytes[2];
    //Serial.print(incomingBytes[0]); Serial.print(", "); 
    //Serial.print(incomingBytes[1]); Serial.print(", ");
    //Serial.print(incomingBytes[2]); Serial.print(", ");
    //Serial.println(incomingBytes[3]);
    HWSERIAL.flush();
    delay(10);
    return temperature;
  }
  return -1;
}

double readCurrentSensor() {
    // read the value from the sensor:
    int vref_value = analogRead(vrefPin);  
    int vout_value = analogRead(voutPin);  
    //Serial.print("Vref = "); Serial.print(vref_value/1024.00 * 3.30); 
    //Serial.print(", Vout = "); Serial.println(vout_value/1024.00 * 3.30); 
    double current = ((vout_value - vref_value)/1024.00 * 3.30) * 300.00/2.50;
    return current;
}
