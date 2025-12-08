#pragma once

#include <iostream>

/**
 * @brief Definisce la macro LIB_API per gestire l'esportazione/importazione di simboli in DLL.
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
  * @brief Abilita o disabilita i messaggi di debug.
  */
#ifndef _DEBUG
#define _DEBUG
#endif
#ifdef _DEBUG
#define DEBUG(message) \
    std::cout << "[DEBUG] " << message << std::endl;
#else
#define DEBUG(message)
#endif

  /**
   * @brief Macro per stampare messaggi di avviso.
   */
#define WARNING(message) \
    std::cout << "[WARNING] " << message << std::endl;

   /**
    * @brief Macro per stampare messaggi di errore.
    */
#define ERROR(message) \
    std::cerr << "[ERROR] " << message << std::endl;
