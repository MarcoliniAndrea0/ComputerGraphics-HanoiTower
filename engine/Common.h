/**
* @file Common.h
* @brief Definizioni di macro comuni, utility di debug e gestione dell'esportazione DLL.
*
* Questo file header contiene macro e definizioni utilizzate in tutto il progetto,
* incluse le macro per l'esportazione/importazione di simboli DLL su Windows,
* macro di logging per il debugging e la gestione degli errori.
*
* @author Gruppo6
* @date 2025
*/
#pragma once

#include <iostream>

/**
* @def LIB_API
* @brief Macro per la gestione dell'esportazione e importazione di simboli nelle librerie dinamiche (DLL).
*
* @details
* Questa macro gestisce automaticamente l'esportazione e importazione dei simboli
* quando si compila una libreria dinamica su Windows:
*
* - **Su Windows (_WINDOWS definito)**:
*   - Se SIMPLEDYNLIB_EXPORTS è definito (durante la compilazione della DLL):
*     espande a __declspec(dllexport) per esportare i simboli
*   - Altrimenti (durante l'uso della DLL): espande a __declspec(dllimport)
*     per importare i simboli
*
* - **Su altre piattaforme** (Linux, macOS): la macro è vuota, poiché questi
*   sistemi gestiscono i simboli delle librerie condivise in modo diverso.
*
* @note Questa macro deve essere usata prima della dichiarazione di ogni classe
*       o funzione che deve essere visibile dall'esterno della DLL.
*
* Esempio di utilizzo:
* @code
* class LIB_API MyClass {
*     // ...
* };
* @endcode
*/
#ifdef _WINDOWS
    #ifdef SIMPLEDYNLIB_EXPORTS
        #define LIB_API __declspec(dllexport)
    #else
        #define LIB_API __declspec(dllimport)
    #endif
#else
    #define LIB_API
#endif

/**
* @brief Assicura che _DEBUG sia definito se non lo è già.
*
* @note Questa sezione forza la modalità debug se non è stata definita
*       dal compilatore. Può essere commentata per le build di release.
*/
#ifndef _DEBUG
    #define _DEBUG
#endif

/**
* @def DEBUG(message)
* @brief Stampa un messaggio di debug su standard output con prefisso [DEBUG].
*
* Questa macro è attiva solo quando _DEBUG è definito. Permette di stampare
* messaggi di debug durante lo sviluppo senza impattare le performance
* delle build di release.
*
* La macro supporta qualsiasi tipo di dato che può essere inviato a std::cout
* tramite l'operatore <<.
*
* @param message Il messaggio o la variabile da stampare (supporta stream operator <<)
*
* Esempio di utilizzo:
* @code
* int x = 42;
* DEBUG("Il valore di x è: " << x);
* // Output: [DEBUG] Il valore di x è: 42
* @endcode
*
* @note In build di release (quando _DEBUG non è definito), questa macro
*       viene completamente rimossa dal codice dal preprocessore.
*/
#ifdef _DEBUG
    #define DEBUG(message) \
        std::cout << "[DEBUG] " << message << std::endl;
    #else
        #define DEBUG(message)
#endif

/**
    * @def WARNING(message)
    * @brief Stampa un messaggio di avviso (warning) su standard output con prefisso [WARNING].
    *
    * Questa macro viene utilizzata per segnalare situazioni anomale o potenzialmente
    * problematiche che non causano necessariamente un errore critico, ma richiedono
    * attenzione.
    *
    * A differenza di DEBUG, questa macro è sempre attiva, anche in build di release.
    *
    * @param message Il messaggio di avviso da stampare
    *
    * Esempio di utilizzo:
    * @code
    * WARNING("Texture non trovata, verrà usata quella di default");
    * // Output: [WARNING] Texture non trovata, verrà usata quella di default
    * @endcode
    */
#define WARNING(message) \
    std::cout << "[WARNING] " << message << std::endl;

/**
* @def ERROR(message)
* @brief Stampa un messaggio di errore su standard error (stderr) con prefisso [ERROR].
*
* Questa macro viene utilizzata per segnalare errori gravi che impediscono
* il normale funzionamento del programma. I messaggi vengono inviati a stderr
* per distinguerli dall'output normale su stdout.
*
* Questa macro è sempre attiva, anche in build di release.
*
* @param message Il messaggio di errore da stampare
*
* Esempio di utilizzo:
* @code
* ERROR("Impossibile caricare il file: " << filename);
* // Output su stderr: [ERROR] Impossibile caricare il file: model.obj
* @endcode
*
* @note Dopo aver stampato un messaggio di errore critico, potrebbe essere
*       necessario terminare il programma o gestire l'errore in modo appropriato.
*/
#define ERROR(message) \
    std::cerr << "[ERROR] " << message << std::endl;
