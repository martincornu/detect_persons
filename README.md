DETECTEUR DE POSITION DES JOUEURS + GENERATION CODE MORSE

 * Détecter 3x2 joueurs qui sont sous les capteurs de distance HCSR04 en même temps.
 * Pour activer un couple de capteurs, il faut que les deux joueurs restent dessous 10 secondes
 * Des qu'un couple est valide, les joueurs ont 30 secs pour valider un autre couple de capteurs, sinon il faut recommencer.
 * Lorsque les 3 couples sont valides alors on désactive un électro aimant via un transistor pilote par AIMANT_PIN.
 * En parallèle, on gère aussi l'éclairage d'une lampe en 230v via un relais par LED_MORSE_PIN.
 * La lampe reproduit le code morse morseCodeDefault par des séries de on/off.