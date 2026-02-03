#include <DHT11.h>
#include <DHT.h>
#include <ArduinoJson.h>
#include <CTBot.h>

CTBot myBot;

String ssid = "KOS PUTRA 2";
String pass = "1sampai8 ";
String token = "6721362417:AAG_rye-mN9O-kvg0O7vWqCM8eNl46R6io8";

#define Api 5
#define Gas 14
#define DHTTYPE DHT11
#define relay 13
#define SPRINKLER_START_DELAY 1000  //5 seconds 
#define SPRINKLER_ON_TIME 2000
DHT dht(21, DHT11); 

bool pesanKesalahanDikirim = false;
unsigned long previousMillis = 0;
unsigned long previousTime = millis();
const long interval = 300000; // Waktu interval dalam milidetik (5 menit)

void setup() {
  pinMode(Api, INPUT);
  pinMode(Gas, INPUT);
  pinMode(relay, OUTPUT);
  dht.begin();
  Serial.begin(115200);
  myBot.wifiConnect(ssid, pass);
  myBot.setTelegramToken(token);

  if (myBot.testConnection())
    Serial.println("\n Terhubung");
  else
    Serial.println("\n Tidak Terhubung");
}

void loop() {
  int bacasensorapi = digitalRead(Api);
  int bacasensorgas = digitalRead(Gas);
  float humidity, temperature;
  temperature = dht.readTemperature();
  humidity = dht.readHumidity();

  unsigned long currentMillis = millis();

  Serial.print("Api : ");
  Serial.print(bacasensorapi);
  Serial.print("   Gas : ");
  Serial.println(bacasensorgas);

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    if (!isnan(humidity) && !isnan(temperature)) {
      String kirim;
      kirim = "Humidity: " + String(humidity) + "%\nTemperature: " + String(temperature) + "°C";
      myBot.sendMessage(1350744048, kirim);
    }
  }

  if (bacasensorgas == 0) // terdeteksi gas
  {
    String kirim;
    kirim = "Lapor Bos.! Ada Kebocoran Gas";
    myBot.sendMessage(1350744048, kirim);
    delay(500);
  }

  if (bacasensorapi == 0) // terdeteksi api
  {
    String kirim;
    kirim = "Lapor bos.! Ada Kebakaran";
    myBot.sendMessage(1350744048, kirim);
    delay(500);
    if (millis() - previousTime > SPRINKLER_START_DELAY)  
    {
      digitalWrite(relay, LOW);                       
      delay(SPRINKLER_ON_TIME);                           
    }
  }
  else
  {    
    digitalWrite(relay, HIGH); 
    previousTime = millis();  
  }
  delay(500);
}
