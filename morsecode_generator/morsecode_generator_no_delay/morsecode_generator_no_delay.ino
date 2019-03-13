const char* MorseTable[] = {
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
        // space, !, ", #, $, %, &, '
        NULL, "-.-.--", ".-..-.", NULL, NULL, NULL, NULL, ".----.",
        // ( ) * + , - . /
        "-.--.", "-.--.-", NULL, ".-.-.", "--..--", "-....-", ".-.-.-", "-..-.",
        // 0 1 2 3 4 5 6 7
        "-----", ".----", "..---", "...--", "....-", ".....", "-....", "--...",
        // 8 9 : ; < = > ?
        "---..", "----.", "---...", "-.-.-.", NULL, "-...-", NULL, "..--..",
        // @ A B C D E F G
        ".--.-.", ".-", "-...", "-.-.", "-..", ".", "..-.", "--.",
        // H I J K L M N O
        "....", "..", ".---", "-.-", ".-..", "--", "-.", "---",
        // P Q R S T U V W
        ".--.", "--.-", ".-.", "...", "-", "..-", "...-", ".--",
        // X Y Z [ \ ] ^ _
        "-..-", "-.--", "--..", NULL, NULL, NULL, NULL, "..--.-",
        // ' a b c d e f g
        NULL, ".-", "-...", "-.-.", "-..", ".", "..-.", "--.",
        // h i j k l m n o
        "....", "..", ".---", "-.-", ".-..", "--", "-.", "---",
        // p q r s t u v w
        ".--.", "--.-", ".-.", "...", "-", "..-", "...-", ".--",
        // x y z { | } ~ DEL
        "-..-", "-.--", "--..", NULL, NULL, NULL, NULL, NULL,
        };

const byte LED_MORSE_PIN = A3;
int led_morse_state = LOW;

const char* morseCodeDefault = "6401";
const byte morseCodeLength = 4;

int index_morseCodeDefault = 0;
int index_morseSymbole = 0;

uint32_t previous_millis_symbole = 0;
uint32_t previous_millis_endMorseCode = 0;
uint32_t previous_millis_dot = 0;
uint32_t previous_millis_dash = 0;
uint32_t previous_millis_dot_or_dash = 0;

const uint16_t interval_millis_dot = 500;                         //Temps dot = 500 ms
const uint16_t interval_millis_dash = 3 * interval_millis_dot;    //Temps dash = 3 * dot = 1,5 s
const uint16_t interval_millis_symbole = 5 * interval_millis_dot; //Temps entre deux symboles = 5 * dot = 2,5 s
const uint16_t interval_millis_endMorseCode = 10000;              //Temps entre les series = 10 s

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_MORSE_PIN, OUTPUT);
  Serial.begin(9600);
}

void loop() {

  uint32_t current_millis = millis();

  if ((index_morseCodeDefault < morseCodeLength) && (current_millis - previous_millis_symbole >= interval_millis_symbole) ) { //Si on a pas fini de parcourir le code morse
                                                                                                                //et que l'interval de temps entre deux symboles (3* dotLength) est respecte alors
    char ch = morseCodeDefault[index_morseCodeDefault];
    const char * morseCodeSymbole = MorseTable[ch];
    flashDashDot(morseCodeSymbole);

    if (index_morseCodeDefault == morseCodeLength) {                    //Declencher compte a rebour 5000 ms si fin code morse
      Serial.println("End of 6401...");
      previous_millis_endMorseCode = millis(); 
    }
    
  } else if ((index_morseCodeDefault == morseCodeLength) && (current_millis - previous_millis_endMorseCode >= interval_millis_endMorseCode)) {  //Si fin code morse et 5000 ms ecoulees alors raz index
    Serial.println("Let's go...");
    index_morseCodeDefault = 0;
  }
}

void flashDashDot(const char * morseCode)
{
  if (morseCode[index_morseSymbole] != 0) {             //Si le parcours de symbole n'est pas fini alors faire dash ou dot
    if (morseCode[index_morseSymbole] == '.') {
      dot();
    } else if (morseCode[index_morseSymbole] == '-') {
      dash();
    }
  } else if (morseCode[index_morseSymbole] == 0) {      //Sinon incrementer indexe code morse default
    index_morseCodeDefault++;
    index_morseSymbole = 0;
    previous_millis_symbole = millis();                 //Mise a jour compte a rebours temps symbole
    Serial.println();
  }
}

void dot()
{
  uint32_t current_millis = millis();
  
  if ((led_morse_state == LOW) && (current_millis - previous_millis_dot_or_dash >= interval_millis_dot))         //Debut dot, on passe la led a 1
  {
    Serial.print(".");
    led_morse_state = HIGH;  // turn it on
    previous_millis_dot = current_millis;   // Remember the time
    digitalWrite(LED_MORSE_PIN, led_morse_state);   // Update the actual LED
  } else if((led_morse_state == HIGH) && (current_millis - previous_millis_dot >= interval_millis_dot))  //Fin dot, on eteind la led et on incremente l'indexe de parcours du tableau char * symbole
  {
    led_morse_state = LOW;  // Turn it off
    previous_millis_dot_or_dash = current_millis;
    digitalWrite(LED_MORSE_PIN, led_morse_state);  // Update the actual LED
    index_morseSymbole++;
  }
}

void dash()
{
    uint32_t current_millis = millis();
  
  if ((led_morse_state == LOW) && (current_millis - previous_millis_dot_or_dash >= interval_millis_dot))         //Debut dash apres interval_millis_dot temps à l'etat bas, on passe la led a 1
  {
    Serial.print("-");
    led_morse_state = HIGH;  // turn it on
    previous_millis_dash = current_millis;   // Remember the time
    digitalWrite(LED_MORSE_PIN, led_morse_state);   // Update the actual LED
  } else if((led_morse_state == HIGH) && (current_millis - previous_millis_dash >= interval_millis_dash))  //Fin dot, on eteind la led et on incremente l'indexe de parcours du tableau char * symbole
  {
    led_morse_state = LOW;  // Turn it off
    previous_millis_dot_or_dash = current_millis;
    digitalWrite(LED_MORSE_PIN, led_morse_state);  // Update the actual LED
    index_morseSymbole++;
  }
}
