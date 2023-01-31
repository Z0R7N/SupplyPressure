#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

const char* ssid = "DIR-3";
// const char* password =  "3!r@fdQX";
const char* password =  "GL543jkl";
String GAS_ID = "your gas id";
const char* host = "script.google.com";
const int httpsPort = 443;
WiFiClientSecure client;

byte tries = 15;


bool sensor(){
	digitalWrite(LED_BUILTIN, 1);
	// digitalWrite(pinPower, 1);
	delay(500);
	// check for connection
	bool connectSns = true;

	

	return connectSns;
}

// connect to wifi net
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


// record data to google table
void sendData(float barPres) {
	digitalWrite(LED_BUILTIN, 1);
	if (!client.connect(host, httpsPort)) {
		Serial.println("connection failed");
		return;
	}
	
	String string_pressure =  String(barPres);
	String url = "/macros/s/" + GAS_ID + "/exec?press=" + string_pressure;
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

// calculating method
float calculating(int maxNum, int minNum, int dt, int minBar){
	int different = dt - minNum;
	float result = (float) different / (maxNum - minNum);
	return (float) result + minBar;
}

// calculating data from manometer to bar
float calculateBar(int dt){
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
	}
	else if(dt > dt1 and dt <= dt2){
		res = calculating(dt2, dt1, dt, 1);
	}
	else if(dt > dt2 and dt <= dt3){
		res = calculating(dt3, dt2, dt, 2);
	}
	else if(dt > dt3 and dt <= dt4){
		res = calculating(dt4, dt3, dt, 3);
	}
	else if(dt > dt4 and dt <= dt5){
		res = calculating(dt5, dt4, dt, 4);
	}
	else if(dt > dt5 and dt <= dt6){
		res = calculating(dt6, dt5, dt, 5);
	}
	return res;
}

// average value calculation
int middleData(){
	int cnt = 0;
	int maxCnt = 0;
	int dataPres = 0;
	int res = -1;
	long sumPress;
	while(maxCnt < 20){
		maxCnt++;
		dataPres = analogRead(A0);
		if (dataPres >= 106 and dataPres <= 988) {
			sumPress += dataPres;
			cnt++;
		}
		if (cnt = 6) {
			res = round (sumPress / cnt);
			break;
		}
	}
	return res;
}


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
	bool correctData = sensor();
	digitalWrite(LED_BUILTIN, 0);
	delay(20);
	delay(1000);
	bool cnct = checkConnection();
	
	// if (!cnct) {
		// digitalWrite(LED_BUILTIN, 1);
		// connectWifi();
	// } else {
		if (correctData){
			digitalWrite(LED_BUILTIN, 0);
			float midBar = calculateBar(middleData());
			Serial.println();
			Serial.print("Pressure: ");
			Serial.println(midBar);
			Serial.println();
			
			
			// sendData(br);
			
			// // ESP.deepSleep(935e6); // every 15 min
			// ESP.deepSleep(15e6);  // every 15 sec
		}
	// }
	
	
}