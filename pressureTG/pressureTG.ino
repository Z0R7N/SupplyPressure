#include <ESP8266WiFi.h>
#include <FastBot.h>

// Настройки Wi-Fi
const char* ssid = "name";          // Название вашей Wi-Fi сети
const char* password = "password";  // Пароль от Wi-Fi сети

// Настройки Telegram-бота
#define BOT_TOKEN "ВАШ_TELEGRAM_BOT_TOKEN"  // Токен вашего бота

byte tries = 15;                             // количество попыток подключения к сети
unsigned long lastCheckTime = 0;             // Переменная для хранения времени последней проверки
const unsigned long checkInterval = 300000;  // Интервал проверки (5 минут)

// Создаем объект бота
FastBot bot(BOT_TOKEN);

// Переменная для хранения времени последнего запроса
unsigned long lastRequestTime = 0;
const unsigned long requestInterval = 60000;  // Интервал 60 секунд

// проверка работает ли манометр
bool sensor() {
  digitalWrite(LED_BUILTIN, 1);
  // digitalWrite(pinPower, 1);
  delay(500);
  // check for connection
  bool connectSns = true;
  if (analogRead(A0) == 0) {
    connectSns = false;
    Serial.print("manometr is not connect, data: ");
    Serial.println(analogRead(A0));
  }
  return connectSns;
}

// дешифровка данных с манометра
float calculating(int maxNum, int minNum, int dt, int minBar) {
  int different = dt - minNum;
  float result = (float)different / (maxNum - minNum);
  return (float)result + minBar;
}

// конвертация данных с манометра
float calculateBar(int dt) {
  Serial.print("data: ");
  Serial.println(dt);
  // data for calculating bar from manometer data
  int dt0 = 106;
  int dt1 = 206;
  int dt2 = 365;
  int dt3 = 540;
  int dt4 = 700;
  int dt5 = 855;
  int dt6 = 988;
  float res = -1;
  res = dt > dt6 ? 7 : res;
  res = dt < dt0 ? 0 : res;
  if (dt >= dt0 and dt <= dt1) {
    res = calculating(dt1, dt0, dt, 0);
  } else if (dt > dt1 and dt <= dt2) {
    res = calculating(dt2, dt1, dt, 1);
  } else if (dt > dt2 and dt <= dt3) {
    res = calculating(dt3, dt2, dt, 2);
  } else if (dt > dt3 and dt <= dt4) {
    res = calculating(dt4, dt3, dt, 3);
  } else if (dt > dt4 and dt <= dt5) {
    res = calculating(dt5, dt4, dt, 4);
  } else if (dt > dt5 and dt <= dt6) {
    res = calculating(dt6, dt5, dt, 5);
  }
  return res;
}

// вычисление среднего значения
int middleData() {
  int cnt = 0;
  int maxCnt = 0;
  int dataPres = 0;
  int res = -1;
  long sumPress = 0;
  while (maxCnt < 20) {
    maxCnt++;
    dataPres = analogRead(A0);
    if (dataPres >= 106 and dataPres <= 988) {
      sumPress += dataPres;
      cnt++;
    }
    if (cnt == 6) {
      res = round(sumPress / cnt);
      break;
    }
  }
  return res;
}

// отправка данных пользователю
void sendData(String IDUser) {
  digitalWrite(LED_BUILTIN, 0);
  String presss = "манометр не работает 🤔";
  bool correctData = sensor();
  float midBar = 0;
  digitalWrite(LED_BUILTIN, 1);
  delay(20);

  if (correctData) {
    digitalWrite(LED_BUILTIN, 0);
    midBar = calculateBar(middleData());
    Serial.println();
    Serial.print("Pressure: ");
    Serial.println(midBar);
    Serial.println();
  }

  if (midBar != 0) presss = String(midBar);

  bot.sendMessage(presss, IDUser);
  Serial.println("message sent");
  Serial.println();
  digitalWrite(LED_BUILTIN, 1);
}

// обработка сообщений
void checkMsg(FB_msg& msg) {
  String uId = msg.userID;
  bot.deleteMessage(msg.messageID, uId);
  String msgOfUser = msg.text;
  // test01--
  Serial.print("сообщение: ");
  Serial.println(msgOfUser);
  // Serial.print(msg.toString());
  // --
  sendData(uId);
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
  // Проверка соединения раз в 5 минут
  if (millis() - lastCheckTime >= checkInterval) {
    lastCheckTime = millis();
    if (WiFi.status() != WL_CONNECTED) {
      Serial.println("Lost WiFi connection, reconnecting...");
      connectWifi();
    }
  }
  // Обработка входящих сообщений
  bot.tick();
}