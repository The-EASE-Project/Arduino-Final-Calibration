#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>
#include <LiquidCrystal_I2C.h> // lcd Header
//LiquidCrystal_I2C lcd(7,6,5,4,3,2); // pins for LCD Connection

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
        
        int flag = 0;
#define buzzer 12 // buzzer pin
#define ledPin 13 //led pin
#define DEVICE (0x53) //ADXL345 device address
#define TO_READ (6) //num of bytes we are going to read each time (two bytes for each axis)

#define maxVal 4.00
#define maxValY 2.00

#define alertTime 5000 
int buz = 0;
long start;

/* Assign a unique ID to this sensor at the same time */
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);

void displaySensorDetails(void)
{
  sensor_t sensor;
  accel.getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    ");  Serial.println(sensor.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" m/s^2");
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" m/s^2");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" m/s^2");  
  Serial.println("------------------------------------");
  Serial.println("");
  delay(500);
}

void displayDataRate(void)
{
  Serial.print  ("Data Rate:    "); 
  
  switch(accel.getDataRate())
  {
    case ADXL345_DATARATE_3200_HZ:
      Serial.print  ("3200 "); 
      break;
    case ADXL345_DATARATE_1600_HZ:
      Serial.print  ("1600 "); 
      break;
    case ADXL345_DATARATE_800_HZ:
      Serial.print  ("800 "); 
      break;
    case ADXL345_DATARATE_400_HZ:
      Serial.print  ("400 "); 
      break;
    case ADXL345_DATARATE_200_HZ:
      Serial.print  ("200 "); 
      break;
    case ADXL345_DATARATE_100_HZ:
      Serial.print  ("100 "); 
      break;
    case ADXL345_DATARATE_50_HZ:
      Serial.print  ("50 "); 
      break;
    case ADXL345_DATARATE_25_HZ:
      Serial.print  ("25 "); 
      break;
    case ADXL345_DATARATE_12_5_HZ:
      Serial.print  ("12.5 "); 
      break;
    case ADXL345_DATARATE_6_25HZ:
      Serial.print  ("6.25 "); 
      break;
    case ADXL345_DATARATE_3_13_HZ:
      Serial.print  ("3.13 "); 
      break;
    case ADXL345_DATARATE_1_56_HZ:
      Serial.print  ("1.56 "); 
      break;
    case ADXL345_DATARATE_0_78_HZ:
      Serial.print  ("0.78 "); 
      break;
    case ADXL345_DATARATE_0_39_HZ:
      Serial.print  ("0.39 "); 
      break;
    case ADXL345_DATARATE_0_20_HZ:
      Serial.print  ("0.20 "); 
      break;
    case ADXL345_DATARATE_0_10_HZ:
      Serial.print  ("0.10 "); 
      break;
    default:
      Serial.print  ("???? "); 
      break;
  }  
  Serial.println(" Hz");  
}

  void displayRange(void)
  {
    Serial.print  ("Range:         +/- "); 
    
    switch(accel.getRange())
    {
      case ADXL345_RANGE_16_G:
        Serial.print  ("16 "); 
        break;
      case ADXL345_RANGE_8_G:
        Serial.print  ("8 "); 
        break;
      case ADXL345_RANGE_4_G:
        Serial.print  ("4 "); 
        break;
      case ADXL345_RANGE_2_G:
        Serial.print  ("2 "); 
        break;
      default:
        Serial.print  ("?? "); 
        break;
    }  
    Serial.println(" g");  
  }

void setup(void) 
{

#ifndef ESP8266
  while (!Serial); // for Leonardo/Micro/Zero
#endif
  lcd.begin(16,2); //initializing lcd
  
  Serial.begin(9600);
  Serial.println("Accelerometer Test"); Serial.println("");

  delay(1000);

   buz = 0;
    lcd.print("Earthquake ");
    lcd.setCursor(0,1);
    
    lcd.print("Detector ");
    delay(2000);
   
    lcd.clear();
    lcd.print("Calibrating.....");
    lcd.setCursor(0,1);
    
    lcd.print("Please wait...");

    
  /* Initialise the sensor */
  if(!accel.begin())
  {
    /* There was a problem detecting the ADXL345 ... check your connections */
    Serial.println("Ooops, no ADXL345 detected ... Check your wiring!");
    while(1);
  }

  /* Set the range to whatever is appropriate for your project */
  accel.setRange(ADXL345_RANGE_2_G);
  // accel.setRange(ADXL345_RANGE_8_G);
  // accel.setRange(ADXL345_RANGE_4_G);
  // accel.setRange(ADXL345_RANGE_2_G);

  //accel.getDataRate(ADXL345_DATARATE_100_HZ);
  
  /* Display some basic information on this sensor */
  displaySensorDetails();
  
  /* Display additional settings (outside the scope of sensor_t) */
  displayDataRate();
  displayRange();
  Serial.println("");

  
    delay(3000);
    lcd.clear();
    lcd.print("Calibrated");
    delay(1000);
    lcd.clear();
    lcd.print("Device Ready");
    delay(1000);
    lcd.clear();
    lcd.print("  X   Y    Z ");
}

  int flagCounter(int tempFlag){
    delay(500);
    if (tempFlag < 1){
      
    tempFlag++;
    Serial.println(tempFlag);
    return tempFlag;
    }
    
    else 
      return 10;
    
  }
   
void loop(void) 
{
  /* Get a new sensor event */ 
  sensors_event_t event; 
  accel.getEvent(&event);
 
  /* Display the results (acceleration is measured in m/s^2) */
  Serial.print("X: "); Serial.print(event.acceleration.x, 2); Serial.print("  ");
  Serial.print("Y: "); Serial.print(event.acceleration.y, 2); Serial.print("  ");
  Serial.print("Z: "); Serial.print(event.acceleration.z, 2); Serial.print("  ");Serial.println("m/s^2 ");
  delay(500);

  lcd.setCursor(0,1);
  lcd.print(event.acceleration.x, 2);
  lcd.setCursor(6,1);
  lcd.print(event.acceleration.y, 2);
  lcd.setCursor(12,1);
  lcd.print(event.acceleration.z, 2);
  delay(100);
  

  if (event.acceleration.x > maxVal || event.acceleration.y > maxValY) {

    lcd .setCursor(0,0);
    Serial.print("Earthquake Alert! ");
    flag = flagCounter(flag);
    if (flag == 10){
    pinMode(ledPin, OUTPUT);
    digitalWrite (ledPin, HIGH);
    
    delay(3000);
    flag = 0;
    }    
  }

  else 
  {
    delay(1000);
    lcd.clear();
    lcd.print("  X   Y    Z ");

    lcd.setCursor(0,1);
  lcd.print(event.acceleration.x, 2);
  lcd.setCursor(6,1);
  lcd.print(event.acceleration.y, 2);
  lcd.setCursor(12,1);
  lcd.print(event.acceleration.z, 2);
  delay(100);
    digitalWrite (ledPin, LOW);
  }
  

  /**if(event.acceleration.x > maxVal || event.acceleration.y > maxVal || event.acceleration.z > maxVal)
  {
  if(buz == 0)
  start=millis(); // timer start
  buz=1; // buzzer / led flag activated
  }*/
}
