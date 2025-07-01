#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include <ESP32Servo.h>

Servo myServo;
const int pinServo = 14;

#define TRIG_PIN 5    // متصل بـ Trig في HC-SR04
#define ECHO_PIN 18   // مستشعر المسافة Echo
#define BUZZER_PIN 21 // متصل بالبازر
#define LED_PIN 22    // متصل بالـ LED الحمراء

/* 1. Wi-Fi Credentials */
#define WIFI_SSID "Fibre_MarocTelecom-2A34"
#define WIFI_PASSWORD "Mr6M6LK2qi"

/* 2. Firebase API Key */
#define API_KEY "AIzaSyCQ1FoNhkuH3TRDUXxGIf4knSNQ-_JVYoU"

/* 3. Firebase Database URL */
#define DATABASE_URL "https://esp32wroom-30937-default-rtdb.firebaseio.com/"

/* 4. Firebase User Credentials */
#define USER_EMAIL "moha@admin.com"
#define USER_PASSWORD "123123"

// Firebase objects
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

// Timer for loop execution
unsigned long sendDataPrevMillis = 0;
const int WhiteLed1 = 23; // LED pin
const int WhiteLed2 = 19; // LED pin
const int WhiteLed3 = 4; // LED pin

void setup() {

    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
    pinMode(BUZZER_PIN, OUTPUT);
    pinMode(LED_PIN, OUTPUT);  // تعيين الـ LED كمخرج


    myServo.attach(pinServo);
    // Start Serial Monitor
    Serial.begin(115200);
    delay(1000);

    Serial.println("\n--------------------------------");
    Serial.println("🚀 ESP32 Startup...");
    Serial.println("--------------------------------\n");

    // Step 1: Connect to Wi-Fi
    Serial.println("🔄 Connecting to Wi-Fi...\n");
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 20) {
        delay(500);
        Serial.print(".");
        attempts++;
    }

    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("\n✅ Wi-Fi Connected!");
        Serial.print("📶 IP Address: ");
        Serial.println(WiFi.localIP());
        Serial.println();
    } else {
        Serial.println("\n❌ Failed to connect to Wi-Fi.");
        Serial.println("🔄 Restarting ESP32...");
        delay(5000);
        ESP.restart();
    }

    // Step 2: Configure Firebase
    Serial.println("--------------------------------");
    Serial.println("🔄 Configuring Firebase...");
    Serial.println("--------------------------------\n");

    config.api_key = API_KEY;
    auth.user.email = USER_EMAIL;
    auth.user.password = USER_PASSWORD;
    config.database_url = DATABASE_URL;

    Serial.println("📌 API Key: " + String(API_KEY));
    Serial.println("📌 Database URL: " + String(DATABASE_URL));
    Serial.println("📌 User Email: " + String(USER_EMAIL));

    // Step 3: Initialize Firebase
    Serial.println("\n🔄 Initializing Firebase...\n");
    Firebase.begin(&config, &auth);
    Firebase.reconnectNetwork(true);

    // Step 4: Wait for Firebase Login
    Serial.println("🔄 Logging into Firebase...");
    int loginAttempts = 0;
    while (!Firebase.ready() && loginAttempts < 20) {
        Serial.print(".");
        delay(500);
        loginAttempts++;
    }

    if (Firebase.ready()) {
        Serial.println("\n✅ Successfully logged into Firebase!\n");
    } else {
        Serial.println("\n❌ Failed to log into Firebase.");
        Serial.print("🔴 Error: ");
        Serial.println(fbdo.errorReason().c_str());
        Serial.println("🔄 Restarting ESP32...\n");
        delay(2000);
        ESP.restart();
    }

    // Step 5: Check Database Connection
    Serial.println("🔄 Testing Firebase Database Connection...");
    if (Firebase.RTDB.setString(&fbdo, "/test", "ESP32 Connected!")) {
        Serial.println("✅ Database test successful! Data written.\n");
    } else {
        Serial.println("❌ Database write failed.");
        Serial.print("🔴 Error: ");
        Serial.println(fbdo.errorReason().c_str());
    }

    // Initialize LED
    pinMode(WhiteLed1, OUTPUT);
    digitalWrite(WhiteLed1, LOW); // Ensure LED is off initially
    pinMode(WhiteLed2, OUTPUT);
    digitalWrite(WhiteLed2, LOW); // Ensure LED is off initially
    pinMode(WhiteLed3, OUTPUT);
    digitalWrite(WhiteLed3, LOW); // Ensure LED is off initially
}

