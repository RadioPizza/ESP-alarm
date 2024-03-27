// ESP-Alarm v0.1.0
// RX - Приёмник

// Подключение библиотек
#include <ESP8266WiFi.h>
#include <espnow.h>
#include <Blinker.h>

// Настройка пинов подключения
#define LED_PIN 2 // это D4 на Wemos

// Инициализация объектов
Blinker led(LED_PIN);

// Структура, принимаемая от передатчика
struct StructMessage
{
    bool myButtonClicked;
};

StructMessage myData;

// Функция тревоги
void alarm()
{
    // мигнуть 3 раза, 700мс вкл, 400мс выкл
    led.blink(3, 700, 400);
}

void setupWiFi()
{
    WiFi.mode(WIFI_STA);
    if (esp_now_init() != 0)
    {
        Serial.println("Error initializing ESP-NOW");
        return;
    }
    esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
    esp_now_register_recv_cb(OnDataRecv);
    Serial.println("ESP-NOW receiver initialized");
}

void OnDataRecv(uint8_t *mac, uint8_t *incomingData, uint8_t len)
{
    StructMessage *msg = (StructMessage *)incomingData;
    if (len == sizeof(StructMessage))
    {
        if (msg->myButtonClicked)
            alarm();
        if (msg->buttonPrev)
            triggerSlideChange(PREV_PIN);
    }
    else
    {
        Serial.println("Received data size does not match");
    }
}

void setup()
{
    Serial.begin(9600);
    setupWiFi();
}

void loop()
{
    // Основной цикл не используется, поскольку все действия происходят в OnDataRecv
    led.tick(); // тикер блинкера
}