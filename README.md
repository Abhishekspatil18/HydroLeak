# 💧 HydroLeak

> **An IoT-Based Smart Water Pipeline Leakage Detection and Management System**

---

## 📌 About the Project

HydroLeak is an IoT-based Smart Water Pipeline Leakage Detection and Management System developed to reduce water wastage through real-time monitoring and automated leak detection. The system uses an ESP32 microcontroller, water flow sensor, GPS module, relay, buzzer, and the Blynk IoT platform to continuously monitor water flow in pipelines.

When abnormal water flow indicating a leakage is detected, HydroLeak automatically turns off the water motor, activates a buzzer, sends an email notification, and provides the GPS location of the leakage. The solution is suitable for residential, agricultural, and municipal water distribution systems, promoting efficient water management and conservation.

---

## 🚀 Features

- 🔹 Real-Time Water Flow Monitoring
- 🔹 Automatic Leakage Detection
- 🔹 Automatic Motor Shutdown
- 🔹 GPS-Based Leakage Location
- 🔹 Email Notification Alerts
- 🔹 Buzzer Alert System
- 🔹 Live Monitoring through Blynk
- 🔹 Low-Cost IoT Solution
- 🔹 Suitable for Rural & Urban Water Supply

---

## 📷 Circuit Diagram

<p align="center">
  <img src="Image/circuit_diagram.jpg" alt="HydroLeak Circuit Diagram" width="800">
</p>

The above circuit diagram illustrates the hardware connections between the ESP32, Water Flow Sensor, GPS Module, Relay Module, Water Pump, and Buzzer used in the HydroLeak system.

---

## 💻 Hardware Components

| Component | Description |
|-----------|-------------|
| ESP32 | Main controller with built-in Wi-Fi |
| Water Flow Sensor | Measures water flow rate |
| Neo-6M GPS Module | Provides leakage location |
| Relay Module | Controls water motor automatically |
| Water Pump | Supplies water |
| Buzzer | Generates leakage alert |
| Power Supply | Powers the complete system |

---

## 🛠 Software & Technologies

| Technology | Purpose |
|------------|---------|
| Arduino IDE | Programming ESP32 |
| Embedded C/C++ | Firmware Development |
| ESP32 Libraries | Wi-Fi Communication |
| Blynk IoT | Remote Monitoring |
| TinyGPS++ | GPS Data Processing |
| ESP Mail Client | Email Notification |
| Git & GitHub | Version Control |

---

## 📂 Project Structure

```text
HydroLeak
│
├── Image
│   └── circuit_diagram.jpg
│
├── Water_lekage_Blynk.ino
├── water_lekage_mail.ino
└── README.md
```

---

## ⚙️ Installation

### Clone the Repository

```bash
git clone https://github.com/Abhishekspatil18/HydroLeak.git
```

### Open the Project

Open the `.ino` files in **Arduino IDE**.

### Install Required Libraries

- Blynk
- TinyGPS++
- WiFi
- HTTPClient
- ESP Mail Client

### Configure Credentials

Update the following credentials in the code:

```cpp
char ssid[] = "YOUR_WIFI_NAME";
char pass[] = "YOUR_WIFI_PASSWORD";
char auth[] = "YOUR_BLYNK_AUTH_TOKEN";
```

Configure your email credentials if email alerts are enabled.

### Upload the Code

1. Connect the ESP32.
2. Select the correct COM Port.
3. Click **Upload** in Arduino IDE.

---

## 🔄 Working Principle

1. Water flows through the pipeline.
2. The flow sensor continuously measures the flow rate.
3. ESP32 processes the sensor readings.
4. If leakage is detected:
   - Relay switches OFF the motor.
   - Buzzer is activated.
   - Email notification is sent.
   - GPS location is obtained.
   - Blynk dashboard is updated.
5. The user can monitor the system remotely through the Blynk application.

---

## 🎯 Project Objectives

- Detect pipeline leakage automatically.
- Reduce water wastage.
- Provide real-time alerts.
- Enable remote monitoring.
- Improve water management efficiency.
- Support sustainable water conservation.

---

## 📊 Applications

- Smart Cities
- Municipal Water Distribution
- Agricultural Irrigation
- Residential Water Supply
- Industrial Water Monitoring

---

## 🔮 Future Enhancements

- AI-Based Leakage Prediction
- Cloud Data Storage
- Mobile Application
- LoRa Communication
- Multiple Sensor Integration
- Water Usage Analytics
- Smart City Integration

---

## 🤝 Contributing

Contributions are welcome!

1. Fork the repository.
2. Create a new branch.
3. Commit your changes.
4. Push the branch.
5. Create a Pull Request.

---

## 📄 License

This project is developed for educational and research purposes.

---

## 👨‍💻 Developed By

**Abhishek Patil**

GitHub: **https://github.com/Abhishekspatil18**

⭐ If you found this project useful, consider giving it a **Star**!
