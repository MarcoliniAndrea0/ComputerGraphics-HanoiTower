#pragma once

// Migliora la qualita delle texture quando sono viste da angoli inclinati.
// Piu alto e il valore, piu dettagliata sara la texture.
#define GL_TEXTURE_MAX_ANISOTROPY_EXT        0x84FE

// Quanto puoi aumentare la qualita delle texture sulla tua scheda grafica.
#define GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT    0x84FF

#include "Common.h"
#include "Object.h"

/**
 * @file Texture.h
 * @brief Dichiarazione della classe Texture per la gestione delle texture in OpenGL.
 *
 * La classe `Texture` permette di caricare immagini da file e di applicarle come texture
 * su modelli 3D. È progettata per integrarsi con OpenGL e utilizza la libreria FreeImage
 * per il caricamento delle immagini.
 */

 /**
  * @class Texture
  * @brief Rappresenta una texture che consente di applicare immagini su un modello 3D.
  *
  * La classe `Texture` rappresenta una texture che viene applicata sui modelli 3D,
  * permettendo di visualizzare immagini sulle superfici delle mesh. Gestisce il caricamento
  * di un'immagine da un percorso specificato e la renderizzazione della texture sugli oggetti 3D.
  */
class LIB_API Texture : public Object
{
public:
    /**
     * @brief Costruttore della classe Texture.
     *
     * Questo costruttore carica un'immagine da un percorso specificato utilizzando la libreria FreeImage
     * e configura la texture in OpenGL. Se il caricamento fallisce, la texture non è valida.
     *
     * @param path Il percorso dell'immagine da caricare come texture.
     */
    Texture(const std::string path);

    /**
     * @brief Distruttore della classe Texture.
     *
     * Libera tutte le risorse allocate, incluse le texture OpenGL e i dati dell'immagine caricata.
     */
    ~Texture();

    /**
     * @brief Verifica se la texture è stata caricata correttamente.
     *
     * @return `true` se la texture è stata caricata con successo, `false` altrimenti.
     */
    bool isLoaded() const;

    /**
     * @brief Renderizza la texture.
     *
     * Associa la texture corrente al target di texture 2D in OpenGL
     * e abilita l'uso delle texture 2D per il rendering.
     *
     * @param viewMatrix La matrice di vista da utilizzare per il rendering.
     */
    void render(const glm::mat4 viewMatrix) const override;

private:
    /**
     * @brief Puntatore ai dati dell'immagine in memoria.
     *
     * Questo puntatore viene utilizzato per conservare i dati dell'immagine caricata
     * tramite la libreria FreeImage. È convertito in un formato compatibile con OpenGL.
     */
    void* _bitmap;

    /**
     * @brief Identificatore della texture OpenGL.
     *
     * Questo ID è assegnato da OpenGL per rappresentare in modo univoco la texture.
     */
    unsigned int _textureId;
};
