/* 
 * Detecter deux personnes qui sont sous les capteurs de distance HCSR04 en même temps.
 */

/* Distances */
const uint16_t distance_capteur_sol_mm = 2300;
const uint16_t taille_min_mm = 1000;

/* Constantes pour les broches */
const byte AIMANT_PIN = A0;    // Broche aimant, analog utilisee comme digital car manque de pin
const byte C1_C2_LED_PIN = A1; // Led qui s'allume si C1 et C2 sont actives
const byte C3_C4_LED_PIN = A2; // Led qui s'allume si C3 et C4 sont actives
const byte C5_C6_LED_PIN = A3; // Led qui s'allume si C5 et C6 sont actives

const byte C1_TRIGGER_PIN = 2; // Broche TRIGGER
const byte C1_ECHO_PIN = 3;    // Broche ECHO

const byte C2_TRIGGER_PIN = 4; // Broche TRIGGER
const byte C2_ECHO_PIN = 5;    // Broche ECHO

const byte C3_TRIGGER_PIN = 6; // Broche TRIGGER
const byte C3_ECHO_PIN = 7;    // Broche ECHO

const byte C4_TRIGGER_PIN = 8; // Broche TRIGGER
const byte C4_ECHO_PIN = 9;    // Broche ECHO

const byte C5_TRIGGER_PIN = 10; // Broche TRIGGER
const byte C5_ECHO_PIN = 11;    // Broche ECHO

const byte C6_TRIGGER_PIN = 12; // Broche TRIGGER
const byte C6_ECHO_PIN = 13;    // Broche ECHO
 
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

/* Variables globales */ 
float distance_mm = 0;

/** --- Fonction setup() --- */
void setup() {
   
  /* Initialise le port série */
  //Serial.begin(9600);
  //Serial.println("Initialisation...");
   
  /* Initialise les broches */
  pinMode(AIMANT_PIN, OUTPUT);
  digitalWrite(AIMANT_PIN, HIGH); // La broche aimant doit être à HIGH au repos

  pinMode(C1_C2_LED_PIN, OUTPUT);
  digitalWrite(C1_C2_LED_PIN, LOW);

  pinMode(C3_C4_LED_PIN, OUTPUT);
  digitalWrite(C3_C4_LED_PIN, LOW);

  pinMode(C5_C6_LED_PIN, OUTPUT);
  digitalWrite(C5_C6_LED_PIN, LOW);
  
  pinMode(C1_TRIGGER_PIN, OUTPUT);
  digitalWrite(C1_TRIGGER_PIN, LOW); // La broche TRIGGER doit être à LOW au repos
  pinMode(C1_ECHO_PIN, INPUT);

  pinMode(C2_TRIGGER_PIN, OUTPUT);
  digitalWrite(C2_TRIGGER_PIN, LOW); // La broche TRIGGER doit être à LOW au repos
  pinMode(C2_ECHO_PIN, INPUT);

  pinMode(C3_TRIGGER_PIN, OUTPUT);
  digitalWrite(C3_TRIGGER_PIN, LOW); // La broche TRIGGER doit être à LOW au repos
  pinMode(C3_ECHO_PIN, INPUT);

  pinMode(C4_TRIGGER_PIN, OUTPUT);
  digitalWrite(C4_TRIGGER_PIN, LOW); // La broche TRIGGER doit être à LOW au repos
  pinMode(C4_ECHO_PIN, INPUT);

  pinMode(C5_TRIGGER_PIN, OUTPUT);
  digitalWrite(C5_TRIGGER_PIN, LOW); // La broche TRIGGER doit être à LOW au repos
  pinMode(C5_ECHO_PIN, INPUT);

  pinMode(C6_TRIGGER_PIN, OUTPUT);
  digitalWrite(C6_TRIGGER_PIN, LOW); // La broche TRIGGER doit être à LOW au repos
  pinMode(C6_ECHO_PIN, INPUT);
}
 
/** --- Fonction loop() --- */
void loop() {

  //Serial.println("Mesure distance...");
   
  /* --- Couple C1 et C2 --- */
  if (C1_C2_actives == false) {
    /* Capteur 1 */
    distance_mm = mesure_distance(C1_TRIGGER_PIN,C1_ECHO_PIN);
    C1_personne_presente = is_personne_presente(distance_mm);
    //afficher_distance("C1", distance_mm);
  
    /* Capteur 2 */
    distance_mm = mesure_distance(C2_TRIGGER_PIN,C2_ECHO_PIN);
    C2_personne_presente = is_personne_presente(distance_mm);
    //afficher_distance("C2", distance_mm);

    C1_C2_actives = is_couple_present(C1_personne_presente, C2_personne_presente);

    if (C1_C2_actives) {
      digitalWrite(C1_C2_LED_PIN, HIGH);
    }
  }
  

  /* --- Couple C3 et C4 --- */
  if (C3_C4_actives == false) {
    /* Capteur 3 */
    distance_mm = mesure_distance(C3_TRIGGER_PIN,C3_ECHO_PIN);
    C3_personne_presente = is_personne_presente(distance_mm);
    //afficher_distance("C3", distance_mm);
  
    /* Capteur 4 */
    distance_mm = mesure_distance(C4_TRIGGER_PIN,C4_ECHO_PIN);
    C4_personne_presente = is_personne_presente(distance_mm);
    //afficher_distance("C4", distance_mm);

    C3_C4_actives = is_couple_present(C3_personne_presente, C4_personne_presente);

    if (C3_C4_actives) {
      digitalWrite(C3_C4_LED_PIN, HIGH);
    }
  }

    /* --- Couple C3 et C4 --- */
  if (C5_C6_actives == false) {
    /* Capteur 5 */
    distance_mm = mesure_distance(C5_TRIGGER_PIN,C5_ECHO_PIN);
    C5_personne_presente = is_personne_presente(distance_mm);
    //afficher_distance("C5", distance_mm);
  
    /* Capteur 6 */
    distance_mm = mesure_distance(C6_TRIGGER_PIN,C6_ECHO_PIN);
    C6_personne_presente = is_personne_presente(distance_mm);
    //afficher_distance("C6", distance_mm);

    C5_C6_actives = is_couple_present(C5_personne_presente, C6_personne_presente);

    if (C5_C6_actives) {
      digitalWrite(C5_C6_LED_PIN, HIGH);
    }
  }

  /* Si tous les capteurs sont actives alors desactiver electro aimant */
  if (C1_C2_actives && C3_C4_actives && C5_C6_actives) {
    //Serial.println("Electro aimant descative !");
    digitalWrite(AIMANT_PIN, LOW);
    delay(5000);
    exit(0);
  }

  delay(100);
}


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

