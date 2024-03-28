// ESP-Alarm v0.1.1
// TX - Передатчик

// Подключение библиотек
#include <ESP8266WiFi.h>
#include <espnow.h>
#include <EncButton.h>

// Настройка пинов подключения
#define BTN_PIN 14 // это D5 на Wemos

// Инициализация объектов
Button myButton(BTN_PIN);

// MAC адрес приёмника
uint8_t broadcastAddress[] = {0x48, 0x55, 0x19, 0xDE, 0xB3, 0x6B};

// Структура для отправки данных
struct StructMessage
{
    bool myButtonClicked;
};

StructMessage myData;

void setupWiFi()
{
    WiFi.mode(WIFI_STA);
    if (esp_now_init() != 0)
    {
        Serial.println("Error initializing ESP-NOW");
        return;
    }
    esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
    esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
    Serial.println("WiFi Setup completed successfully");
}

void setup()
{
    Serial.begin(9600);
    setupWiFi();
}

void loop()
{
    myButton.tick();
    myData.myButtonClicked = myButton.click();
    if (myData.myButtonClicked)
    {
        Serial.println("Button was clicked");
        if (esp_now_send(broadcastAddress, (uint8_t *)&myData, sizeof(myData)) != 0)
        {
            Serial.println("Error sending the data");
        }
    }
}