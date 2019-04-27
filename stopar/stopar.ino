#include <SoftwareSerial.h>
#include <EEPROM.h>

#define LED 13
#define BUTTON_OUT 6
#define BUTTON_IN 7
#define OBSTACLE 12

#define NO_LINE 10000
#define IN_STAIN 10001

#define ML_A  8
#define ML_B  9
#define MR_A  10
#define MR_B  11

#define LEFT_MOTOR  1
#define RIGHT_MOTOR 2

#define s1 A0
#define s2 A1
#define s3 A2
#define s4 A3
#define s5 A4
#define s6 A5
#define s7 A6
#define s8 A7

#define print(X) { Serial.print(X); bt.print(X); }
#define println(X) { Serial.println(X); bt.println(X); }

volatile uint8_t rL, rR;
volatile uint8_t t1_tick;
volatile uint8_t onL, offL, onR, offR;

uint16_t ln[8];
uint16_t lnmin[8];
uint16_t lnmax[8];

int16_t ln_cal[8];

long tm_last_seen_line;

SoftwareSerial bt(3, 2);

void setup_timer()
{
  ICR1 = 1600;  // timer1 freq. = 10kHz
  TCNT1 = 0;
  TCCR1A = 0b00000010;
  TCCR1B = 0b00011001;
  TCCR1C = 0;
  TIMSK1 = 0b00000001;
}

void setup() 
{
  pinMode(LED, OUTPUT);
  for (int i = 14; i <= 19; i++)
  {
    pinMode(i, INPUT);
    digitalWrite(i, LOW);
  }
  
  pinMode(BUTTON_IN, INPUT);
  digitalWrite(BUTTON_IN, HIGH);
  pinMode(BUTTON_OUT, OUTPUT);
  digitalWrite(BUTTON_OUT, LOW);

  pinMode(OBSTACLE, INPUT);

  pinMode(ML_A, OUTPUT);
  digitalWrite(ML_A, LOW);
  pinMode(ML_B, OUTPUT);
  digitalWrite(ML_B, LOW);
  pinMode(MR_A, OUTPUT);
  digitalWrite(MR_A, LOW);
  pinMode(MR_B, OUTPUT);
  digitalWrite(MR_B, LOW);

  setup_timer();

  delay(200);
  motor_speed(LEFT_MOTOR, 0);
  motor_speed(RIGHT_MOTOR, 0);
  motor_dir(LEFT_MOTOR, 0);
  motor_dir(RIGHT_MOTOR, 0);

  Serial.begin(115200);
  bt.begin(9600);

  read_eeprom_calibration();

  DIDR0 = 63;
  ADCSRA |= 7;
  TWCR &= ~(4);
}

void read_eeprom_calibration()
{
  for (int i = 0; i < 8; i++)
  {
    lnmin[i] = (uint8_t)EEPROM.read(i * 4) + ((uint16_t)EEPROM.read(i * 4 + 1) << 8);
    lnmax[i] = (uint8_t)EEPROM.read(i * 4 + 2) + ((uint16_t)EEPROM.read(i * 4 + 3) << 8);
  }
  digitalWrite(LED, HIGH);
  delay(100);
  digitalWrite(LED, LOW);
  delay(100);
}

void write_eeprom_calibration()
{
  for (int i = 0; i < 8; i++)
  {
    EEPROM.write(i * 4, lnmin[i] & 255);
    EEPROM.write(i * 4 + 1, lnmin[i] >> 8);
    EEPROM.write(i * 4 + 2, lnmax[i] & 255);
    EEPROM.write(i * 4 + 3, lnmax[i] >> 8);
  }
  digitalWrite(LED, HIGH);
  delay(1000);
  digitalWrite(LED, LOW);
  delay(500);
}

int available()
{
  return Serial.available() || bt.available();  
}

int readchar()
{
  if (bt.available()) return bt.read();
  if (Serial.available()) return Serial.read();
  return 0;
}

// returns -5000..5000 - position of line
int detect_line()
{
  int8_t sused;
  read_calib_line_sensor(); 
  
  uint8_t max_index = 0;
  uint8_t full = 0;
  for (int i = 0; i < 8; i++)
  {
    if (ln_cal[i] > ln_cal[max_index]) max_index = i;
    if (ln_cal[i] == 1000) full++;
  }

  if (ln_cal[max_index] < 150)
  {
    return NO_LINE;
  } 

  tm_last_seen_line = millis();
  
  if (full >= 3) return IN_STAIN;
  
  if (max_index == 0) 
  {
    if (ln_cal[1] >= 100) sused = -1;
    else sused = 1;
  }
  else if (max_index == 7) 
  {
    if (ln_cal[6] >= 100) sused = 1;
    else sused = -1;
  }
  else if (ln_cal[max_index - 1] > ln_cal[max_index + 1]) sused = 1;
  else sused = -1;

  return (4 - max_index) * 1000 - 500 + sused * (1000 - ln_cal[max_index]);
}

