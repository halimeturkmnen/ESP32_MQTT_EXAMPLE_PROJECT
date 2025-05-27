# main/CMakeLists.txt

# 1) Tek component kaydı
idf_component_register(
    SRCS "main.c"
    INCLUDE_DIRS "."
    REQUIRES
        driver
        led_strip
        mqtt          # ← Eski mqtt_client yerine "mqtt"
        esp_wifi
        esp_event
        esp_netif
        nvs_flash
        esp_http_client  # Eğer HTTP client kullanıyorsanız
)

# 2) Sertifika dosyasını ikili veri olarak ekle
target_add_binary_data(${COMPONENT_LIB}
    "hivemq_cert.pem"
    TEXT
)
