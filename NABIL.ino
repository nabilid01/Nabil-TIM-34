#include <WiFi.h>
#include <HTTPClient.h>
#include "DHT.h"

#define DHTPIN 4     // Pin yang terhubung ke DHT11
#define DHTTYPE DHT11   // Tipe sensor DHT 11

const char* ssid = "WIFI ORANG KAYA";       // Ganti dengan nama SSID WiFi Anda
const char* password = "Xenonff01"; // Ganti dengan kata sandi WiFi Anda
const char* serverName = "http://192.168.203.50:5000/sensor/griffin"; // Ganti dengan alamat server lokal Anda

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  delay(10);

  // Menghubungkan ke Wi-Fi
  Serial.println();
  Serial.print("Menghubungkan ke ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi terhubung");
  Serial.println("Alamat IP: ");
  Serial.println(WiFi.localIP());

  dht.begin();
}

void loop() {
  if ((WiFi.status() == WL_CONNECTED)) { // Memeriksa apakah ESP32 terhubung ke Wi-Fi
    HTTPClient http;

    float h = dht.readHumidity();
    float t = dht.readTemperature();

    // Memeriksa apakah ada kesalahan dalam membaca sensor
    if (isnan(h) || isnan(t)) {
      Serial.println("Gagal membaca dari sensor DHT!");
      return;
    }
String timestampStr = String(millis());
// Membuat payload JSON
String postData = "{\"Ondo\":";
postData += String(t);
postData += ",\"Shitsudo\":";
postData += String(h);
postData += ",\"Jikanjunkan\":";
postData += timestampStr;
postData += "}";


    http.begin(serverName); // Memulai koneksi ke server
    http.addHeader("Content-Type", "application/json"); // Menentukan tipe konten sebagai JSON

    int httpResponseCode = http.POST(postData);

    if (httpResponseCode > 0) {
      String response = http.getString(); // Mendapatkan respons dari server
      Serial.println(httpResponseCode);   // Mencetak kode respons HTTP
      Serial.println(response);           // Mencetak respons dari server
    } else {
      Serial.print("Error pada pengiriman POST: ");
      Serial.println(httpResponseCode);
    }

    http.end(); // Mengakhiri koneksi

  } else {
    Serial.println("WiFi tidak terhubung");
  }

  delay(10000); // Menunggu 10 detik sebelum mengirim data lagi
}