void flushread()
{
  while (available()) readchar();
}

void motor_speed(uint8_t motor, uint8_t speed)
{
  switch (motor)
  {
    case 1: rL = speed; break;
    case 2: rR = speed; break;
  }
}

void motor_dir(uint8_t motor, uint8_t smer)
{
  switch (motor)
  {
    case 1: digitalWrite(ML_B, smer); offL = smer; onL = 1 - smer; break;
    case 2: digitalWrite(MR_B, smer); offR = smer; onR = 1 - smer; break;
  }
}

void motor_reverse_direction()
{
  uint8_t smer = offL;
  offL = 1 - smer;
  onL = smer;
  digitalWrite(ML_B, 1 - smer);
  smer = offR;
  offR = 1 - smer;
  onR = smer;
  digitalWrite(MR_B, 1 - smer);
}

void set_motor(uint8_t motor, int8_t speed)
{
  if (speed > 0)
  {
    motor_dir(motor, (motor == 1) ? 0 : 1);
    motor_speed(motor, speed);
  }
  else
  {
    motor_dir(motor, (motor == 1) ? 1 : 0);
    motor_speed(motor, -speed);
  }
}

void test_motors_with_timer()
{
  println("test motors");
  for (int i = 0; i < 21; i++)
  {
    //Serial.println(i);

    //Serial.println("1 on");
    motor_speed(1, i);
    delay(400);
    //Serial.println("1 off");
    motor_speed(1, 0);
    delay(400);
    //Serial.println("2 on");
    motor_speed(2, i);
    delay(400);
    //Serial.println("2 off");
    motor_speed(2, 0);
    delay(400);
  }
}

ISR(TIMER1_OVF_vect)
{
  if (t1_tick == 0)
  {
    digitalWrite(ML_A, onL);
    digitalWrite(MR_A, onR);
  }
  if (t1_tick == rL) digitalWrite(ML_A, offL);
  if (t1_tick == rR) digitalWrite(MR_A, offR);
  t1_tick++;
  if (t1_tick >= 20) t1_tick = 0;
}

uint8_t button()
{
  return !digitalRead(BUTTON_IN);  
}

#define TURN_RIGHT_DELAY 700
#define MOVE_TO_RIGHT_DELAY 1100
#define ALIGN_STRAIGHT_DELAY 600
#define PASS_OBSTACLE_DELAY 600
#define CENTER_ON_LINE_DELAY 150
#define ALIGN_LINE_BACK_DELAY 150

#define ON_LEFT 1
#define ON_RIGHT 2
uint8_t AVOID_SIDE = ON_RIGHT;

long AVOID_START_TIME = 15000;
long AVOID_END_TIME = 180000;

long tm_started;

uint8_t avoiding_time()
{
  long tm = millis() - tm_started;
  if ((tm < AVOID_START_TIME) || (tm > AVOID_END_TIME)) return 0;
  return 1;
}

void avoid_obstacle()
{
  static uint8_t avoid_count = 0;
  uint8_t m1, m2;
  
  if (!avoiding_time()) return;

  avoid_count++;
  if (avoid_count == 1) { tm_started = millis(); AVOID_START_TIME = 8000; return; }
  if (avoid_count > 3) return;
  
  if (AVOID_SIDE == ON_RIGHT)
  {
    m1 = LEFT_MOTOR;
    m2 = RIGHT_MOTOR;
  }
  else
  {
    m1 = RIGHT_MOTOR;
    m2 = LEFT_MOTOR;
  }
  set_motor(m1, 0);
  set_motor(m2, -10);
  delay(TURN_RIGHT_DELAY);
  
  set_motor(m1, 10);
  set_motor(m2, 10);
  delay(MOVE_TO_RIGHT_DELAY);
  
  set_motor(m1, 0);
  delay(ALIGN_STRAIGHT_DELAY);
  
  set_motor(m1, 10);
  delay(PASS_OBSTACLE_DELAY);
  
  set_motor(m1, 0);
  delay(ALIGN_STRAIGHT_DELAY);
  
  set_motor(m1, 6);
  set_motor(m2, 6);
  while (detect_line() == NO_LINE);
  
  delay(CENTER_ON_LINE_DELAY);
  set_motor(m1, 7);
  set_motor(m2, -4);
  delay(ALIGN_LINE_BACK_DELAY);
  set_motor(m2, 10);

  AVOID_SIDE = ON_LEFT;
}

