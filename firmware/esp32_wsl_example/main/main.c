/**
 * @file main.c
 * @brief ESP32 MQTT example with WS2812 LED strip control
 * 
 * This program connects to Wi-Fi, establishes a secure MQTT connection,
 * subscribes to a topic to control LED colors, and publishes simulated sensor data.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "mqtt_client.h"
#include "driver/gpio.h"

#include "led_strip.h"    /**< WS2812 LED strip driver */
#include "driver/rmt.h"   

/** Wi-Fi SSID */
#define WIFI_SSID "HalimeiPhone"
/** Wi-Fi Password */
#define WIFI_PASS "halime.1234"

//#define RELAY_TEMP_PIN (14) /**< Relay pin, currently unused */

#define LED_GPIO 8   /**< GPIO pin connected to LED strip */
#define LED_NUM  1   /**< Number of LEDs in the strip */

/** MQTT Broker URI */
#define MQTT_BROKER_URI "mqtts://7ab1e9bf6329443886532525b88adc36.s1.eu.hivemq.cloud"
/** MQTT username */
#define MQTT_USERNAME   "esp32hivemq"
/** MQTT password */
#define MQTT_PASSWORD   "12345678hH"

static const char *TAG = "MQTT_APP";  /**< Logging tag */

static esp_mqtt_client_handle_t mqtt_client;   /**< MQTT client handle */
static led_strip_handle_t led_strip_handle = NULL;  /**< LED strip driver handle */

extern const uint8_t hivemq_cert_pem_start[] asm("_binary_hivemq_cert_pem_start"); /**< HiveMQ TLS certificate start */
extern const uint8_t hivemq_cert_pem_end[]   asm("_binary_hivemq_cert_pem_end");   /**< HiveMQ TLS certificate end */

/**
 * @brief MQTT event handler callback
 * 
 * Handles MQTT connection, incoming messages, and errors.
 * @param handler_args User data (unused)
 * @param base Event base
 * @param event_id Event ID
 * @param event_data Event data pointer
 */
static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data) {
    esp_mqtt_event_handle_t event = event_data;

    switch (event_id) {
        case MQTT_EVENT_CONNECTED:
            ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");
            esp_mqtt_client_subscribe(mqtt_client, "home/temperature", 0);
            break;

        case MQTT_EVENT_DATA:
            ESP_LOGI(TAG, "Message received on topic: %.*s, Message: %.*s",
                     event->topic_len, event->topic, event->data_len, event->data);

            if (strncmp(event->topic, "home/temperature", event->topic_len) == 0) {
                
                if (strncmp(event->data, "increase", event->data_len) == 0) {
                    
                    led_strip_set_pixel(led_strip_handle, 0, 0, 255, 0);
                    led_strip_refresh(led_strip_handle);
                    ESP_LOGI(TAG, "LED color: GREEN (increase)");
                }
                else if (strncmp(event->data, "decrease", event->data_len) == 0) {
                    
                    led_strip_set_pixel(led_strip_handle, 0, 255, 0, 0);
                    led_strip_refresh(led_strip_handle);
                    ESP_LOGI(TAG, "LED color: RED (decrease)");
                }
                else {
                    led_strip_set_pixel(led_strip_handle, 0, 0, 0, 255); // Blue
                    led_strip_refresh(led_strip_handle);
                    ESP_LOGW(TAG, "Unknown message: %.*s", event->data_len, event->data);
                }
                
            }

            break;

        case MQTT_EVENT_ERROR:
            ESP_LOGE(TAG, "MQTT_EVENT_ERROR");
            break;

        default:
            break;
    }
}

/**
 * @brief Wi-Fi event handler callback
 * 
 * Handles Wi-Fi events such as connection, disconnection, and IP acquisition.
 * Initiates MQTT connection once IP is obtained.
 * 
 * @param arg User data (unused)
 * @param event_base Event base
 * @param event_id Event ID
 * @param event_data Event data pointer
 */
static void wifi_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data) {
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
        esp_wifi_connect();
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
        ESP_LOGI(TAG, "Disconnected from Wi-Fi, reconnecting...");
        esp_wifi_connect();
    } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        ESP_LOGI(TAG, "Got IP, connecting to MQTT...");

        esp_mqtt_client_config_t mqtt_cfg = {
            .broker = {
                .address.uri = MQTT_BROKER_URI,
                .verification = {
                    .certificate = (const char *)hivemq_cert_pem_start,
                }
            },
            .credentials = {
                .username = MQTT_USERNAME,
                .authentication = {
                    .password = MQTT_PASSWORD,
                }
            }
        };

        mqtt_client = esp_mqtt_client_init(&mqtt_cfg);
        esp_mqtt_client_register_event(mqtt_client, ESP_EVENT_ANY_ID, mqtt_event_handler, NULL);
        esp_mqtt_client_start(mqtt_client);
    }
}

/**
 * @brief Initialize Wi-Fi in station mode and register event handlers
 */
static void wifi_init(void) {
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_sta();

    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler, NULL, NULL));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &wifi_event_handler, NULL, NULL));

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    wifi_config_t wifi_config = {
        .sta = {
            .ssid = WIFI_SSID,
            .password = WIFI_PASS,
        },
    };

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());
}

/**
 * @brief Main application entry point
 * 
 * Initializes NVS, Wi-Fi, MQTT, and LED strip driver.
 * Publishes random sensor data every 5 seconds.
 */
void app_main(void) {
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    //gpio_set_direction(RELAY_TEMP_PIN, GPIO_MODE_OUTPUT);
    //gpio_set_level(RELAY_TEMP_PIN, 0);

    led_strip_config_t strip_config = {
        .strip_gpio_num = LED_GPIO,
        .max_leds = LED_NUM,
        .led_model = LED_MODEL_WS2812,
        .color_component_format = LED_STRIP_COLOR_COMPONENT_FMT_GRB,
        .flags = {
            .invert_out = false,
        }
    };
    led_strip_rmt_config_t rmt_config = {
        .clk_src = RMT_CLK_SRC_DEFAULT,
        .resolution_hz = 10 * 1000 * 1000,
        .flags = {
            .with_dma = false,
        },
    };
    ESP_ERROR_CHECK(led_strip_new_rmt_device(&strip_config, &rmt_config, &led_strip_handle));
    
    led_strip_clear(led_strip_handle);
    ESP_LOGI(TAG, "WS2812 LED driver is started");
   
    wifi_init();

    while (1) {
        int temperature = rand() % 51 + 10;
        int humidity    = rand() % 101;
        int light       = rand() % 101;

        char buf[16];
        snprintf(buf, sizeof(buf), "%d", temperature);
        esp_mqtt_client_publish(mqtt_client, "esp/in/temperature", buf, 0, 1, 0);

        snprintf(buf, sizeof(buf), "%d", humidity);
        esp_mqtt_client_publish(mqtt_client, "esp/in/humidity", buf, 0, 1, 0);

        snprintf(buf, sizeof(buf), "%d", light);
        esp_mqtt_client_publish(mqtt_client, "esp/in/light", buf, 0, 1, 0);

        ESP_LOGI(TAG, "Temp: %d | Humidity: %d | Light: %d", temperature, humidity, light);

        vTaskDelay(5000 / portTICK_PERIOD_MS);
    }
}
