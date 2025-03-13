#include <ESP8266WiFi.h>
#include <FastBot.h>

// Настройки Wi-Fi
const char* ssid = "DIR-1";         // Название вашей Wi-Fi сети
const char* password = "GL543jkl";  // Пароль от Wi-Fi сети

// Настройки Telegram-бота
#define CHAT_ID "00000"
#define BOT_TOKEN "ВАШ_TELEGRAM_BOT_TOKEN"  // Токен вашего бота

byte tries = 15;  // количество попыток подключения к сети

// Создаем объект бота
FastBot bot(BOT_TOKEN);

// Переменная для хранения времени последнего запроса
unsigned long lastRequestTime = 0;
const unsigned long requestInterval = 60000;  // Интервал 60 секунд

// обработка сообщений
void checkMsg(FB_msg& msg) {
}

// подключение к wifi
void connectWifi() {
  Serial.print("Connecting");
  while (--tries && WiFi.status() != WL_CONNECTED) {
    digitalWrite(LED_BUILTIN, 0);
    delay(250);
    digitalWrite(LED_BUILTIN, 1);
    Serial.print(".");
    delay(250);
  }
  tries = 15;
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Non Connecting to WiFi..");
    digitalWrite(LED_BUILTIN, 0);
    Serial.println("restart");
    delay(500);
    ESP.restart();
  } else {
    digitalWrite(LED_BUILTIN, 1);
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  }
}


void setup() {
  Serial.begin(115200);
  pinMode(A0, INPUT);
  WiFi.begin(ssid, password);
  pinMode(LED_BUILTIN, OUTPUT);
  delay(100);
  Serial.println();
  Serial.println();
  connectWifi();
  bot.attach(checkMsg);  // подключаем обработчик сообщений
}

void loop() {
  // Обработка входящих сообщений
  bot.tick();
}