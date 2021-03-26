/* 
 * Detecter 3x2 personnes qui sont sous les capteurs de distance HCSR04 en même temps.
 * Pour activer un couple de capteurs, il faut que les deux personnes resteent dessous 10 secondes
 * Des qu'un couple est valide, les joueurs ont 30 secs pour valider un autre couple de capteurs, sinon il faut recommencer.
 * Lorsque les 3 couples sont valides alors on desactive un electro aimant via un transistor pilote par AIMANT_PIN.
 * 
 * En parallele, on gere aussi l'eclairage d'une lampe en 230v via un relais par LED_MORSE_PIN.
 * La lampe reproduit le code morse morseCodeDefault par des series de on/off.
 */


/* -------------------- Variables detect_person -------------------- */

/* Distances */
const uint16_t distance_capteur_sol_mm = 2300;
const uint16_t taille_min_mm = 1000;

/* Constantes pour les broches */
const byte AIMANT_PIN = A0;    // Broche aimant, analog utilisee comme digital car manque de pin
const byte C1_C2_LED_PIN = A1; // Led qui s'allume si C1 et C2 sont actives
const byte C3_C4_LED_PIN = A2; // Led qui s'allume si C3 et C4 sont actives
const byte C5_C6_LED_PIN = A3; // Led qui s'allume si C5 et C6 sont actives

const byte B1_PIN = 2; // Broche TRIGGER
const byte B2_PIN = 3;    // Broche ECHO
const byte B3_PIN = 4; // Broche TRIGGER
const byte B4_PIN = 5;    // Broche ECHO
const byte B5_PIN = 6; // Broche TRIGGER
const byte B6_PIN = 7; // Broche TRIGGER
 
/* Constantes pour le timeout */
const unsigned long MEASURE_TIMEOUT = 25000UL; // 25ms = ~8m à 340m/s

/* Vitesse du son dans l'air en mm/us */
const float SOUND_SPEED = 340.0 / 1000;

/* Flags personne_presente */
bool C1_personne_presente = false;
bool C2_personne_presente = false;
bool C3_personne_presente = false;
bool C4_personne_presente = false;
bool C5_personne_presente = false;
bool C6_personne_presente = false;

/* Flags couple de capteurs activés */
bool C1_C2_actives = false;
bool C3_C4_actives = false;
bool C5_C6_actives = false;
bool C1_C2_actives_10 = false;
bool C3_C4_actives_10 = false;
bool C5_C6_actives_10 = false;

/* Flags passage boucle */
bool C1_C2_first_pass = true;
bool C3_C4_first_pass = true;
bool C5_C6_first_pass = true;

/* Variables globales */ 
float distance_mm = 0;
uint32_t C1_C2_start_millis = 0;
uint32_t C3_C4_start_millis = 0;
uint32_t C5_C6_start_millis = 0;
uint32_t etape_30_start_millis = 0;


/* -------------------- Variables morse_generator_no_delay -------------------- */
/* Arbre code morse */
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

const byte LED_MORSE_PIN = A4;
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


/* -------------------- Fonction setup() -------------------- */
void setup() {
   
  /* Initialise le port série */
  //Serial.begin(9600);
  //Serial.println("Initialisation...");

  /* Initialise les broches code morse */
  pinMode(LED_MORSE_PIN, OUTPUT);
   
  /* Initialise les broches detect_person */
  pinMode(AIMANT_PIN, OUTPUT);
  digitalWrite(AIMANT_PIN, HIGH); // La broche aimant doit être à HIGH au repos

  pinMode(C1_C2_LED_PIN, OUTPUT);
  digitalWrite(C1_C2_LED_PIN, LOW);

  pinMode(C3_C4_LED_PIN, OUTPUT);
  digitalWrite(C3_C4_LED_PIN, LOW);

  pinMode(C5_C6_LED_PIN, OUTPUT);
  digitalWrite(C5_C6_LED_PIN, LOW);
  
  pinMode(B1_PIN, INPUT);
  pinMode(B2_PIN, INPUT);
  pinMode(B3_PIN, INPUT);
  pinMode(B4_PIN, INPUT);
  pinMode(B5_PIN, INPUT);
  pinMode(B6_PIN, INPUT);
  
}

