#include <GyverOLED.h>
#include <pins.h>

#define OLED_SOFT_BUFFER_64
#define ITEMS 7
#define analogPin A0
GyverOLED<SSD1306_128x64, OLED_BUFFER> oled;

// Порогові значення для кнопок
int thresholds[] = { 4095, 1790, 800, 2260, 2695 };  // Down, Up, Enter, Back (нова кнопка), Right
int buttonRange = 100; // Розмір діапазону для порогових значень

// Змінні для меню
int8_t pointer = 0;

// Затримка між натисканнями кнопок
const unsigned long buttonDelay = 100; // Затримка між натисканнями кнопок (мс)

// Функція обробки натискання кнопок
void handleButtonPress(int id) {
  if (id == 0) { // Down
    pointer = constrain(pointer + 1, 0, ITEMS - 1);
  } else if (id == 1) { // Up
    pointer = constrain(pointer - 1, 0, ITEMS - 1);
  } else if (id == 2) { // Enter
    executeMenuFunction(pointer);
  } else if (id == 3) { // Back (нова кнопка)
    executeBack();
  }
  displayMenu();
}

// Функція виконання кнопки "Back"
void executeBack() {
  oled.clear();
  oled.home();
  oled.println(F("Back pressed"));
  oled.update();
  delay(500);
  displayMenu(); // Повернення до меню
}

// Функція обробки аналогових кнопок
void checkButtons() {
  int sensorValue = analogRead(analogPin); // Читання значення з аналогового піну

  for (int i = 0; i < 5; i++) {
    // Перевірка значення на відповідність кожному порогу з діапазоном
    if (sensorValue > (thresholds[i] - buttonRange) && sensorValue < (thresholds[i] + buttonRange)) {
      handleButtonPress(i);
      delay(buttonDelay); // Затримка після обробки кнопки
      break;
    }
  }
}

// Функція виводу меню на дисплей
void displayMenu() {
  oled.clear();
  oled.home();
  oled.println(F(" Menu:"));
  oled.println(F(" minitext"));
  oled.println(F(" music"));
  oled.println(F(" time"));
  oled.println(F(" games"));
  oled.println(F(" file system"));
  oled.println(F(" gyroscope"));
  oled.println(F(" settings"));
  oled.println(F(" exit"));
  printPointer(pointer);
  oled.update();
}

// Функція виводу вказівника
void printPointer(uint8_t pointer) {
  oled.setCursor(0, pointer + 1);
  oled.print(">");
}

// Функція виконання вибраного пункту меню
void executeMenuFunction(int8_t pointer) {
  oled.clear();
  oled.home();

  switch (pointer) {
    case 0:
      menuMinitext();
      break;
    case 1:
      menuMusic();
      break;
    case 2:
      menuTime();
      break;
    case 3:
      menuGames();
      break;
    case 4:
      menuFileSystem();
      break;
    case 5:
      menuGyroscope();
      break;
    case 6:
      menuSettings();
      break;
    case 7:
      menuExit();
      break;
  }

  oled.update();
  delay(1000); // Затримка після виконання функції меню (зменшена до 1000ms)
  displayMenu(); // Повернення до меню
}

// Шаблонні функції для пунктів меню
void menuMinitext() { oled.println(F("Minitext selected")); }
void menuMusic() { oled.println(F("Music selected")); }
void menuTime() { oled.println(F("Time selected")); }
void menuGames() { oled.println(F("Games selected")); }
void menuFileSystem() { oled.println(F("File system selected")); }
void menuGyroscope() { oled.println(F("Gyroscope selected")); }
void menuSettings() { oled.println(F("Settings selected")); }
void menuExit() {
  oled.clear();
  oled.home();
  oled.println(F("Exiting menu..."));
  oled.update();
}

// Налаштування
void setup() {
  oled.init();
  oled.setContrast(255);
  displayMenu();
}

// Основний цикл
void loop() {
  checkButtons(); // Перевірка кнопок
}

