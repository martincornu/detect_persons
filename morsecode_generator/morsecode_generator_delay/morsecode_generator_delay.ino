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

const byte LED_PIN = A3;
int dotLength = 350;
int dashLength = dotLength*3;
const char* morseCodeDefault = "6401";
const byte morseCodeLength = 4;
int cpt = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  
  for (int cpt = 0; cpt < morseCodeLength; cpt++) {
  char ch = morseCodeDefault[cpt];
  Serial.print("Ch = ");
  Serial.println(ch);
  flashDashDot(MorseTable[ch]);
  delay(dotLength*3);
  }
  
  Serial.println("End of 6401...");
  delay(5000);
}

void flashDashDot(const char * morseCode)
{
  int i = 0;
  while(morseCode[i] != 0)
  {
    if(morseCode[i] == '.'){
      dot();
    } else if (morseCode[i] == '-'){
      dash();
    }
    i++;
  }
}

void dot()
{
  digitalWrite(LED_PIN, HIGH);
  delay(dotLength);
  digitalWrite(LED_PIN, LOW);
  delay(dotLength);
}

void dash()
{
  digitalWrite(LED_PIN, HIGH);
  delay(dashLength);
  digitalWrite(LED_PIN, LOW);
  delay(dotLength);
}
