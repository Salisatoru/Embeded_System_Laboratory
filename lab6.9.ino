#define BUZZER_PIN 21
#define BUTTON_PIN 22

volatile int speedLevel = 2; // เริ่มต้นที่ระดับปกติ (0 = ช้ามาก, 4 = เร็วมาก)
const float speedFactors[5] = {2.0, 1.5, 1.0, 0.75, 0.5}; // ปรับตามระดับความเร็ว

// โน้ตของเพลง "Giorno's Theme - JoJo's Bizarre Adventure"
int melody[] = {
    370, 294, 294, 330, // F# D D E
    370, 330, 294, 277, // F E D Db
    294, 330, 370, 494, // D E F# B
    494, 277, 294, 330, // B(lower) Db D E
    294, 277, 220, 196, // D Db A G
    370, 294, 294, 330, // F# D D E
    370, 330, 294, 277, // F E D Db
    294, 330, 370, 494, // D E F# B
    494, 277, 294, 392, // B Db(higher) D(higher) G
    370, 349, 294, 330 // F# F D(higher) E(higher)
};

int noteDurations[] = {
    400, 400, 100, 100,
    200, 200, 100, 200,
    200, 100, 400, 400,
    100, 100, 200, 200,
    100, 100, 200, 200,
    400, 400, 100, 100,
    200, 200, 100, 200,
    200, 100, 400, 400,
    100, 100, 200, 200
};

void IRAM_ATTR handleButton() {
    speedLevel = (speedLevel + 1) % 5; // วนลูประดับความเร็ว 0-4
}

void setup() {
    pinMode(BUZZER_PIN, OUTPUT);
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    attachInterrupt(BUTTON_PIN, handleButton, FALLING);
}

void loop() {
    for (int i = 0; i < sizeof(melody) / sizeof(melody[0]); i++) {
        int adjustedDuration = noteDurations[i] * speedFactors[speedLevel];
        tone(BUZZER_PIN, melody[i]);
        delay(adjustedDuration);
        noTone(BUZZER_PIN);
        delay(50 * speedFactors[speedLevel]);
    }
    delay(2000 * speedFactors[speedLevel]);
}

