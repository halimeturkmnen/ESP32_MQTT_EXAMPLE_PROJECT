# 📜 TLS Certificate for Secure MQTT Communication (`hivemq_cert.pem`)

This folder contains the `hivemq_cert.pem` file, a Root Certificate Authority (CA) certificate required to establish a secure connection between the ESP32 device and the [HiveMQ Cloud MQTT broker](https://www.hivemq.com/mqtt-cloud-broker/).

The certificate enables encrypted MQTT communication over TLS (`mqtts://`) and ensures that the ESP32 only connects to a trusted broker.

---

## 📌 Purpose

- **Encryption**: All communication between the ESP32 and the broker is encrypted to prevent eavesdropping.
- **Authentication**: The certificate allows the ESP32 to verify the identity of the broker.
- **Data Integrity**: Ensures that the messages are not tampered with in transit.

---

## 📥 How This Certificate Was Obtained

- **Broker Used**: [HiveMQ Cloud Free Cluster](https://www.hivemq.com/mqtt-cloud-broker/)
- **Root Certificate Authority**: [ISRG Root X1](https://letsencrypt.org/certificates/), issued by Let's Encrypt.

### Steps to Obtain:
1. After creating a free HiveMQ Cloud cluster, HiveMQ provides a broker endpoint like: your-broker-id.s1.eu.hivemq.cloud
2. This endpoint uses a certificate chain signed by the **ISRG Root X1** certificate.
3. The `ISRG Root X1` root certificate was:
- Downloaded from the [Let's Encrypt certificates page](https://letsencrypt.org/certificates/).
- Saved in `.pem` format as `hivemq_cert.pem`.
- Placed in this folder for inclusion in the ESP32 firmware.

> You can alternatively export the CA certificate from your web browser when visiting your broker URL over HTTPS and extracting the root from the certificate chain.

---

## 🧪 How to Validate This Certificate

You can inspect and verify the contents of `hivemq_cert.pem` using the OpenSSL command-line tool:

```bash
openssl x509 -in hivemq_cert.pem -text -noout
## ✅ Expected Output Highlights

### **Subject:** ISRG Root X1  
**Issuer:** ISRG Root X1  
**Validity:** From June 4, 2015 to June 4, 2035  
**Key Usage:** Certificate Signing, CRL Signing  

This confirms that the certificate is the official **ISRG Root X1**, trusted by modern systems and compatible with **HiveMQ**.

---
## 🧩 How It Is Used in the ESP32 Project

The `hivemq_cert.pem` file is embedded directly into the **ESP32 firmware** and used to establish a secure **TLS connection** to the MQTT broker.

### **Code Integration Example (ESP-IDF):**

```c
extern const uint8_t hivemq_cert_pem_start[] asm("_binary_hivemq_cert_pem_start");
extern const uint8_t hivemq_cert_pem_end[]   asm("_binary_hivemq_cert_pem_end");

esp_mqtt_client_config_t mqtt_cfg = {
    .uri = "mqtts://your-broker.hivemq.cloud",
    .cert_pem = (const char *) hivemq_cert_pem_start,
    ...
};

esp_mqtt_client_handle_t client = esp_mqtt_client_init(&mqtt_cfg);
esp_mqtt_client_start(client);

# Build System Note:
To include the certificate in the final firmware image, it is referenced in `CMakeLists.txt` using `embed_txtfiles` or the equivalent `idf_component_register()` directive.

---

## 🛡️ Security Notes
- This is a public root certificate — safe to embed in firmware.
- **Do not include private keys here.**
- Replace the certificate if:
  - The HiveMQ broker changes its certificate chain.
  - The current CA is compromised.
  - You are using a different broker or CA.
- Always ensure the broker's server certificate chains up to this root.

---

## 🔄 How to Update This Certificate (Optional)
1. Visit the Let's Encrypt root certificates page:  
   👉 [https://letsencrypt.org/certificates/](https://letsencrypt.org/certificates/)
2. Download the latest **ISRG Root X1** `.pem` file.
3. Replace the existing `hivemq_cert.pem` file with the updated one.
4. Clean and rebuild your firmware:
   ```bash
   idf.py fullclean
   idf.py build flash monitor

## ✅ References
- [HiveMQ Cloud Documentation](https://www.hivemq.com/docs/cloud/)
- [Let's Encrypt Root Certificates](https://letsencrypt.org/certificates/)
- [ESP-IDF TLS/SSL Docs](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/protocols/esp_tls.html)
   

