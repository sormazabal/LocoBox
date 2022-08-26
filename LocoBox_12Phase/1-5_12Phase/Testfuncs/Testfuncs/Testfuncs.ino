#include <Adafruit_I2CDevice.h>
#include <Adafruit_BusIO_Register.h>
#include <Adafruit_SPIDevice.h>
#include <Adafruit_I2CRegister.h>
#include <RTClib.h>
#include <Wire.h>

RTC_DS3231 rtc; 

// Digital In-Out
int DIn[5] = {2, 4, 6, 8, 10};  // PIR evens number digital use pinMode
int DOut[5] = {3, 5, 7, 9, 11}; // LED odds number digital use pinMode
unsigned long previousMillis = 0UL;
unsigned long interval1 = 1000UL;
unsigned long interval90 = 90UL;
unsigned long invalue = 0UL;
unsigned long maxValue = 16000000UL;
int previoussecs;
int HourOn = 7;
float tcyclelen = 22.3;

void millis_delay(unsigned long interval)
{
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis > interval)
  {
    /* The Arduino executes this code once every second
     *  (interval = 1000 (ms) = 1 second).
     */
    // Don't forget to update the previousMillis value
    previousMillis = currentMillis;
  }
}

void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);
  Wire.begin();
  // start the connexion to the RTC

  // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  for (int i = 0; i < 5; i++)
  {
    pinMode(DIn[i], INPUT);   // PIR
    pinMode(DOut[i], OUTPUT); // LED
  }

  if (!rtc.begin())
  {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while (1)
    {
      millis_delay(interval1); // recommended delay before start-up (1-sec)
    }
  }

  if (rtc.lostPower())
  {
    Serial.println("RTC lost power, let's set the time!");
    // When time needs to be set on a new device, or after a power loss, the
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }

}

void loop() {
  // put your main code here, to run repeatedly:

HourOn = ActHourOn1(HourOn, 0, tcyclelen, 1);


}


int ActHourOn1(int HourOn, int MinuteOn, float tcyclelen, int phase1)
{

    tcyclelen = check_if_phase1_started(tcyclelen, phase1);
    int i = 1;    
    int hro = HourOn;
    int mino = MinuteOn;
    //int hroff = HourOFF1[i];
    int minutes_total = tcyclelen * 60;
    int tcycleminutes = minutes_total % 60;

    DateTime now = rtc.now();
    DateTime yesterday  = now - TimeSpan(0,tcyclelen,tcycleminutes,0);    
    DateTime old_on = DateTime(yesterday.year(),yesterday.month(),yesterday.day(),hro,mino,0); //yesterday won't increment in the console because I'm checking yesterday
    DateTime future = (old_on + TimeSpan(0,tcyclelen,tcycleminutes,0)); //if tcyclelen is float, it just truncates it
    int diff = (future - old_on).hours();
    String str = String(future.year(), DEC) + '/' + String(future.month(), DEC) + '/' + String(future.day(), DEC) + " " + String(future.hour(), DEC) + ':' + String(future.minute(), DEC) + ':' + String(future.second(), DEC);
    Serial.println(str);
    HourOn = future.hour();
    return HourOn;
  if(diff > tcyclelen) { 
    
    
    Serial.println("future ");
    Serial.println(future.hour());
    return HourOn;
  }
  else{
    Serial.println("Haven't reached next phase ");
  }
}


int check_if_phase1_started( float tcyclelen, int phase1){

  if (phase1 == 0){
    tcyclelen = 24;
    }else{
      tcyclelen = tcyclelen;
      
      }  

    return tcyclelen;
    
  }

  
