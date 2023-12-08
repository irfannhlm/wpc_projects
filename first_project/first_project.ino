// Made by Kelompok 7:
// - Akmal Hafizh Musyaffa / 13222057
// - Irfan Nurhakim Hilmi / 13222048
// - Mahardhika Putra Adipratama / 13222081
// - Mohammad Ari Alexander Azis / 13222093
// - Nurul Annisa / 13222109
// - Ronaldo / 18322007

int count = 0, indeks = 0;
int onLed[4] = { 0, 0, 0, 0 };
int lockedLed[4] = { 0, 0, 0, 0 };
const char buttonPins[3] = { D0, D1, D2 };
const char ledPins[4] = { D5, D6, D7, D8 };

void setup() {
  pinMode(buttonPins[0], INPUT);
  pinMode(buttonPins[1], INPUT);
  pinMode(buttonPins[2], INPUT);
  pinMode(ledPins[0], OUTPUT);
  pinMode(ledPins[1], OUTPUT);
  pinMode(ledPins[2], OUTPUT);
  pinMode(ledPins[3], OUTPUT);
  Serial.begin(9600);
}

void loop() {

  // Ganti elemen di onLed buat dinyalain
  if (digitalRead(buttonPins[0])) {
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
  if (digitalRead(buttonPins[1])) {
    indeks = count % 4;
    lockedLed[indeks] = 1;
  }

  // Print serial dan reset onLed serta lockedLed
  if (digitalRead(buttonPins[2])) {
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
      digitalWrite(ledPins[i], HIGH);
    } else {
      digitalWrite(ledPins[i], LOW);
    }
  }

  while ((digitalRead(buttonPins[0])) || (digitalRead(buttonPins[1])) || (digitalRead(buttonPins[2]))) { delay(10); }
  delay(10);
}
