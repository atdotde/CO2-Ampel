# CO2-Ampel
CO2 Ampel auf Basis des MH-Z19B

Code stammt aus dem Beispielcode des Displays https://www.waveshare.com/wiki/0.96inch_LCD_Module
und einer schönen MH-Z19B-an-Arduino-Anleitung von Harald Angerer https://www.blikk.it/forum/blog.php?bn=neuemedien_fb&lang=de&id=1575901160 

Die Ampel zeigt grün bei einer CO2 Konzentration unter 500ppm und rot über 1000ppm sowie gelb dazwischen. Mehr Beschreibung unter https://homeschoolschwabing.blogspot.com/2020/09/co2-ampel-selber-bauen.html

Und ja, das hier ist noch eine ziemliche Unordnung. Der Display-Code ist ziemlich instabil, und auch der Event-Loop funktioniert nicht richtig, aber in setup() laeufts.