/* -------------------- Fonction loop() -------------------- */
void loop() {

  //Serial.println("Mesure distance...");

  uint32_t current_millis = millis();

  /* -- Code pour la lampe morse --- */
  if ((index_morseCodeDefault < morseCodeLength) && (current_millis - previous_millis_symbole >= interval_millis_symbole) ) { //Si on a pas fini de parcourir le code morse
                                                                                                                //et que l'interval de temps entre deux symboles (3* dotLength) est respecte alors
    char ch = morseCodeDefault[index_morseCodeDefault];
    const char * morseCodeSymbole = MorseTable[ch];
    flashDashDot(morseCodeSymbole);

    if (index_morseCodeDefault == morseCodeLength) {                    //Declencher compte a rebour 5000 ms si fin code morse
      //Serial.println("End of 6401...");
      previous_millis_endMorseCode = millis(); 
    }
    
  } else if ((index_morseCodeDefault == morseCodeLength) && (current_millis - previous_millis_endMorseCode >= interval_millis_endMorseCode)) {  //Si fin code morse et 5000 ms ecoulees alors raz index
    //Serial.println("Let's go...");
    index_morseCodeDefault = 0;
  }

  /* --- Code pour detect_person --- */
  
  /* --- Couple C1 et C2 --- */
  if (C1_C2_actives_10 == false) {
    /* Capteur 1 */
    C1_personne_presente = (digitalRead(B1_PIN) == HIGH);
    //afficher_distance("C1", distance_mm);
  
    /* Capteur 2 */ 
    //distance_mm = mesure_distance(C2_TRIGGER_PIN,C2_ECHO_PIN);
    C2_personne_presente = (digitalRead(B2_PIN) == HIGH);
    //afficher_distance("C2", distance_mm);

    C1_C2_actives = is_couple_present(C1_personne_presente, C2_personne_presente);
    
    C1_C2_actives_10 = check_presence_10_sec(C1_C2_actives, &C1_C2_first_pass, current_millis, &C1_C2_start_millis, C1_C2_LED_PIN);

    /* Si le couple vient d'etre active maj timer etape 30 secondes */
    if (C1_C2_actives_10) {
      etape_30_start_millis = current_millis;  
    }
  }
  

  /* --- Couple C3 et C4 --- */
  if (C3_C4_actives_10 == false) {
    /* Capteur 3 */
    //distance_mm = mesure_distance(C3_TRIGGER_PIN,C3_ECHO_PIN);
    C3_personne_presente = (digitalRead(B3_PIN) == HIGH);
    //afficher_distance("C3", distance_mm);
  
    /* Capteur 4 */
    //distance_mm = mesure_distance(C4_TRIGGER_PIN,C4_ECHO_PIN);
    C4_personne_presente = (digitalRead(B4_PIN) == HIGH);
    //afficher_distance("C4", distance_mm);

    C3_C4_actives = is_couple_present(C3_personne_presente, C4_personne_presente);

    C3_C4_actives_10 = check_presence_10_sec(C3_C4_actives, &C3_C4_first_pass, current_millis, &C3_C4_start_millis, C3_C4_LED_PIN);

    /* Si le couple vient d'etre active maj timer etape 30 secondes */
    if (C3_C4_actives_10) {
      etape_30_start_millis = current_millis;
    }
  }

    /* --- Couple C3 et C4 --- */
  if (C5_C6_actives_10 == false) {
    /* Capteur 5 */
    //distance_mm = mesure_distance(C5_TRIGGER_PIN,C5_ECHO_PIN);
    C5_personne_presente = (digitalRead(B5_PIN) == HIGH);
    //afficher_distance("C5", distance_mm);
  
    /* Capteur 6 */
    //distance_mm = mesure_distance(C6_TRIGGER_PIN,C6_ECHO_PIN);
    C6_personne_presente = (digitalRead(B6_PIN) == HIGH);
    //afficher_distance("C6", distance_mm);

    C5_C6_actives = is_couple_present(C5_personne_presente, C6_personne_presente);
    
    C5_C6_actives_10 = check_presence_10_sec(C5_C6_actives, &C5_C6_first_pass, current_millis, &C5_C6_start_millis, C5_C6_LED_PIN);

    /* Si le couple vient d'etre active maj timer etape 30 secondes */
    if (C5_C6_actives_10) {
      etape_30_start_millis = current_millis;
    }
  }

  /* Verifier que la derniere activation etait il y a moins de 30 secondes, si non, tout remettre à zero */
  if (C1_C2_actives_10 || C3_C4_actives_10 || C5_C6_actives_10) {
    if ((current_millis - etape_30_start_millis) >= 30000) {
      raz();
    }
  }

  /* Si tous les capteurs sont actives alors desactiver electro aimant */
  if (C1_C2_actives_10 && C3_C4_actives_10 && C5_C6_actives_10) {
    //Serial.println("Electro aimant descative !");
    digitalWrite(AIMANT_PIN, LOW);
    delay(5000);
    exit(0);
  }
}

