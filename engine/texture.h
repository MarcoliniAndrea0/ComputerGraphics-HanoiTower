#pragma once

/**
 * @file Texture.h
 * @brief Gestione delle texture (caricamento immagini e binding OpenGL).
 */
#include "Object.h"
#include <string>

class Texture : public Object
{
public:
	/**
	 * @brief Costruttore
	 * @param name Nome identificativo della texture (es. "TextureLegno").
	 * @param filename Percorso del file immagine su disco (es. "assets/wood.jpg").
	 */
	Texture(const std::string& name, const std::string& filename);

	/**
	 * @brief Distruttore. Rilascia la memoria della texture OpenGL.
	 */
	virtual ~Texture();

	/**
	 * @brief Carica l'immagine da disco e genera l'ID OpenGL.
	 * Viene chiamato automaticamente dal costruttore.
	 * @return true se il caricamento ha successo.
	 */
	bool load();

	/**
	 * @brief Attiva questa texture per il rendering corrente.
	 * Chiama glBindTexture e glEnable(GL_TEXTURE_2D).
	 */
	void render() override;

	/**
	 * @brief Verifica se la texture è valida.
	 */
	bool isValid() const;
	
	/**
	 * @brief Restituisce l'ID OpenGL della texture.
	 */
	unsigned int getTextureId() const;

private:
	std::string m_filename;     ///< Percorso del file su disco
	unsigned int m_textureId;   ///< ID generato da OpenGL (handle)
	void* m_bitmap;             ///< Puntatore ai dati raw (gestito internamente)
};