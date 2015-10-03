const int coinSelector = A0; // Analog input pin that the coin selector uses
const int signalCostFactor = 5; // Each signal pulse is worth 5p

int signalValue = 0; // For storing value from analog input
int state; // Current state
int lastState = 0; // Last state
int balance = 0; // Pence
int update = 0; // Used for updating

long updateDebounceTime = 0; // The last time we sent an update
long updateDebounceDelay = 500; // Update 500ms after last singal pulse

void setup() {

  pinMode(13, OUTPUT); // Status LED

  Serial.begin(9600); // Setup serial at 9600 baud

  delay(2000); // Don't start main loop untill we're sure that the coin selector has started

}

void loop() {

  signalValue = analogRead(coinSelector); // Read analog value from coin selector

  if (signalValue > 1000) {

    state = 1; // State is 1 as we're high

  } else {

    state = 0;

    // Should we send a balance update

    if (update == 0) {

      if ((millis() - updateDebounceTime) > updateDebounceDelay) {

        Serial.println(balance); // Send an update over serial

        update = 1; // Make sure we don't run again, till next coin

      }

    }

  }

  if (state != lastState) {

    // Process new signal

    if (state == 1) {

      digitalWrite(13, HIGH); // Turn status LED on to show signal

      balance = balance + signalCostFactor; // Update balance

      updateDebounceTime = millis(); // Update last time we processed a signal

      update = 0; // Time to send a update now?

    } else {

      digitalWrite(13, LOW);  // Turn status LED off

    }

    lastState = state; // Update last state

  }

  delay(1);

}
