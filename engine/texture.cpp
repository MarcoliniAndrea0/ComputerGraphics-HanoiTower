/**
 * @file Texture.cpp
 * @brief Implementazione della classe Texture con FreeImage.
 */
#include "Texture.h"
#include "engine.h"      // Include gli header di OpenGL
#include <FreeImage.h>   
#include <iostream>

#pragma comment(lib, "FreeImage.lib")

Texture::Texture(const std::string& name, const std::string& filename)
    : Object(name, "Texture"), m_filename(filename), m_textureId(0), m_bitmap(nullptr)
{
    // Tentiamo il caricamento subito alla creazione
    load();
}

Texture::~Texture()
{
    // Pulizia OpenGL
    if (m_textureId != 0) {
        glDeleteTextures(1, &m_textureId);
    }
    // FreeImage gestisce la sua memoria internamente quando facciamo Unload
}

bool Texture::load()
{
    // Capire il formato del file (JPG, PNG, ecc.)
    FREE_IMAGE_FORMAT format = FreeImage_GetFileType(m_filename.c_str(), 0);
    if (format == FIF_UNKNOWN) {
        format = FreeImage_GetFIFFromFilename(m_filename.c_str());
    }
    if (format == FIF_UNKNOWN) {
        std::cerr << "[ERROR] Texture format not recognized: " << m_filename << std::endl;
        return false;
    }

    // Caricare l'immagine in memoria RAM
    FIBITMAP* bitmap = FreeImage_Load(format, m_filename.c_str());
    if (!bitmap) {
        std::cerr << "[ERROR] Failed to load texture file: " << m_filename << std::endl;
        return false;
    }

    // Convertire in 32 bit (RGBA) per compatibilità massima
    FIBITMAP* bitmap32 = FreeImage_ConvertTo32Bits(bitmap);
    FreeImage_Unload(bitmap); // L'originale non serve più

    int width = FreeImage_GetWidth(bitmap32);
    int height = FreeImage_GetHeight(bitmap32);
    void* data = FreeImage_GetBits(bitmap32);

    // Generare la texture in OpenGL (GPU)
    glGenTextures(1, &m_textureId);
    glBindTexture(GL_TEXTURE_2D, m_textureId);

    // Parametri di ripetizione
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Filtri di qualità
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    // Anisotropia (Opzionale: migliora le texture viste di sbieco)
    // Nota: richiede GLEW o gestione estensioni manuale, per ora commentiamo per sicurezza
    /*
    if (strstr((const char*)glGetString(GL_EXTENSIONS), "GL_EXT_texture_filter_anisotropic")) {
        GLfloat maxAnisotropy;
        glGetFloatv(0x84FF, &maxAnisotropy); // GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT
        glTexParameterf(GL_TEXTURE_2D, 0x84FE, maxAnisotropy); // GL_TEXTURE_MAX_ANISOTROPY_EXT
    }
    */

    // Caricare i pixel nella scheda video
    // Nota: FreeImage usa l'ordine BGRA, OpenGL su Windows lo supporta nativamente con GL_BGRA_EXT
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, data);

    // Genera le versioni ridotte per la distanza
    //glGenerateMipmap(GL_TEXTURE_2D);

    // Pulizia memoria RAM (i dati ora sono nella GPU)
    FreeImage_Unload(bitmap32);

    std::cout << "[INFO] Texture loaded successfully: " << m_filename << " (ID: " << m_textureId << ")" << std::endl;
    return true;
}

void Texture::render()
{
    if (m_textureId != 0) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, m_textureId);

        // Importante: Reset del colore a bianco per non tingere la texture
        // (Nota: questo potrebbe sovrascrivere il materiale se non gestito bene, 
        //  ma per ora assicura che la texture si veda con i suoi colori originali)
        // glColor3f(1.0f, 1.0f, 1.0f); 
    }
}

bool Texture::isValid() const {
    return m_textureId != 0;
}

unsigned int Texture::getTextureId() const {
    return m_textureId;
}