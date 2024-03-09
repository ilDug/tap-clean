#include "lcd.h"

DagLCD::DagLCD(LCD_I2C *_lcd) {
  lcd = _lcd;
}

bool DagLCD::init(String version) {
  lcd->begin();                 // inizializza LCD
  lcd->backlight();             // accende lo sfondo
  lcd->home();                  // si posrta in prima riga
  lcd->print("DAG TAP-CLEAN");  // messaggio di benvenuto
  lcd->setCursor(0, 1);         // passa alla seconda riga
  lcd->print(version);          // stampa
  delay(3000);                  // pausa
  lcd->clear();                 // pulisce tutto lo schermo
  return true;
}

void DagLCD::mainPage(uint8_t program, uint16_t level) {
  lcd->clear();                             // cancella tutto lo schermo
  lcd->home();                              // prima riga e prima colonna
  lcd->print(programDescription(program));  // stampa la descrizione del programma
  lcd->setCursor(0, 1);                     // va a capo sulla seconda riga
  int lungh = map(level, 0, 1024, 1, 16);   // calcola la lunghezza della barra in base al valore del potenziometro
  String bar = "";                          // definisce i caratteri barra da stampare
  for (int i = 0; i < lungh; i++)           // ciclo per la lunghezza della barra in base al valore del potenziometro
  {
    bar = bar + "=";  // riempie la barra con i caratteri '+'
  }
  bar = bar + ">";
  lcd->print(bar);  // stampa la barra
}

String DagLCD::programDescription(uint8_t prg) {
  switch (prg) {
    case P1:
      return "DET + ANC + IGZ";
      break;
    case P2:
      return "AMMORBIDENTE";
      break;
    case P3:
      return "DET + ANTICALC";
      break;
    case CLN:
      return "CLEAN ALL";
      break;
    default:
      return "nessun PRG";
      break;
  }
}

void DagLCD::programProgress(Dispenser *dispenser) {
  lcd->clear();  // cancella tutto lo schermo
  lcd->home();   // prima riga e prima colonna

  lcd->print(dispenser->activeFlaskLabel + " " + dispenser->completion + "%");  // stampa il progresso del programma

  lcd->setCursor(0, 1);                                    // va a capo sulla seconda riga
  String bar = "";                                         // definisce i caratteri barra da stampare
  int percentage = int(dispenser->completion / 100 * 16);  // calcola la lunghezza della barra in base al valore del potenziometro
  for (int i = 0; i < percentage; i++)                     // ciclo per la lunghezza della barra in base al valore del potenziometro
  {
    bar = bar + "=";  // riempie la barra con i caratteri '+'
  }
  lcd->print(bar);  // stampa la barra
}