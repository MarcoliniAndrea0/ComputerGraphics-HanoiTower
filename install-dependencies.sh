#!/bin/bash

# Script per installare le dipendenze necessarie per il progetto su Linux

echo "=========================================="
echo "Installazione dipendenze progetto grafica"
echo "=========================================="
echo ""

# Verifica se il comando è eseguito con privilegi di amministratore
if [ "$EUID" -ne 0 ]; then
    echo "ATTENZIONE: Questo script richiede privilegi di amministratore."
    echo "Rieseguilo con: sudo ./install-dependencies.sh"
    exit 1
fi

echo "Aggiornamento repository..."
apt-get update

echo ""
echo "Installazione build tools..."
apt-get install -y build-essential make g++

echo ""
echo "Installazione librerie OpenGL..."
apt-get install -y libgl1-mesa-dev libglu1-mesa-dev freeglut3-dev

echo ""
echo "Installazione FreeImage..."
apt-get install -y libfreeimage-dev

echo ""
echo "=========================================="
echo "Installazione completata!"
echo "=========================================="
echo ""
echo "Puoi ora compilare ed eseguire il progetto con:"
echo "  ./run.sh"
echo ""