/* -------------------- Fonctions detect_person -------------------- */

/* Mesure distance */
float mesure_distance (const byte trigger_pin, const byte echo_pin) {
  /* 1. Lance une mesure de distance en envoyant une impulsion HIGH de 10µs sur la broche TRIGGER */
  digitalWrite(trigger_pin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger_pin, LOW);
  
  /* 2. Mesure le temps entre l'envoi de l'impulsion ultrasonique et son écho (si il existe) */
  long measure = pulseIn(echo_pin, HIGH, MEASURE_TIMEOUT);
   
  /* 3. Calcul la distance à partir du temps mesuré */
  float distance_mm = measure / 2.0 * SOUND_SPEED;

  return distance_mm;
}

bool is_couple_present(bool captA_personne_presente, bool captB_personne_presente) {
  /* 5. Activer flag couple si deux personnes sont detectees */
  if (captA_personne_presente == true && captB_personne_presente == true){
    //Serial.println("Deux personnes ont active un couple de capteurs");
    return true;
  } else {
    return false;
  }
}

bool is_personne_presente(float distance_mm) {
  /* 4. Activer flag personne_presente si distance <= à distance capteur sol - distance taille humain min */
  if (distance_mm <= (distance_capteur_sol_mm - taille_min_mm)) {
    //Serial.println("Personne presente");
    return true;
  } else {
    return false;
  }
}

/* Verifie si la personne reste au moins 10 secondes sous le capteur, retourne vrai si plus de 10sec ou faux sinon */
bool check_presence_10_sec(bool actives, bool * first_pass, uint32_t current_millis, uint32_t * start_millis, const byte led_pin) {
  bool presence_10_sec = false;
  if (actives) {
    if(*first_pass == true) {                               /* Si la personne vient detre detectee */
      *start_millis = millis();                             /* On lance le decompte */
      *first_pass = false;
    } else if ((current_millis - *start_millis) >= 10000) { /* Si il sest ecoule plus de 10 secondes alors */
      digitalWrite(led_pin, HIGH);
      presence_10_sec = true;
    }
  } else {
    *start_millis = 0;                                      /* Si la personne s'enleve de sous le capteur alors */
    *first_pass = true;                                     /* Remettre à 0 le decompte */
  }

  return presence_10_sec;
}

/* Verifie si la derniere etape d'activation du couple de capteur etait il y a plus de 30 secondes, retourne vrai si plus de 30sec ou faux sinon */
bool check_etape_30_sec(bool actives_10, bool * first_pass, uint32_t current_millis, uint32_t * start_millis) {
  bool etape_30_sec = false;
  if (actives_10) {
    if(*first_pass == true) {
      *start_millis = millis();
      *first_pass = false;
    }
  }
  
  return etape_30_sec;
}

/* Affiche les résultats en mm, cm et m */
void afficher_distance(String capt, float distance_mm) {
    /*Serial.print(capt + " = ");
    Serial.print(F("Distance: "));
    Serial.print(distance_mm);
    Serial.print(F("mm ("));
    Serial.print(distance_mm / 10.0, 2);
    Serial.print(F("cm, "));
    Serial.print(distance_mm / 1000.0, 2);
    Serial.println(F("m)"));*/
}

/* Remise à zero des variables et des flags */
void raz() {
  C1_personne_presente = false;
  C2_personne_presente = false;
  C3_personne_presente = false;
  C4_personne_presente = false;
  C5_personne_presente = false;
  C6_personne_presente = false;

  C1_C2_actives = false;
  C3_C4_actives = false;
  C5_C6_actives = false;
  C1_C2_actives_10 = false;
  C3_C4_actives_10 = false;
  C5_C6_actives_10 = false;

  C1_C2_first_pass = true;
  C3_C4_first_pass = true;
  C5_C6_first_pass = true;

  distance_mm = 0;
  C1_C2_start_millis = 0;
  C3_C4_start_millis = 0;
  C5_C6_start_millis = 0;
  etape_30_start_millis = 0;

  digitalWrite(C1_C2_LED_PIN, LOW);
  digitalWrite(C3_C4_LED_PIN, LOW);
  digitalWrite(C5_C6_LED_PIN, LOW);
}

/* -------------------- Fonctions morse_generator_no_delay -------------------- */

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
    //Serial.println();
  }
}

void dot()
{
  uint32_t current_millis = millis();
  
  if ((led_morse_state == LOW) && (current_millis - previous_millis_dot_or_dash >= interval_millis_dot))         //Debut dot, on passe la led a 1
  {
    //Serial.print(".");
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
    //Serial.print("-");
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

