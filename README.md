# Upravljanje potrošnjom energije mikrokontrolera korištenjem Sleep moda

## Sadržaj

* Opis zadatka
* Komponente
* Funkcionalnosti
* Usproedba sleep modova

## Opis zadatka

Sustav demonstrira upravljanje potrošnjom energije mikrokontrolera korištenjem Sleep moda. Projekt se sastoji od jednostavne Arduino aplikacije koja uključuje LED diodu i tipkalo. U aktivnoj fazi LED dioda svijetli određeno vrijeme, nakon čega mikrokontroler prelazi u sleep mod. Mikrokontroler se zatim budi putem:

* Prekida izazvanog pritiskom na tipkalo
* Watchdog timera (WDT) nakon određenog vremena

Nakon buđenja, sustav ponovno ulazi u aktivnu fazu i ciklus se ponavlja.

## Komponente

Projek koristi sljedeće komponente:

* Arduino Mega 2560
* Tipkalo
* LED dioda

| Komponenta        | Količina | Pin na Arduino Mega |
| ----------------- | -------- | ------------------- |
| Arduino Mega 2560 | 1        | -                   |
| Tipkalo           | 1        | 2                   |
| LED dioda         | 1        | 13                  |

## Funkcionalnosti


Nakon pokretanja, mikrokontroler uključuje LED diodu koja svijetli 5 sekundi, označavajući aktivni radni period sustava.

Nakon aktivnog perioda, mikrokontroler se prebacuje u sleep mod (SLEEP_MODE_PWR_DOWN) kako bi smanjio potrošnju energije. U tom stanju su svi nebitni moduli ugašeni, a sustav čeka na događaj buđenja.

Na digitalni pin 2 spojeno je tipkalo koje generira vanjski prekid kada se pritisne.
Pritiskom na tipkalo mikrokontroler se budi iz sleep moda, a zastavica isSleeping se postavlja na false.

Watchdog Timer (WDT) je konfiguriran da generira prekid nakon 4 sekunde, čime se također može probuditi mikrokontroler iz sleep moda.
Nakon buđenja, u serijskom monitoru se ispisuje poruka da je uređaj probuđen.

Nakon buđenja, sustav ponovno ulazi u aktivni period, LED svijetli 5 sekundi, a zatim se ciklus ponavlja – mikrokontroler ponovno ulazi u sleep.

## Usporedba sleep modova
| Sleep Mode     | Potrošnja energije | Načini buđenja                      |
| -------------- | ------------------ | ----------------------------------- |
| **Idle**       | Najmanja ušteda    | Timer, prekidi                      |
| **Power-down** | Najveća ušteda     | Eksterni prekid, WDT                |
| **Standby**    | Velika ušteda      | Eksterni prekid                     |

## Slika spoja
![zadatak2slika](https://github.com/user-attachments/assets/b3f3dfc5-6a5c-4c49-8294-cfe1adae2efc)
