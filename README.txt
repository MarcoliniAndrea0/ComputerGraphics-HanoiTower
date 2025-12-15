================================================================================
PROGETTO GRAFICA - GRUPPO 6
================================================================================

PREREQUISITI
------------
Per compilare ed eseguire il progetto su Linux è necessario avere installato:

1. Build tools:
   - g++ (con supporto C++20)
   - make

2. Librerie OpenGL:
   - libgl1-mesa-dev
   - libglu1-mesa-dev
   - freeglut3-dev

3. Libreria di imaging:
   - libfreeimage-dev

4. GLM (già incluso in dependencies/glm)


INSTALLAZIONE RAPIDA
---------------------
Per installare automaticamente tutte le dipendenze necessarie, eseguire:

    sudo ./install-dependencies.sh

Questo script installerà tutti i pacchetti richiesti sul sistema.


COMPILAZIONE ED ESECUZIONE
--------------------------
Per compilare ed eseguire il progetto in modalità Release:

    ./run.sh

Lo script si occuperà di:
1. Compilare l'engine (libengine.so)
2. Compilare il client
3. Configurare correttamente LD_LIBRARY_PATH
4. Eseguire l'applicazione


ESECUZIONE MANUALE
-------------------
Se si preferisce compilare ed eseguire manualmente:

1. Compilare il progetto:
   make

2. Spostarsi nella directory client:
   cd client

3. Eseguire il client:
   LD_LIBRARY_PATH=../engine/bin/Release:../dependencies/freeimage/lib/x64/Release:$LD_LIBRARY_PATH ./bin/Release/client


STRUTTURA DEL PROGETTO
----------------------
- engine/          : Libreria engine grafica (libengine.so)
- client/          : Applicazione client che utilizza l'engine
- dependencies/    : Dipendenze esterne (GLM, FreeImage, FreeGLUT)
- install-dependencies.sh : Script di installazione dipendenze
- run.sh           : Script di compilazione ed esecuzione


PULIZIA
-------
Per rimuovere tutti i file compilati:

    make clean

================================================================================
