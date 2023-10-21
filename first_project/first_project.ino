int count = 0, indeks = 0;
int onLed[4] = {0, 0, 0, 0};
int lockedLed[4] = {0, 0, 0, 0};

void setup() {
  pinMode(7, INPUT);
  pinMode(6, INPUT);
  pinMode(5, INPUT);
  pinMode(11, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(8, OUTPUT);
  Serial.begin(9600);
}

void loop() {

  // Ganti elemen di onLed buat dinyalain
  if (digitalRead(7)) {
    count++;
    indeks = count % 4;
    onLed[indeks] = 1;

    for (int i = 0; i < 4; i++) {
      if (not((lockedLed[i]) || (i == indeks))) {
        onLed[i] = 0;
        }
      }
  }

  // Ganti elemen di lockedLed buat dilock
  if (digitalRead(6)) {
    indeks = count % 4;
    lockedLed[indeks] = 1;
  }

  // Print serial dan reset onLed serta lockedLed
  if (digitalRead(5)) {
    for (int i = 0; i < 4; i++) {
      if (i == 3) {
        Serial.println(lockedLed[i]);
      } else {
        Serial.print(lockedLed[i]);
      }
      onLed[i] = 0;
      lockedLed[i] = 0;
    }
  }

  // Nyalain LED berdasarkan onLed
  for (int i = 0; i < 4; i++) {
    if (onLed[i]) {
      digitalWrite(i+8, HIGH);
    } else {
      digitalWrite(i+8, LOW);
    }
  }

  while ((digitalRead(7)) || (digitalRead(6)) || (digitalRead(5))) {delay(10);}
  delay(10);
}

