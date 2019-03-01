/* 
 * Detecter deux personnes qui sont sous les capteurs de distance HCSR04 en même temps.
 */

/* Constantes pour les broches */
const byte C1_TRIGGER_PIN = 4; // Broche TRIGGER
const byte C1_ECHO_PIN = 5;    // Broche ECHO
const byte C2_TRIGGER_PIN = 2; // Broche TRIGGER
const byte C2_ECHO_PIN = 3;    // Broche ECHO
 
/* Constantes pour le timeout */
const unsigned long MEASURE_TIMEOUT = 25000UL; // 25ms = ~8m à 340m/s

/* Vitesse du son dans l'air en mm/us */
const float SOUND_SPEED = 340.0 / 1000;

/* Flags personne_presente */
bool C1_personne_presente = false;
bool C2_personne_presente = false;

/* Flags couple de capteurs activés */
bool C1_C2_actives = false;

/** Fonction setup() */
void setup() {
   
  /* Initialise le port série */
  Serial.begin(115200);
   
  /* Initialise les broches */
  pinMode(C1_TRIGGER_PIN, OUTPUT);
  digitalWrite(C1_TRIGGER_PIN, LOW); // La broche TRIGGER doit être à LOW au repos
  pinMode(C1_ECHO_PIN, INPUT);

  pinMode(C2_TRIGGER_PIN, OUTPUT);
  digitalWrite(C2_TRIGGER_PIN, LOW); // La broche TRIGGER doit être à LOW au repos
  pinMode(C2_ECHO_PIN, INPUT);
}
 
/** Fonction loop() */
void loop() {

  if (C1_C2_actives == false) {
    /* Capteur 1 */
    /* 1. Lance une mesure de distance en envoyant une impulsion HIGH de 10µs sur la broche TRIGGER */
    digitalWrite(C1_TRIGGER_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(C1_TRIGGER_PIN, LOW);
    
    /* 2. Mesure le temps entre l'envoi de l'impulsion ultrasonique et son écho (si il existe) */
    long measure = pulseIn(C1_ECHO_PIN, HIGH, MEASURE_TIMEOUT);
     
    /* 3. Calcul la distance à partir du temps mesuré */
    float distance_mm = measure / 2.0 * SOUND_SPEED;
  
    /* 4. Activer flag personne_presente si distance < 1000 mm */
    if (distance_mm <= 1000) {
      Serial.println(distance_mm / 10.0, 2);
      C1_personne_presente = true;
    } else {
      C1_personne_presente = false;
    }
  
    /* Capteur 2 */
    /* 1. Lance une mesure de distance en envoyant une impulsion HIGH de 10µs sur la broche TRIGGER */
    digitalWrite(C2_TRIGGER_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(C2_TRIGGER_PIN, LOW);
    
    /* 2. Mesure le temps entre l'envoi de l'impulsion ultrasonique et son écho (si il existe) */
    measure = pulseIn(C2_ECHO_PIN, HIGH, MEASURE_TIMEOUT);
     
    /* 3. Calcul la distance à partir du temps mesuré */
    distance_mm = measure / 2.0 * SOUND_SPEED;
  
    /* 4. Activer flag personne_presente si distance < 1000 mm */
    if (distance_mm <= 1000) {
      Serial.println(distance_mm / 10.0, 2);
      C2_personne_presente = true;
    } else {
      C2_personne_presente = false;
    }
  }

  /* 5. Activer flag couple si deux personnes sont detectees */
  if (C1_personne_presente == true && C2_personne_presente == true){
    C1_C2_actives = true;
    Serial.println("Deux personnes ont active les capteurs");
  }
   
  /* Affiche les résultats en mm, cm et m */
  /*Serial.print(F("Distance: "));
  Serial.print(distance_mm);
  Serial.print(F("mm ("));
  Serial.print(distance_mm / 10.0, 2);
  Serial.print(F("cm, "));
  Serial.print(distance_mm / 1000.0, 2);
  Serial.println(F("m)"));*/
   
  /* Délai d'attente pour éviter d'afficher trop de résultats à la seconde */
  delay(50);
}
