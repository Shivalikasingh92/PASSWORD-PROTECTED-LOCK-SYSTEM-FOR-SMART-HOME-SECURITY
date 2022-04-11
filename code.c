#include <Keypad.h> // naming the keypad
#include <LiquidCrystal.h> // naming the LCD 
#include <Servo.h> // naming the servo

const int buzzer = 0;//piezo connected to pin 0
Servo myservo;// declaring servo
LiquidCrystal lcd(A0,A1,A2,A3,A4,A5);// pin connected to arduino from LCD
int angle;// angle of micro servo rotation 

#define Password_Lenght 5 // Give enough room for six chars + NULL char
char Data[Password_Lenght]; // 6 is the number of chars it can hold + the null char = 7
char Master[Password_Lenght] = "4502"; //Password to unlock the door
byte data_count = 0, master_count = 0;
bool Pass_is_good;
char customKey;
const byte ROWS = 4;
const byte COLS = 4;
char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[ROWS] = {8,7,6,5}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {4,3,2,1}; //connect to the column pinouts of the keypad
Keypad myKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

void setup()// action are performed in the setup
{
  myservo.attach(9);// Servo is connected at pin 9
  pinMode(buzzer, OUTPUT);// Set buzzer as OUTPUT
  lcd.begin(16, 2);//To begin LCD
}

void loop()// action are performed in the loop
{
  
  myservo.write(0);
  lcd.setCursor(0, 0);// LCD prints at first row 
  lcd.print("Enter Password");// LCD prints Enter Password

  customKey = myKeypad.getKey();
  if (customKey) // makes sure a key is actually pressed, equal to (customKey != NO_KEY)
  {
    Data[data_count] = customKey; // store char into data array
    lcd.setCursor(data_count, 1); // move cursor to show each new char
    lcd.print(Data[data_count]); // print char at said cursor
    data_count++; // increment data array by 1 to store new char, also keep track of the number of chars entered
  }

  if (data_count == Password_Lenght - 1) // if the array index is equal to the number of expected chars, compare data to master
  {
    lcd.clear();
    lcd.setCursor(0, 0); 

    if (!strcmp(Data, Master)) // equal to (strcmp(Data, Master) == 0)
    {
      lcd.print("CORRECT!");           
      angle = map(angle, 0, 1023, 0, 90); // servo will turn 90 degrees to left
      myservo.write(angle);
      delay(1000); // To make the servo hold 1 seeconds before turning left
      angle = map(angle, 0, 1023, 90, 0); // servo will turn 90 degrees to right
      myservo.write(angle);
    }

    else 
    {
      lcd.print("INCORRECT!");// LCD will print INCORRECT
      lcd.setCursor(0, 1); // LCD prints at second line 
      lcd.print("TRY AGAIN");//LCD prints TRY AGAIN
      
      tone(buzzer, 1000); // Buzzer rings 
      delay(1000);//Buzzer rings for 1 seconds
      noTone(buzzer);
    }
    delay(1000);// added 1 second delay to make sure the password is completely shown on screen before it gets cleared.
    lcd.clear();
    clearData();
  }
}

void clearData()
{
  while (data_count != 0)  // This can be used for any array size,
  { 
    Data[data_count--] = 0; //clear array for new data
    

  }
 return;
 
}