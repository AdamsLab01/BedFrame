// PIN names
const int LED = 0; // PIN the LED array is connected to (more accuratly PIN the MOSFET that controlls the LED array is connected to). 
const int LDR = A1; // PIN the LDR is connected to.
const int BrightnessPot = A2; // PIN the brightness pot is connected to.
const int ThresholdPot = A3; // PIN the threshold pot is connected to.

// Variables
int LDRbase = 0; // Reading of the LDR at boot to establish a base value.
int LDRreading = 0; // Reading from the LDR during program run.
int ThresholdValue = 0; // Reading of the ThresholdPot.
int BrightnessPotOutput = 0; // Raw reading from the BrightnessPot.
int BrightnessValue = 0; // Mapped reading of the BrightnessPot.

unsigned long ReadingDelay = 0; // Place holder for recodring curent millis.
unsigned long ReadingDelayLength = 6000; // How long (in milliseconds) we wait before taking a reading from the LDR and POTs. This and using the average of 10 readings acts as a debounce.

const int LDRnumReadings = 10; // How many readings from the LDR we take.
int LDRreadings[LDRnumReadings]; // Array to store the LDR readings.
int LDRreadingsIndex = 0; // Current array index.
int LDRreadingsTotal = 0; // The total of all the LDR readings.
int LDRreadingsAverage = 0; // The average of all the LDR readings.

void setup() {
  // PIN modes
  pinMode(LED, OUTPUT);
  pinMode(LDR, INPUT);
  pinMode(BrightnessPot, INPUT);
  pinMode(ThresholdPot, INPUT);

  LDRbase = analogRead(LDR); // Get base reading from LDR at boot.

  ReadingDelay = millis(); // Set delay clock.

  for (int InitializeReadings = 0; InitializeReadings < LDRnumReadings; InitializeReadings++) { // Initialize the LDR array to 0 at boot.
    LDRreadings[InitializeReadings] = 0;
  }
}

void loop() {  
  F_GetPotReadings(); // Get the BrightnessPot and ThresholdPot readings.

  F_GetLDRreadings(); // Get the LDR readings.

  F_SetLEDs(); // Set the state of the LED array based on the above readings.
}

// Functions //
void F_GetPotReadings() {
  BrightnessPotOutput = analogRead(BrightnessPot); // Get the current value of the BrightnessPot.
  BrightnessValue = map(BrightnessPotOutput, 0, 1023, 0, 255); // Map BrightnessPotValue to a usable value between 0 and 255.
  ThresholdValue = analogRead(ThresholdPot); // Get the current value of the ThresholdPot.
}

void F_GetLDRreadings() {
  if (millis() - ReadingDelay > ReadingDelayLength) { // If we've waited long enough get readings from the LDR.
    LDRreadingsTotal = LDRreadingsTotal - LDRreadings[LDRreadingsIndex]; // Subtract the last LDR reading.
    LDRreadings[LDRreadingsIndex] = analogRead(LDR); // Get reading from the LDR and record it to the array.
    LDRreadingsTotal = LDRreadingsTotal + LDRreadings[LDRreadingsIndex]; // Add the latest LDR reading to the total.
    LDRreadingsIndex = LDRreadingsIndex + 1; // Increment the array to the next position.

    if (LDRreadingsIndex >= LDRnumReadings) {   // When we've reached the end of the array, reset the index and calculate values.
      LDRreadingsIndex = 0;
    }
    LDRreadingsAverage = LDRreadingsTotal / LDRnumReadings; // Get the average of the LDR readings.
    ReadingDelay = millis(); // Reset the delay clock.
  }
}

void F_SetLEDs() {
  if ((LDRbase - LDRreadingsAverage) > ThresholdValue) { // Subtract the base LDR reading from the average, if the resultis is greater that the ThresholdValue, we turn the LEDs on.
    analogWrite(LED, BrightnessValue); // Turn on the LEDs to the brightness level based off BrightnessPot.
  } else { // Otherwise turn the LEDs off.
    analogWrite(LED, 0);
  }
}

