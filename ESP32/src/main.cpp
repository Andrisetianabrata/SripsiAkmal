#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <WiFi.h>
#include <HTTPClient.h>

int c;
int humanCount = 0;
String status = "";
const int8_t buzzer = 19;
const char *ssid = "ADMIN";
const char *password = "MesakkeBangsaku1611";
const char *url = "http://192.168.1.16:8080/website/api/";
String httpRequestData = "";

struct flaging
{
  bool flag = false;
  bool flag_kirim = false;
  uint32_t prevTime = 0;
};

struct pinout
{
  const int8_t dempet;
  const int8_t count;
};

struct variable
{
  uint32_t timer;
  uint32_t prevTimer = 0;
  String dempet;
  String kalimat;
  bool flag = false;
  int count;
};

variable ke1;
variable ke2;
variable ke3;
pinout A = {35, 33};
pinout B = {32, 34};
flaging counting;
flaging dempet_samping;
flaging dempet_depan;
LiquidCrystal_I2C lcd(0x27, 16, 2);

void lcd_update()
{
  lcd.setCursor(4, 1);
  lcd.print("Count: ");
  lcd.print(humanCount);
  lcd.print(" ");
}
void task_sensor(void *pvParameters)
{

  lcd.setCursor(4, 1);
  lcd.print("Count: ");
  lcd.print(humanCount);
  lcd.print(" ");
  bool testo = false;
  while (true)
  {
    int sensorA = analogRead(A.count);
    int sensorB = analogRead(B.count);
    if ((sensorA > 1000) || (sensorB > 1000)){}
    
    if (sensorA > 1000 && sensorB < 1000 && !ke1.flag)
    {
      ke1.timer = millis();
      ke1.flag = 1;
    }

    if (sensorB > 1000 && sensorA < 1000 && !ke2.flag)
    {
      ke2.timer = millis();
      ke2.flag = 1;
    }

    if (sensorA < 1000 && sensorB < 1000)
    {
      ke1.prevTimer = ke1.timer;
      ke2.prevTimer = ke2.timer;
      ke1.flag = false;
      ke2.flag = false;
    }

    if (ke1.flag && ke2.flag)
    {
      if (ke2.timer > ke1.timer)
      {
        humanCount = humanCount + 1;
        c = humanCount;
        lcd_update();
        status = "masuk";
      }
      if (ke1.timer > ke2.timer)
      {
        humanCount = humanCount - 1;
        if (humanCount < 0)
        {
          humanCount = 0;
        }
        c = humanCount;
        lcd_update();
        status = "keluar";
      }
      if(!counting.flag){
        counting.flag_kirim = true;
        vTaskDelay(10);
      }
      counting.flag_kirim = false;
      counting.flag = true;
      ke1.flag = false;
      ke2.flag = false;
    }else{
      counting.flag = false;
      counting.flag_kirim = false;
    }

    int ldr1 = analogRead(A.dempet);
    int ldr2 = analogRead(B.dempet);

    if (ldr1 < 1000 || ldr2 < 1000)
    {
      digitalWrite(buzzer, HIGH);
    }
    else
    {
      digitalWrite(buzzer, LOW);
    }

    if (ldr1 < 1000)
    {
      ke1.kalimat = "ya";
      if(!dempet_depan.flag)
      {
        dempet_depan.flag_kirim = true;
        vTaskDelay(10);
      }
      dempet_depan.flag = true;
      dempet_depan.flag_kirim = false;
    }
    else
    {
      ke1.kalimat = "tidak";
      dempet_depan.flag = false;
      dempet_depan.flag_kirim = false;
    }

    if (ldr2 < 1000)
    {
      ke2.kalimat = "ya";
      if(!dempet_samping.flag)
      {
        dempet_samping.flag_kirim = true;
        vTaskDelay(10);
      }
      dempet_samping.flag = true;
      dempet_samping.flag_kirim = false;
    }
    else
    {
      ke2.kalimat = "tidak";
      dempet_samping.flag = false;
      dempet_samping.flag_kirim = false;
    }

    ke3.timer = millis();
    if (dempet_depan.flag_kirim || dempet_samping.flag_kirim)
    {
      testo = true;
    }
    if (ke3.timer - ke3.prevTimer > 500)
    {
      Serial.printf("Kalimat1: %s\nKalimat2: %s\n", ke1.kalimat, ke2.kalimat);
      Serial.printf("timer1  : %d\nTimer2  : %d\n", ke1.timer, ke2.timer);
      Serial.printf("LDR1    : %d\nLDR2    : %d\n", sensorA, sensorB);
      Serial.printf("Samping : %d\nDepan   : %d\n", dempet_samping.flag, dempet_depan.flag);
      Serial.printf("Skirim  : %d\nDkirim  : %d\n", dempet_samping.flag_kirim, dempet_depan.flag_kirim);
      Serial.printf("Testo   : %d\n\n", testo);
      ke3.prevTimer = ke3.timer;
    }
    vTaskDelay(5);
  }
}

