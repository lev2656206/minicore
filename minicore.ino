#include <AnalogButtons.h>
#define OLED_SOFT_BUFFER_64     // Буфер на стороні МК
#include <GyverOLED.h>          // Бібліотека дисплея
GyverOLED<SSD1306_128x64, OLED_BUFFER> oled;

#define ITEMS 8               // Кількість пунктів меню
#define analogPin A0          // Аналоговий пін для кнопок

// Порогові значення для кнопок
int thresholds[] = { 4095, 1790, 800, 1195, 2695 };  // Down, Up, Enter, Back, Right

AnalogButtons buttons(analogPin, INPUT, 5, 10);  // Налаштування AnalogButtons

// Змінні для меню
int8_t pointer = 0;  // Індекс вибраного пункту меню
unsigned long lastBackPressTime = 0; // Час останнього натискання кнопки "Back"
bool waitingForDoublePress = false; // Чи очікується друге натискання
const unsigned long doublePressInterval = 500; // Інтервал для подвійного натискання (мс)

// Функція обробки натискання кнопок
void handleButtonPress(int id) {
  if (id == 0) { // Down
    pointer = constrain(pointer + 1, 0, ITEMS - 1);
  } else if (id == 1) { // Up
    pointer = constrain(pointer - 1, 0, ITEMS - 1);
  } else if (id == 2) { // Enter
    executeMenuFunction(pointer); // Виклик функції для вибраного пункту
  } else if (id == 3) { // Back (ліво)
    handleBackPress();
  }

  // Оновлення меню
  displayMenu();
}

// Логіка для обробки подвійного натискання кнопки "Back"
void handleBackPress() {
  unsigned long currentTime = millis();
  
  if (waitingForDoublePress && (currentTime - lastBackPressTime < doublePressInterval)) {
    // Якщо друге натискання в межах інтервалу
    menuExit();
    waitingForDoublePress = false; // Скидання очікування
  } else {
    // Інакше стартуємо таймер для подвійного натискання
    waitingForDoublePress = true;
    lastBackPressTime = currentTime;
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
  oled.setCursor(0, pointer + 1);  // Вивід стрілочки біля пункту
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
  delay(2000);
  displayMenu(); // Повернення до меню
}

// Шаблонні функції для пунктів меню
void menuMinitext() {
  oled.println(F("Minitext selected"));
}

void menuMusic() {
  oled.println(F("Music selected"));
}

void menuTime() {
  oled.println(F("Time selected"));
}

void menuGames() {
  oled.println(F("Games selected"));
}

void menuFileSystem() {
  oled.println(F("File system selected"));
}

void menuGyroscope() {
  oled.println(F("Gyroscope selected"));
}

void menuSettings() {
  oled.println(F("Settings selected"));
}

void menuExit() {
  oled.clear();
  oled.home();
  oled.println(F("Exiting menu..."));
  oled.update();
  delay(1000);
  // Додай свою логіку для виходу
}

// Налаштування
void setup() {
  oled.init();           // Ініціалізація дисплея
  oled.setContrast(255); // Максимальна яскравість

  // Додавання кнопок
  buttons.add(Button(thresholds[0], []() { handleButtonPress(0); })); // Down
  buttons.add(Button(thresholds[1], []() { handleButtonPress(1); })); // Up
  buttons.add(Button(thresholds[2], []() { handleButtonPress(2); })); // Enter
  buttons.add(Button(thresholds[3], []() { handleButtonPress(3); })); // Back
  buttons.add(Button(thresholds[4], []() { handleButtonPress(4); })); // Right

  displayMenu(); // Відображення меню при старті
}

// Основний цикл
void loop() {
  buttons.check(); // Перевірка кнопок

  // Автоматичне скидання очікування, якщо інтервал вийшов
  if (waitingForDoublePress && millis() - lastBackPressTime >= doublePressInterval) {
    waitingForDoublePress = false;
  }
}

