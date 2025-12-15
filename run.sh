#!/bin/bash

# Script per compilare ed eseguire il progetto

echo "=========================================="
echo "Compilazione progetto grafica"
echo "=========================================="
echo ""

# Pulizia build precedenti (opzionale, commentato di default)
# echo "Pulizia build precedenti..."
# make clean

# Compilazione
echo "Compilazione in corso..."
make

# Verifica se la compilazione è andata a buon fine
if [ $? -ne 0 ]; then
    echo ""
    echo "ERRORE: Compilazione fallita!"
    echo "Verifica che tutte le dipendenze siano installate correttamente."
    echo "Esegui: sudo ./install-dependencies.sh"
    exit 1
fi

echo ""
echo "=========================================="
echo "Compilazione completata!"
echo "=========================================="
echo ""

# Cambio directory in client
cd client || exit 1

echo "Esecuzione del client..."
echo ""
echo "=========================================="
echo ""

# Esecuzione con LD_LIBRARY_PATH corretto
LD_LIBRARY_PATH=../engine/bin/Release:../dependencies/freeimage/lib/x64/Release:$LD_LIBRARY_PATH ./bin/Release/client

# Salva il codice di uscita
EXIT_CODE=$?

echo ""
echo "=========================================="
if [ $EXIT_CODE -eq 0 ]; then
    echo "Programma terminato correttamente."
else
    echo "Programma terminato con codice di errore: $EXIT_CODE"
fi
echo "=========================================="

exit $EXIT_CODE