#define GET_OUT_OF_STAIN_TIME 5000

void get_out_of_stain()
{
  digitalWrite(LED, HIGH); 
  set_motor(LEFT_MOTOR, -1);
  set_motor(RIGHT_MOTOR, -1);
  delay(100);
  /*set_motor(LEFT_MOTOR,-6);
  set_motor(RIGHT_MOTOR, 6);
  while (detect_line() != NO_LINE);
  set_motor(LEFT_MOTOR, 6);
  set_motor(RIGHT_MOTOR, 6); */
  long tm = millis();
  do {
     int ln = detect_line();
     if (ln == NO_LINE)
     {
        set_motor(LEFT_MOTOR, -6);
        set_motor(RIGHT_MOTOR, 6);
     }
     else if (ln < 0) 
     {
        set_motor(LEFT_MOTOR, 0);
        set_motor(RIGHT_MOTOR, 7);
     }
     else
     {
        set_motor(LEFT_MOTOR, 7);
        set_motor(RIGHT_MOTOR, 0);
     }
     
  } while (millis() - tm < GET_OUT_OF_STAIN_TIME);
  set_motor(LEFT_MOTOR, 4);
  set_motor(RIGHT_MOTOR, 4);
}

#define TIME_LINE_LOST 2000
#define TIME_SEARCH_LINE 5000

#define TIME_TUNEL 12000
//#define TIME_TUNEL2 000

void tunel()
{
  long tm = millis();
    do {
     int ln = detect_line();
     if (ln == NO_LINE)
     {
        set_motor(LEFT_MOTOR, 6);
        set_motor(RIGHT_MOTOR, 6);
     }
     else if (ln < 0) 
     {
        set_motor(LEFT_MOTOR, 0);
        set_motor(RIGHT_MOTOR, 10);
     }
     else
     {
        set_motor(LEFT_MOTOR, 10);
        set_motor(RIGHT_MOTOR, 0);
     }
  } while (millis() - tm < TIME_TUNEL);

}

void test_line_follow()
{
  static int ln = 0;
  tunel();
  while (!available())
  {
    if (obstacle())
      avoid_obstacle();
    
    int val = detect_line();

    if (val == NO_LINE) 
    {
      if (millis() - tm_last_seen_line > TIME_LINE_LOST)
      {
        digitalWrite(LED, HIGH);
        // try going back the same time
        motor_reverse_direction();
        long tm_start_reverse = millis();
        int see_it = 0;
        while (millis() - tm_start_reverse < TIME_LINE_LOST) 
        {
          if (detect_line() != NO_LINE)
            see_it++;
          if (see_it >= 3) break;
        }
        if (see_it >= 3) { set_motor(LEFT_MOTOR, 4); set_motor(RIGHT_MOTOR,4); digitalWrite(LED, LOW); continue; }

        for (int i = 4; i < 15; i++)
        {
          long tm_start_search = millis();
          set_motor(LEFT_MOTOR, i);
          set_motor(RIGHT_MOTOR, i / 3);
          while (((val = detect_line()) == NO_LINE) && (millis() - tm_start_search < TIME_SEARCH_LINE));
          if (val != NO_LINE) break;
        }
      }
      digitalWrite(LED, LOW); 
      set_motor(LEFT_MOTOR, 7);
      set_motor(RIGHT_MOTOR, 7);
      continue;
    }
    if (val == IN_STAIN)
    {
      get_out_of_stain();
      continue;
    }
    
    if (abs(val - ln) > 2500) 
    {
      ln = val;
      continue;
    }

    //ln = (int)(((long)ln * 7000 + val * 3000) / 10000); 
     if (ln < 0) 
     {
        set_motor(LEFT_MOTOR, 0);
        set_motor(RIGHT_MOTOR, 10);
     }
     else
     {
        set_motor(LEFT_MOTOR, 10);
        set_motor(RIGHT_MOTOR, 0);
     }
  }
}

