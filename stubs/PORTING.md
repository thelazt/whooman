Portierung auf StuBS
====================

Das Spiel lässt sich sehr einfach (in wenigen Minuten) auf das eigene *StuBS* portieren -- sofern man die [Pflichtaufgaben](https://www4.cs.fau.de/Lehre/WS20/V_BS/Uebungen/#aufgaben), Aufgabe 7 sowie die Maus implementiert hat.
Und natürlich weder zu viel der Interfaces modifiziert hat noch fiese Fehler in seinem Code hat ;)


Eingabe
-------

Da im Spiel der aktuelle Zustand der Tasten direkt geprüft wird (via `PS2Controller::isPressed()`), wird weder Epilog noch `getKey` mehr gebraucht -- der Prolog (mit Behandlung des [Affengriffs](https://de.wikipedia.org/wiki/Klammergriff)) wird jedoch weiterhin gebraucht.

Außerdem muss die [Mausunterstützung](https://www4.cs.fau.de/Lehre/WS20/V_BS/Uebungen/aufgabe2/a2.shtml#autotoc_md31) aktiviert sein; der Treiber braucht sich nicht (mehr) um die absolute Position der Maus kümmern, sondern muss nur in der Lage sein die relative Änderung seit dem letzten Abruf via `Mouse::delta` zurückzugeben (ein Epilog ist somit nicht notwendig):

	int delta_x, delta_y;
	bool delta_button;

	bool Mouse::prologue() {
		bool update = PS2Controller::fetch(state);
		delta_x += state.x;
		delta_y += state.y;
		delta_button = delta_button || state.button[MOUSE_LEFT];
		return false;
	}

	bool Mouse::delta(int & x, int & y) {
		bool status = Core::Interrupt::disable();
		x = delta_x;
		y = delta_y;
		delta_x = delta_y = 0;
		bool pressed = delta_button;
		delta_button = false;
		Core::Interrupt::restore(status);
		return pressed;
	}


Ausgabe
-------

Das Herzstück dieser Anwendung ist natürlich die Grafik.
Deshalb sind hier auch die meisten Anpassungen notwendig,..

###  Passenden Grafikmodus setzen

Das Spiel läuft (für beste Unterstützung bei VBE) mit einer Auflösung von 1024×768 Pixel bei 32bit Farbtiefe.

Dies muss entsprechend in `boot/multiboot/config.inc` angegeben werden:

	MULTIBOOT_HEADER_FLAGS equ MULTIBOOT_VIDEO_MODE
	
	; Desired video mode (only considered if MULTIBOOT_VIDEO_MODE set)
	; (boot loader will choose the best fitting mode, which might differ from the settings below)
	MULTIBOOT_VIDEO_WIDTH equ 1024    ; Desired width
	MULTIBOOT_VIDEO_HEIGHT equ 768    ; Desired height
	MULTIBOOT_VIDEO_BITDEPTH equ 32   ; Desired bit depth

Und auch in der `main` angepasst werden:

	// Maximum visible display
	const unsigned fb_width  = 1024;  ///< screen width
	const unsigned fb_height = 768;   ///< screen height
	const unsigned fb_bpp    = 32;    ///< bits per pixel

	const unsigned fb_size = fb_width * fb_height * fb_bpp / sizeof(char);
	char buffer_1[fb_size] __attribute__((aligned));
	char buffer_2[fb_size] __attribute__((aligned));
	GuardedGraphics graphics(fb_size, buffer_1, buffer_2);


### Pufferkopie

Unser *StuBS* verwendet für die flüssige Darstellung der Bildschirmausgabe eine Doppelpufferung.
Da sich große Teile des Bildschirms während einer Spielrunde nicht ändern, werden diese nur zu Beginn einmal gezeichnet.
Entsprechend muss nun die Funktion `Graphics::switchBuffers` so angepasst werden, dass sie den Inhalt nach dem initialen kompletten Zeichnen auf den zweiten Puffer kopiert -- gesteuert über dem Parameter `duplicate`:

	bool Graphics::switchBuffers(bool duplicate) {
		if (!refresh || duplicate) {
			printer->buffer(buffer[scanout_buffer]);
			if (duplicate) {
				memcpy(buffer[scanout_buffer], buffer[1 - scanout_buffer], size);
			}
			// Switch
			scanout_buffer = 1 - scanout_buffer;
			refresh = true;
			return true;
		} else {
			return false;
		}
	}

Dieser Parameter muss auch über `GuardedGraphics` verwendet werden können.


### Debugausgabe via serielle Schnittstelle (optional)

Da dadurch auch der Textmodus nicht mehr verwendet wird, ist es ggf. empfehlenswert die Fehlerausgabe über die [serielle Schnittstelle](https://www4.cs.fau.de/Lehre/WS20/V_BS/Uebungen/aufgabe2/a2.shtml#autotoc_md30) auszugeben, bei *OOStuBS* z.B. via

	#include "device/serialstream.h"
	SerialStream dout;


Bitte Beachten: Aufgrund von via Multiboot gesetzten Grafikmodus (VBE) lässt sich das Spiel nicht mehr direkt via `--kernel` Parameter von Qemu/KVM starten, sondern muss als ISO geladen werden.


### Grafiken via initiale Ramdisk

Der Grafikordner `img` muss in das Verzeichnis `initrd` kopiert werden. 
Die `Makefile` im Wurzelordner macht dies automatisch beim Target `iso` (und führt dabei noch von der PNG-Bibliothek unterstützten Optimierungen durch).

Da die Grafiken (*Sprites*) für das Spiel jedoch ggf. größer als die standardmäßig konfigurierten 1 MB der initialen Ramdisk sind, sollte hier etwas mehr Platz konfiguriert werden, z.B. 4 MB (`INITRD_FREE ?= 4194304` in der *StuBS* `Makefile` -- wird automatisch über das `iso` Target im Hauptordner des Repos erledigt).


Für ausreichend Speicher sorgen
-------------------------------

Wir brauchen genug Stack für die Threads, z.B. in `thread/thread.h` auf mindestens 128 KB setzen:

	static const size_t STACK_SIZE = 131072

Ebenfalls sollte der (*Dynamic Memory Allocator* aus Aufgabe 7)[https://www4.cs.fau.de/Lehre/WS20/V_BS/Uebungen/aufgabe7/a7.shtml#autotoc_md70] in der Lage sein bis zu 64 MB Speicher zu vergeben (alle Grafiken liegen als unkomprimierte *Bitmap* im Speicher!).


Spiel in StuBS laden
--------------------

In der `main` muss nun der Thread für das Spiel erstellt werden:

	#include "user/game/whooman.h"

	extern "C" int main() {

		// ... 

		WHOOMan * wm = new WHOOMan();
		Scheduler::ready(wm);
		
		// ...
	}


Weitere Optimierungen
---------------------

Andere Anwendungen (aus vorherigen Aufgaben) sollten deaktiviert werden.
Entsprechend gibt es eigentlich auch keinen Grund mehr für das Scheduling im `Watch`-Epilog, es ist ausreichend hier nur `Bellringer::check()` sowie alle 24ms (für 42 FPS) `Graphics::scanoutFrontbuffer()` aufzurufen.

Andere optionalen Aufgabe wie [RTC oder HPET](https://www4.cs.fau.de/Lehre/WS20/V_BS/Uebungen/aufgabe5/a5.shtml) sollten besser deaktiviert werden.

