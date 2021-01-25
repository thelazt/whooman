WHOOMan - ein Bomberman-Klon in (OO)StuBS
=========================================

Dieses Spiel, basierend auf dem aktuellen (Stand Januar 2021) 
[64-bit StuBS](https://gitlab.cs.fau.de/i4/bs/stubs),
soll die Möglichkeiten von etwas komplexeren Anwendungen für die freiwillige 
[Aufgabe 7](https://www4.cs.fau.de/Lehre/WS20/V_BS/Uebungen/aufgabe7/a7.shtml)
im Fach Betriebssystem demonstrieren.

Es wird dabei die neue (schnellere) Grafikbibliothek mit der Möglichkeit 
Bilder im *PNG* Format vom Dateisystem zu laden ausgiebig genutzt, neben
weiteren Zusatzaufgaben wie der Mausunterstützung.

Dabei sind die Modifikationen am StuBS Kern minimal, für das Spiel selbst
mussten keine weiteren Hardwaretreiber implementiert werden -- lediglich die
Spielmechanik mit ihrer grafischen Ausgabe sowie das Menü.

Dabei wurde (mangels Talent) auf vorhande Bilddateien (zum Teil aus 
[SNES Roms](https://www.spriters-resource.com/snes/sbomber/sheet/60462/))
zurückgegriffen und für das Spiel [angepasst](https://de.wikipedia.org/wiki/Hqx).

Zur einfacheren Entwicklung wurden die Ein- und Ausgabeschnittstellen mittels
[SDL](https://de.wikipedia.org/wiki/Simple_DirectMedia_Layer) nachgeahmt und
das Spiel zuerst unter einem aktuellen Linux entwickelt, bevor mti den
Anpassungen für StuBS begonnen wurde.

<img src="screenshot_1.png"  width="256" height="192">
<img src="screenshot_2.png"  width="256" height="192">
<img src="screenshot_3.png"  width="256" height="192">


Benutzung
---------

Um die SDL-Anwendung zu erstellen & starten, einfach

    make bomber.sdl
    ./bomber.sdl

ausführen (dazu muss natürlich die *SDL* Bibliothek lokal installiert sein).

Um das Spiel im eigenen *StuBS* laufen zu lassen, bitte zuerst die
[Portierungsanleitung](stubs/PORTING.md) befolgen.
Danach kann die bootfähige ISO mittels

    make bomber.iso

erstellt werden. Dabei wird natürlich die
[Entwicklungsumgebung für StuBS](https://www4.cs.fau.de/Lehre/WS20/V_BS/Uebungen/aufgabe1/env.shtml)
inklusive *GRUB* und *Xorriso* vorausgesetzt.

Das erstellte Abbild kann auch direkt in *Qemu* (mit Hardwarebeschleunigung)
gestartet werden:

    make kvm


Autor
-----

Ein kleines Hobbyprojekt, stückchenweise programmiert (mit langenen Pausen)
zwischen Anfang 2018 und 2021 von [Bernhard Heinloth](mailto:bernhard@heinloth.net).

Die Rechte am Spiel Bomberman und den zugrunde liegenden Spielprinzip sowie den
Grafiken liegen bei Hudson/Konami und Nintendo.
