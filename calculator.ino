/*
  Enkel keypad kalkulator (OBS! fungerar endast med ensiffriga heltal)
  Martin Myrberg
  IOT22
  22/03/2023
*/

// Inkludera headerfiler för keypad och LCD bibliotek
#include <Keypad.h> 
#include <LiquidCrystal.h>

// Makro för 4a Rader och 4a Kolumner på keypad
#define ROWS 4 
#define COLS 4

// Skapa ett objekt "lcd" från klass i LiquidCrystal-biblioteket och definera vilka pins på Arduino som LCD-displayen är kopplad till
LiquidCrystal lcd(0,1,2,3,4,5);

// Definiera keypaden med en 2D array 
char Keys[ROWS][COLS] = { 
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

// Ange Arduino-anslutningarna till keypadens rad- och kolumnpins. 
byte rowPins[ROWS] = {13,12,11,10};
byte colPins[COLS] = {9,8,7,6};

// Skapa objekt från Keypad-biblioteket. 
// Konstruktorn tar 5 argument:
// "makeKeymap(keys)" anger att keypad ska vara enligt den definierade keypaden (2D-arrayen)
// rowPins/colPins specificerar anslutningarna på Keypadens rad- och kolumnpins till arduinokortets pins
// ROWS/COLS anger antalet rader och kolumner (4x4)
Keypad myKeyPad = Keypad(makeKeymap(Keys), rowPins, colPins, ROWS, COLS);

// Array för att hålla siffrorna i det beräknade uttrycket (ex: 1+1 {1,1}, 2*8 {2,8}, osv.)
int numbers[1];
// Räknare för att hålla koll på input-ordningen från användaren. Ökar sitt värde med 1.
int counter; 
// Variabel för att hålla det valda räknesättet (+,-,*,/)
int Operator;

// Funktion för att nollställa kalkylatorn
void resetCalc() {
  counter = 0;
  lcd.clear();
  lcd.setCursor(0,0);
}

// setup-funktion. Visar välkomstmeddelanden på LCD-skärmen. Körs en gång vid uppstart.
void setup() {
  lcd.begin(16,2); // Definierar att vi använder en 16*2 LCD-display
  lcd.setCursor(0,0);  // sätter startposition för textmarkören på LCD-skärmen
  lcd.print("Welcome to Ardu-"); // visar välkomstmeddelande på LCD-skärmen
  lcd.setCursor(0, 1);   // sätter textmarkören till kolumn 0, rad 1 på LCD-skärmen
  lcd.print("ino calculator");
  delay(3000); // visa infon i 4 sekunder på LCD-skärmen
  lcd.clear(); // töm skärmen
  lcd.setCursor(0,0);
  lcd.print("A = +, B = -");
  lcd.setCursor(0,1);
  lcd.print("C = *, D = /");
  delay(5000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Press * to reset");
  delay(3500);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("NOTE:Only single");
  lcd.setCursor(0,1);
  lcd.print(" digit numbers!");
  delay(3500);
  lcd.clear();
}

// loop-funktionen. Genomför beräkningslogiken.
void loop() {
  char key = myKeyPad.getKey(); // getKey() är en metod från keypad-biblioteket. Lagrar den tryckta tangenten på keypad i en char.
  
  if(key) // Om en tanget på keypaden trycks ner (true), genomför något av villkoren nedan:
  {
    if(counter == 0) // Initialt är counter lika med 0, då...
    {
      // ....kollar vi först så användaren tryckt in en siffra och...
      if(key == 'A' || key == 'B' || key == 'C' || key == 'D' || key == '*' || key == '#')
      { // Felhantering
        lcd.setCursor(0,1);
        lcd.print("  Syntax Error  "); // Meddelande vid eventuellt feltryck
        delay(1500);
        resetCalc();
      }
      else
      { // ...sedan lagrar vi den första siffran till beräkningen i numbers array (index 0) och skriver ut det på LCD-skärmen
        // 48 är lika med ASCII-värdet för '0'. Genom att ta -48 översätter vi char-värdena '0'..'9' till int-värdena 0..9 ('0' - 48 == 0, '1' - 48 == 1 osv.)
        numbers[0] = key-48;
        lcd.print(numbers[0]);
        counter++;
      }
    }
    else if(counter == 1) // När counter är lika med 1...
    {
      // ...lagrar vi det valda beräkningssättet i variabeln "Operator"...
      Operator = key; 
      counter++;
      
      // ...och printar ut det på LCD-skärmen m.h.a en IF-sats.
      if(Operator == 'A')
        lcd.print(" + ");
      else if(Operator == 'B')
        lcd.print(" - ");
      else if(Operator == 'C')
        lcd.print(" * ");
      else if(Operator == 'D')
        lcd.print(" / ");
      else if(key == '*') // om man vill rensa skärmen halvägs in i sin beräkning
        resetCalc();
      else // Felhantering
      {
        lcd.setCursor(0,1);
        lcd.print("  Syntax Error  "); // Meddelande vid feltryck
        delay(1500);
        resetCalc();
      }
    }
    else if(counter == 2) // När counter är lika med 2...
    { // ....kollar vi först så användaren tryckt in en siffra...
      if(key == 'A' || key == 'B' || key == 'C' || key == 'D' || key == '#')
      { // Felhantering
        lcd.setCursor(0,1);
        lcd.print("  Syntax Error  "); // Meddelande vid feltryck
        delay(1500);
        resetCalc();
      }
      else if(key == '*') // om man vill rensa skärmen halvägs in i sin beräkning
        resetCalc();
      else
      { // ...och sen lagrar vi den andra siffran till beräkningen i numbers array (index 1) och skriver ut det på LCD-skärmen
        numbers[1] = key-48;
        lcd.print(numbers[1]);
        lcd.print(" = ");

        // Beroende på operator, genomförs vald beräkning, som samtidigt printas ut på LCD-skärmen
        if(Operator == 'A')
          lcd.print(numbers[0] + numbers[1]);
        else if(Operator == 'B')
          lcd.print(numbers[0] - numbers[1]);
        else if(Operator == 'C')
          lcd.print(numbers[0] * numbers[1]);
        else if(Operator == 'D')
          lcd.print(numbers[0] / numbers[1]);
      
        counter++;
      }
    }  
    // Om counter nått 3 och * trycks ner på keypaden, ska LCD-skärmen rensas
    else if(counter == 3 || key == '*')
      resetCalc();
  }
}
