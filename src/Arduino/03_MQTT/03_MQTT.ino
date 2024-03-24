const char* client_id = "station-XX";      // zde je treba zmenit CLIENT_ID (1-5);
const char* ssid = "RUT240_5F92";
const char* wifi_password = "Password1";
const char* mqtt_server = "192.168.2.1";

#define PIN 19

#include <WiFi.h>
#include <PubSubClient.h>

WiFiClient espClient;
PubSubClient client(espClient);
int prev_button = -1;

void setup() {
  Serial.begin(115200);
  pinMode(PIN, INPUT_PULLUP);

  setup_wifi();
  setup_mqtt();
}

void loop() {
  int button = digitalRead(PIN) ? 0 : 1;
  if (button != prev_button) {
    Serial.printf("Tlaciko = %d\n", button);
    client.publish(client_id, button ? "1" : "0");
    prev_button = button;
  }

  delay(100);
}

void setup_wifi() {

  Serial.println();
  Serial.print("Pripojuji se k ");
  Serial.println(ssid);

  WiFi.begin(ssid, wifi_password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print("CLIENT_ID");
  }

  Serial.println("");
  Serial.println("WiFi pripojena");
  Serial.println("IP adresa: ");
  Serial.println(WiFi.localIP());
}

void setup_mqtt() {
  client.setServer(mqtt_server, 1883);

  while (!client.connected()) {
    Serial.print("Pripojuji k MQTT...");
    if (client.connect("ESP32_Client")) {
      Serial.println("pripojeno");
    } else {
      Serial.print("chyba, rc=");
      Serial.print(client.state());
      Serial.println(" 5 sekund...");

      delay(5000);
    }
  }
}