void task_web(void *pvParameters)
{
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED)
  {
    vTaskDelay(500);
    Serial.print(".");
  }
  Serial.print("OK! IP=");
  Serial.println(WiFi.localIP());
  while (true)
  {
    if (counting.flag_kirim)
    {
      if (WiFi.status() == WL_CONNECTED)
      {
        HTTPClient http;
        char bufer[90];
        // sprintf(bufer,"%screate.php?kondisi=%s&dempet_samping=%s&dempet_belakang=%s&jumlah=%d", url, status.c_str(), ke1.kalimat.c_str(), ke2.kalimat.c_str(), c);
        sprintf(bufer,"%screate.php?kondisi=%s&jumlah=%d", url, status.c_str(), humanCount);
        Serial.println(bufer);
        http.begin(bufer);
        int httpResponseCode = http.GET();

        if (httpResponseCode > 0)
        {
          Serial.print("HTTP Response code: ");
          Serial.println(httpResponseCode);
          String payload = http.getString();
          Serial.println(payload);
        }
        else
        {
          Serial.print("Error code: ");
          Serial.println(httpResponseCode);
        }
        http.end();
      }
      else
      {
        Serial.println("WiFi Disconnected");
      }
    }
    ke1.dempet = ke1.kalimat;
    ke2.dempet = ke2.kalimat;
    vTaskDelay(5);
  }
}
void task_web_kondisi(void *pvParameters)
{
  while (true)
  {
    if (dempet_depan.flag_kirim || dempet_samping.flag_kirim)
    {
      if (WiFi.status() == WL_CONNECTED)
      {
        HTTPClient http;
        char bufer[150];
        sprintf(bufer,"%screate_peringatan.php?dempet_samping=%s&dempet_depan=%s", url, ke1.kalimat.c_str(), ke2.kalimat.c_str());
        Serial.println(bufer);
        http.begin(bufer);
        int httpResponseCode = http.GET();

        if (httpResponseCode > 0)
        {
          Serial.print("HTTP Response code: ");
          Serial.println(httpResponseCode);
          String payload = http.getString();
          Serial.println(payload);
        }
        else
        {
          Serial.print("Error code: ");
          Serial.println(httpResponseCode);
        }
        http.end();
      }
      else
      {
        Serial.println("WiFi Disconnected");
      }
    }
    ke1.dempet = ke1.kalimat;
    ke2.dempet = ke2.kalimat;
    vTaskDelay(5);
  }
}

void setup()
{
  Serial.begin(115200);
  lcd.init();      // Initialize the lcd
  lcd.backlight(); // Turn on LCD's led
  lcd.clear();     // Clear LCD screen
  pinMode(buzzer, OUTPUT);
  
  xTaskCreatePinnedToCore(task_web, "web task", 7800, NULL, 1, NULL, 0);
  xTaskCreatePinnedToCore(task_web_kondisi, "web kondisi task", 7800, NULL, 2, NULL, 0);
  xTaskCreatePinnedToCore(task_sensor, "sensor task", 2344, NULL, 3, NULL, 1);
}
void loop()
{
}