void print_maintenance_menu()
{
  println("--------------------------------");
  println("--- Istrobot 2019 / 100(1+1) ---");
  println("--------------------------------");
  println("---   user maintenance mode  ---");
  println("--------------------------------");
  println(" 1) test button");
  println(" 2) test line sensor");
//  println(" 3) test motors simple");
//  println(" 4) full speed fwd 2 sec");
  println(" 5) test motors with timer");
  println(" 6) test line follow");
  println(" 7) sensor calibration");
  println(" 8) write calibration to EEPROM");
  println(" 9) print calibration");
  println(" a) test obstacle");
  println(" b) test calib. line sensor");  
  println(" c) test detect line");
  println(" d) test motors");
  println(" e) test reverse direction");
  println(" q) quit maintenance mode");
  println("--------------------------------");
  print(">");
}

void maintenance_menu()
{
  char z = 0;
  print_maintenance_menu();
  do {
    if (available())
    {
      z = readchar();
      println(z);
    
      switch (z)
      {
        case '1': test_button(); break;
        case '2': test_line_sensor(); break;
        case '3': test_motors_simple(); break;
        case '4': full_speed_fwd_2_sec(); break;
        case '5': test_motors_with_timer(); break;
        case '6': test_line_follow(); break;
        case '7': sensor_calibration(); break;
        case '8': write_calib(1); break;
        case '9': print_calibration(); break;
        case 'a': test_obstacle(); break;
        case 'b': test_calib_line_sensor(); break;
        case 'c': test_detect_line(); break;
        case 'd': test_motors(); break;
        case 'e': test_reverse_direction(); break;
        case ' ': set_motor(LEFT_MOTOR, 0); set_motor(RIGHT_MOTOR, 0); break;
      }

      print_maintenance_menu();
    }
    else if (button() == 0)
    {
      digitalWrite(LED, HIGH);
      sensor_calibration();
      write_calib(0);
      delay(50);
      digitalWrite(LED, LOW);
    }
    digitalWrite(LED, HIGH);
    delay(100);
    digitalWrite(LED, LOW);
    delay(100);
  } while (z != 'q');
}

void test_reverse_direction()
{
  set_motor(LEFT_MOTOR, 7);
  set_motor(RIGHT_MOTOR, 3);
  delay(700);
  motor_reverse_direction();
  delay(700);
  set_motor(LEFT_MOTOR, 0);
  set_motor(RIGHT_MOTOR, 0);

  set_motor(LEFT_MOTOR, 3);
  set_motor(RIGHT_MOTOR, 7);
  delay(700);
  motor_reverse_direction();
  delay(700);
  set_motor(LEFT_MOTOR, 0);
  set_motor(RIGHT_MOTOR, 0);

  set_motor(LEFT_MOTOR, 7);
  set_motor(RIGHT_MOTOR, -4);
  delay(700);
  motor_reverse_direction();
  delay(700);
  set_motor(LEFT_MOTOR, 0);
  set_motor(RIGHT_MOTOR, 0);

  set_motor(LEFT_MOTOR, -4);
  set_motor(RIGHT_MOTOR, 7);
  delay(700);
  motor_reverse_direction();
  delay(700);
  set_motor(LEFT_MOTOR, 0);
  set_motor(RIGHT_MOTOR, 0);

}


void test_motors()
{
  for (int m = LEFT_MOTOR; m <= RIGHT_MOTOR; m++)
  {
    if (m == LEFT_MOTOR) { print("LEFT:"); } else { print("RIGHT:"); }
    for (int i = 0; i <= 80; i++)
    {
      int8_t speed = (i <= 20) ? i : ((i <= 60) ? (40 - i) : (i - 80));
      print(speed);print("..");
      set_motor(m, speed);
      delay(250);
      if (available()) { set_motor(1, 0); set_motor(2, 0); flushread(); return; }
    }
  }
}

void test_button()
{
  while (!available())
  {
    println(button());
  }  
  flushread();
}

uint8_t obstacle()
{
  static uint8_t seen = 0;
  if (digitalRead(OBSTACLE)) seen++;
  else seen = 0;
  return (seen > 3);
}

void test_obstacle()
{
  while (!available())
  {
    println(obstacle());
    delay(100);
  }  
  flushread();
}

void test_detect_line()
{
  while (!available())
  {
    println(detect_line());
    delay(50);
  }  
  flushread();
}

void print_10bit(uint16_t val)
{
  if (val > 999) { print(val / 1000); } else { print(" "); }
  if (val > 100) { print((val % 1000) / 100); } else { print(" "); }
  if (val > 10) { print((val % 100) / 10); } else { print(" "); }
  print(val % 10);
}

