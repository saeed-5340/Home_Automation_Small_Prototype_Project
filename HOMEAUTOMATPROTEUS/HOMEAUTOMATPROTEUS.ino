//Counting People
int maxPeople = 10; // maximum number of people allowed before the alarm goes off
int sensitivity = 10; // lower values will make it more sensitive and higher values will make it less sensitive

int currentPeople = 0;

int insideSensor[] = {9, 8}; // Pin configuration for the sensor placed inside the room
int outsideSensor[] = {11,10}; // Pin configuration for the sensor placed outside the room
int insideSensorInitial;
int outsideSensorInitial;

String sequence = "";

int timeoutCounter = 0;

void setup() {
  // Setup code
  Serial.begin(9600);

  delay(500);
  insideSensorInitial = measureDistance(insideSensor);
  outsideSensorInitial = measureDistance(outsideSensor);
}

void loop() {
  // Read ultrasonic sensors
  int insideSensorVal = measureDistance(insideSensor);
  int outsideSensorVal = measureDistance(outsideSensor);
  
  // Process the data
  if (insideSensorVal < insideSensorInitial && sequence.charAt(0) != '1') {
    sequence += "1";
  } else if (outsideSensorVal < 16 && sequence.charAt(0) != '2') {
    sequence += "2";
  }
  
  if (sequence.equals("21")) {
    currentPeople++;  
    sequence = "";
    delay(1000); // Delay to avoid multiple counts for the same person
  } else if (sequence.equals("12") && currentPeople > 0) {
    currentPeople--;  
    sequence = "";
    delay(1000); // Delay to avoid multiple counts for the same person
  }

  // Resets the sequence if it is invalid or timeouts
  if (sequence.length() > 2 || sequence.equals("11") || sequence.equals("22") || timeoutCounter > 200) {
    sequence = "";  
  }

  if (sequence.length() == 1) {
    timeoutCounter++;
  } else {
    timeoutCounter = 0;
  }
  // Print values to serial
  Serial.print(" Seq: ");
  Serial.print(sequence);
  Serial.print(" Inside: ");
  Serial.print(insideSensorVal);
  Serial.print(" Outside: ");
  Serial.println(outsideSensorVal);
  Serial.print("  Current people ");
  Serial.print(currentPeople);

  // If the number of people is too high, trigger the buzzer
  if (currentPeople > maxPeople) {
    // tone(buzzer, 1700); // Buzzer pin not defined, commented out
  } else {
    // noTone(buzzer); // Buzzer pin not defined, commented out
  }
}

// Returns the distance of the ultrasonic sensor that is passed in
// a[0] = echo, a[1] = trig
int measureDistance(int a[]) {
  pinMode(a[1], OUTPUT);
  pinMode(a[0], INPUT);
  digitalWrite(a[1], LOW);
  delayMicroseconds(2);
  digitalWrite(a[1], HIGH);
  delayMicroseconds(10);
  digitalWrite(a[1], LOW);
  long duration = pulseIn(a[0], HIGH, 1000000);
  return duration / 29 / 2;
}