void loop() {
    if (Firebase.ready() && (millis() - sendDataPrevMillis > 2000)) {
        sendDataPrevMillis = millis();

        //Serial.println("🔄 Checking LED status from Firebase...");
        int WhiteLed1State;

        if (Firebase.RTDB.getInt(&fbdo, "/WhiteLed1", &WhiteLed1State)) {
            //Serial.print("🔵 Received White LED State WhiteLed1: ");
            //Serial.println(WhiteLed1State);
            digitalWrite(WhiteLed1, WhiteLed1State);
        } else {
            //Serial.println("❌ Failed to get LED state from Firebase.");
            //Serial.print("🔴 Error: ");
            Serial.println(fbdo.errorReason().c_str());
        }
        int WhiteLed2State;
        if (Firebase.RTDB.getInt(&fbdo, "/WhiteLed2", &WhiteLed2State)) {
            //Serial.print("🔵 Received White LED State WhiteLed2: ");
            //Serial.println(WhiteLed2State);
            digitalWrite(WhiteLed2, WhiteLed2State);
        } else {
            //Serial.println("❌ Failed to get LED state from Firebase.");
            //Serial.print("🔴 Error: ");
            Serial.println(fbdo.errorReason().c_str());
        }
        int WhiteLed3State;
        if (Firebase.RTDB.getInt(&fbdo, "/WhiteLed3", &WhiteLed3State)) {
            //Serial.print("🔵 Received White LED State WhiteLed3: ");
            //Serial.println(WhiteLed3State);
            digitalWrite(WhiteLed3, WhiteLed3State);
        } else {
            //Serial.println("❌ Failed to get LED state from Firebase.");
            //Serial.print("🔴 Error: ");
            Serial.println(fbdo.errorReason().c_str());
        }
        int ServoMotorState;
        if (Firebase.RTDB.getInt(&fbdo, "/ServoMotor", &ServoMotorState)) {
            //Serial.println("🔵 >>>>>>>> Received White LED State ServoMotor: ");
            if(ServoMotorState == 0 || ServoMotorState == '0'){
              myServo.write(90);
            //Serial.println("🔵🔵🔵🔵🔵🔵 Received White LED State ServoMotor: ");
            }else{
              myServo.write(0);
            //Serial.println("🔴🔴🔴🔴🔴🔴 Received White LED State ServoMotor: ");
            }
        } else {
            //Serial.println("❌ Failed to get LED state from Firebase.");
            //Serial.print("🔴 Error: ");
            //Serial.println(fbdo.errorReason().c_str());
        }
        int UltraSonicState;
        if (Firebase.RTDB.getInt(&fbdo, "/UltraSonic", &UltraSonicState)) {
            //Serial.println("🔵 >>>>>>>> Received White ........... State UltraSonic: ");
            if(UltraSonicState == 1 || UltraSonicState == '1'){
              
              //******************************************************************************
              



           
    float distance = getDistance(); // قراءة المسافة
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");

    // تشغيل البازر والـ LED إذا كانت المسافة أكبر من 2 سم
    if (distance > 5) {
        digitalWrite(BUZZER_PIN, HIGH);
        digitalWrite(LED_PIN, HIGH); // تشغيل الـ LED
            Serial.println("led on");
    } else {
            Serial.println("led off ");
        digitalWrite(BUZZER_PIN, LOW);
        digitalWrite(LED_PIN, LOW);  // إطفاء الـ LED
    }

    delay(500); // تأخير للحد من عدد القراءات



              //******************************************************************************

            //Serial.println("😊😊😊😊😊😊😊😊😊😊😊😊😊😊😊😊😊😊😊😊 Received White LED State UltraSonic: ");
            }else{
              Serial.println("stop led ");
              digitalWrite(BUZZER_PIN, LOW);
              digitalWrite(LED_PIN, LOW);  // إطفاء الـ LED
            }
        } else {
            //Serial.println("❌ Failed to get LED state from Firebase.");
            //Serial.print("🔴 Error: ");
            Serial.println(fbdo.errorReason().c_str());
        }
    }
}

float getDistance() {
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);

    long duration = pulseIn(ECHO_PIN, HIGH); // قياس مدة استقبال الإشارة
    float distance = duration * 0.034 / 2;  // حساب المسافة بالسنتيمتر

    return distance;
}
