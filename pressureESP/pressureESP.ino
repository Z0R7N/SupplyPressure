#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

const char* ssid = "DIR-3";
// const char* password =  "3!r@fdQX";
const char* password =  "GL543jkl";
String GAS_ID = "your code for google sheets";
const char* host = "script.google.com";
const int httpsPort = 443;
WiFiClientSecure client;

byte tries = 15;


void setup() {
	Serial.begin(115200);
	pinMode(A0, INPUT);
	WiFi.begin(ssid, password);
	pinMode(LED_BUILTIN, OUTPUT);

	delay(100);
	Serial.println();
	Serial.println();
	// connectWifi();
  
}

void loop() {
	// bool correctData = sensor();
	// digitalWrite(LED_BUILTIN, 0);
	delay(20);
	delay(500);
	// bool cnct = checkConnection();
	
	// if (!cnct) {
		// digitalWrite(LED_BUILTIN, 1);
		// connectWifi();
	// } else {
		// if (correctData){
			// digitalWrite(LED_BUILTIN, 0);
			Serial.println();
			Serial.print("Pressure: ");
			Serial.println(calculateBar(digitalRead(A0)));
			Serial.println();
			
			// sendData();
			
			// // ESP.deepSleep(935e6); // every 15 min
			// ESP.deepSleep(15e6);  // every 15 sec
		// }
	// }
	
	
}

bool sensor(){
	digitalWrite(LED_BUILTIN, 1);
	digitalWrite(pinPower, 1);
	delay(2000);
	// check for connection
	bool connectSns = true;

	

	return connectSns;
}

void connectWifi(){
	Serial.print("Connecting");
	while (--tries && WiFi.status() != WL_CONNECTED) {
		digitalWrite(LED_BUILTIN, 1);
		delay(250);
		digitalWrite(LED_BUILTIN, 0);
		Serial.print(".");
		delay(250);
	}
	tries = 15;
	if (WiFi.status() != WL_CONNECTED) {
		Serial.println("Non Connecting to WiFi..");
		digitalWrite(LED_BUILTIN, 1);
	} else {
		digitalWrite(LED_BUILTIN, 0);
		Serial.println("");
		Serial.println("WiFi connected");
		Serial.print("IP address: ");
		Serial.println(WiFi.localIP());
		client.setInsecure();
	}
}

bool checkConnection(){
	return WiFi.status() == WL_CONNECTED;
}

void sendData() {
	digitalWrite(LED_BUILTIN, 1);
	if (!client.connect(host, httpsPort)) {
		Serial.println("connection failed");
		return;
	}
	
	String string_pressure =  String(pressure);
	String url = "/macros/s/" + GAS_ID + "/exec?pressure=" + string_pressure;
	// Serial.print("requesting URL: ");
	// Serial.println(url);

	client.print(String("GET ") + url + " HTTP/1.1\r\n" +
	 "Host: " + host + "\r\n" +
	 "User-Agent: BuildFailureDetectorESP8266\r\n" +
	 "Connection: close\r\n\r\n");

	Serial.println("request sent");
	
	
	// while (client.connected()) {
		// String line = client.readStringUntil('\n');
		// if (line == "\r") {
			// Serial.println("headers received");
			// break;
		// }
	// }
	// String line = client.readStringUntil('\n');
	// if (line.startsWith("{\"state\":\"success\"")) {
		// Serial.println("esp8266/Arduino CI successfull!");
	// } else {
		// Serial.println("esp8266/Arduino CI has failed");
	// }
	// Serial.print("reply was : ");
	// Serial.println(line);
	// Serial.println("closing connection");
	// Serial.println("==========");
	Serial.println();
	digitalWrite(LED_BUILTIN, 0);
}

float calculateBar(int dt){
	// y=−3.1667x3+29.7143x2+83.7381x+114.8571 из bar в данные
	// y=−0.0000x2+0.0062x−0.3512    из данных в bar
	return (float)-0.0000 * (dt * dt) + 0.0062 * dt - 0.3512;
}