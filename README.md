# tap-clean
Progetto arduino per la miscelazione dei detersivi.

## Funzionamento

Quattro pompe dosano il detersivo,  l'ammorbidente,  l'anticalcare e l'igenizzante in una tazza pronta per essere versata nel vassoio della lavatrice.

### Led

- PWR: apparecchio acceso
- DETERSIVO:  (lampeggia quando la pompa è attiva)
- ANTICALCARE: (lampeggia quando la pompa è attiva)
- IGENIZZANTE: (lampeggia quando la pompa è attiva)
- AMMORBIDENTE: (lampeggia quando la pompa è attiva)
- LEVEL: 3 led che indicano il livello di dosaggio


Otto led in totale che posso comandare con uno SHIFT_REGISTER.

### Programmi

Alla pressione del pulsante *PRG*  posso navigare tra i programmi. Ad un programma corrispondono una serie di led accesi che evidenziano quali sostanze saranno pompate .

| #            | P1 | P2 | P3 | CLN |
|--------------|----|----|----|-----|
| DETERSIVO    | X  |    | X  | X   |
| ANTICALCARE  | X  |    | X  | X   |
| IGENIZZANTE  | X  |    |    | X   |
| AMMORBIDENTE |    | X  |    | X   |

Alla pressione del pulsante *LVL* si possono selezionare tre livelli di dosaggio.A partire dal dosaggio medio, ogni pressione ciclicamente aumenta o diminuisce il livello.

Alla pressioen del pulsante *RUN* avvia il programma di miscelazione e attiva le pompe.