void read_line_sensor()
{
  for (int i = 0; i < 24; i += 3)
    {
      uint8_t channel = i & 7;
      ln[channel] = analogRead(channel);
      if (channel == 3)
      {
        ln[channel] += analogRead(3);
        ln[channel] += analogRead(3);
        ln[channel] += analogRead(3);
        ln[channel] += analogRead(3);
        ln[channel] += analogRead(3);
        ln[channel] /= 6;
      }
      delayMicroseconds(500);
    }
}

void test_line_sensor()
{
  while (!available())
  {    
    read_line_sensor();
    for (int i = 7; i > 0; i--)
    {
      print_10bit(ln[i]);
      print(":");
    }
    print_10bit(ln[0]);
    println("");
    delay(200);
  }  
  flushread();
}

void read_calib_line_sensor()
{
    read_line_sensor();
    for (int i = 0; i < 8; i++)
    {
       ln_cal[i] = (int16_t) map(ln[i], lnmin[i], lnmax[i], 0, 1000);
       if (ln_cal[i] < 0) ln_cal[i] = 0;
       else if (ln_cal[i] > 1000) ln_cal[i] = 1000;
    }
}

void test_calib_line_sensor()
{
  while (!available())
  {    
    read_calib_line_sensor();

    for (int i = 7; i > 0; i--)
    {
      print_10bit(ln_cal[i]);
      print(":");
    }
    print_10bit(ln_cal[0]);
    println("");
    delay(200);
  }  
  flushread();
}

void full_speed_fwd_2_sec()
{
  digitalWrite(ML_A, HIGH);
  digitalWrite(ML_B, LOW);  
  digitalWrite(MR_A, LOW);
  digitalWrite(MR_B, HIGH);

  delay(2000);
  digitalWrite(ML_A, LOW);
  digitalWrite(ML_B, LOW);  
  digitalWrite(MR_A, LOW);
  digitalWrite(MR_B, LOW);
}

void test_motors_simple()
{
  uint8_t state = 0;
  while (!available())
  {
    print(state);
    print("...");

    switch (state)
    {
      case 0: // fwd
        digitalWrite(ML_A, HIGH);
        digitalWrite(ML_B, LOW);
        break;
      case 1:
      case 3: // off
        digitalWrite(ML_A, LOW);
        digitalWrite(ML_B, LOW);
        break;
      case 2: // bwd
        digitalWrite(ML_A, LOW);
        digitalWrite(ML_B, HIGH);
        break;

      case 4: // fwd
        digitalWrite(MR_A, LOW);
        digitalWrite(MR_B, HIGH);
        break;
      case 5:
      case 7: // off
        digitalWrite(MR_A, LOW);
        digitalWrite(MR_B, LOW);
        break;
      case 6: // bwd
        digitalWrite(MR_A, HIGH);
        digitalWrite(MR_B, LOW);
        break;
    }
    for (int i = 0; i < 100; i++)
      if (available()) break;
      else  delay(30);
    state++;
    state %= 8;
  }  
  flushread();
  println("off");
  digitalWrite(ML_A, LOW);
  digitalWrite(ML_B, LOW);

  digitalWrite(MR_A, LOW);
  digitalWrite(MR_B, LOW);
}

void sensor_calibration()
{
  uint8_t b = button();
  uint32_t tm = millis();
  for (int i = 0; i < 8; i++)
  {
    lnmin[i] = 2000;
    lnmax[i] = 0;
  }
  while (b == button())
  {
     read_line_sensor();
     for (int i = 0; i < 8; i++)
     { 
        if (ln[i] < lnmin[i]) lnmin[i] = ln[i];
        else if (ln[i] > lnmax[i]) lnmax[i] = ln[i];
     }
     if (millis() - tm > 500)
     {
        print_calibration();
        tm = millis();
     }     
  }
}

void print_calibration()
{
   for (int i = 7; i > 0; i--)
   {
     print_10bit(lnmin[i]);
     print(" -");
     print_10bit(lnmax[i]);
     print(" :");
   }
   print_10bit(lnmin[0]);
   print(" -");
   print_10bit(lnmax[0]);
   println("");
}
void write_calib(uint8_t secure)
{
  if (secure)
  {
    flushread();
    print("Type 'ee': >");
    while (!available());
    if (readchar() != 'e') return;
    print("e");
    while (!available());
    if (readchar() != 'e') return;
    println("e");
  }
  write_eeprom_calibration();
  println("done.");
}

void stopar()
{
  tm_started = millis();
  test_line_follow();  
}

void loop() 
{
  if (button()) maintenance_menu();

  while (!button()) ;
  stopar();
}
