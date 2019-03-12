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

  uint32_t current_millis = millis();
   
  /* --- Couple C1 et C2 --- */
  if (C1_C2_actives_10 == false) {
    /* Capteur 1 */
    distance_mm = mesure_distance(C1_TRIGGER_PIN,C1_ECHO_PIN);
    C1_personne_presente = is_personne_presente(distance_mm);
    //afficher_distance("C1", distance_mm);
  
    /* Capteur 2 */
    distance_mm = mesure_distance(C2_TRIGGER_PIN,C2_ECHO_PIN);
    C2_personne_presente = is_personne_presente(distance_mm);
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
    distance_mm = mesure_distance(C3_TRIGGER_PIN,C3_ECHO_PIN);
    C3_personne_presente = is_personne_presente(distance_mm);
    //afficher_distance("C3", distance_mm);
  
    /* Capteur 4 */
    distance_mm = mesure_distance(C4_TRIGGER_PIN,C4_ECHO_PIN);
    C4_personne_presente = is_personne_presente(distance_mm);
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
    distance_mm = mesure_distance(C5_TRIGGER_PIN,C5_ECHO_PIN);
    C5_personne_presente = is_personne_presente(distance_mm);
    //afficher_distance("C5", distance_mm);
  
    /* Capteur 6 */
    distance_mm = mesure_distance(C6_TRIGGER_PIN,C6_ECHO_PIN);
    C6_personne_presente = is_personne_presente(distance_mm);
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

  delay(50);
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

