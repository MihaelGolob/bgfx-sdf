# Upodabljanje pisave z uporabo predznačenega polja razdalje

Pomemben vidik video iger je vizualno privlačno prikazovanje besedila, ki se pogosto prilagaja različnim skaliranjem in ločljivostim zaslona. Da bi zagotovili jasno in nezamegljeno pisavo, moramo uporabiti velike slikovne atlase. [SDF](https://steamcdn-a.akamaihd.net/apps/valve/2007/SIGGRAPH2007_AlphaTestedMagnification.pdf) (Signed Distance Field) tehnika se izkaže kot rešitev, saj omogoča oster prikaz pisave tudi pri velikem povečanju, ob minimalni uporabi teksturnega prostora. Nadgradnja te metode, imenovana [MSDF](https://github.com/Chlumsky/msdfgen?tab=MIT-1-ov-file) (Multi-Channel Signed Distance Field), še dodatno izboljša natančnost, zlasti pri upodabljanju ostrejših kotov v črkah. Poleg tega tehnika SDF in MSDF zmanjšuje tudi overdraw, ki se pojavi pri generiranju različnih učinkov na pisavi, kot so sence, obrobe in mehki robovi, kar pripomore k izboljšanju splošne performance igre.

V diplomski nalogi bom podrobno preučil delovanje SDF in MSDF in ti dve metodi implementiral s pomočjo knjižnice za upodabljanje BGFX. Preveril bom ali je mogoče že znane implementacije izboljšati oz. pohitriti na koncu pa še evalviral SDF/MSDF z drugimi alternativnimi metodami, ki se uporabljajo v industriji.

![image](https://github.com/MihaelGolob/bgfx-sdf/assets/48100773/a13c8c66-32ef-4d40-99ea-fefc4b6ab70c)

## Navodila za izvajanje programa

- Klonirajte repozitorij (`git clone https://github.com/MihaelGolob/bgfx-sdf`)
- Prenesite vse module projekta (`git submodule update --init --recursive`)
- Odprite bgfx-sdf projekt z urejevalnikom, ki podpira Cmake (na primer JetBrains CLion). Alternativno lahko ročno prevedete kodo z ukazom `make`.
- V mapi `src` najdete vso izvorno kodo

## Navodila za prevajanje senčilnikov

- V direktoriju `src/shaders/` najdete vse senčilnike
- Če naredite v spremembo v senčilniku, ga morate prevesti, preden se bo sprememba poznala
- To naredite tako, da v programu `bash` v direktoriju `src/shaders/` poženete ukaz `make rebuild`
