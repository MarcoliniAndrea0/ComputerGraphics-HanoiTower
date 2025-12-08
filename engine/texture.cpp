#include "texture.h"

#include <GL/freeglut.h>

#define FREEIMAGE_LIB
#include <FreeImage.h>

/**
 * @brief Crea una nuova istanza di `Texture` caricando un file immagine dal percorso specificato.
 *
 * Il costruttore carica un'immagine da un file utilizzando FreeImage e crea una texture OpenGL. Se il caricamento dell'immagine fallisce,
 * il puntatore `bitmap` viene impostato a `nullptr`.
 *
 * @param path Il percorso del file dell'immagine da caricare come texture.
 *
 * Se l'immagine viene caricata correttamente, viene convertita in un formato a 32 bit (RGBA) e configurata come texture OpenGL.
 * Se il caricamento fallisce, viene semplicemente impostato a `nullptr` e non viene creata alcuna texture.
 */
Texture::Texture(const std::string path)
    : Object{ "Texture" }, _textureId{ 0 }
{
    // Carica l'immagine dal percorso specificato utilizzando FreeImage.
    FIBITMAP* bmp = FreeImage_Load(FreeImage_GetFileType(path.c_str(), 0), path.c_str());

    if (bmp == nullptr)
    {
        ERROR("Impossible load the texture \"" + path + "\".");
        this->_bitmap = nullptr;
        return;
    }

    // Converte l'immagine caricata in un formato a 32 bit (RGBA).
    this->_bitmap = (void*)FreeImage_ConvertTo32Bits(bmp);

    // Scarica l'immagine originale non piu necessaria per liberare memoria.
    FreeImage_Unload(bmp);

    // numero di texture = 1 // Genera l'id
    glGenTextures(1, &this->_textureId);
    glBindTexture(GL_TEXTURE_2D, this->_textureId); // Texture attualmente attiva

    // Imposta la modalita di wrapping della texture a GL_REPEAT, 
    // che ripete la texture se le coordinate eccedono l'intervallo [0,1].
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Filtro di ingrandimento // se no aliasing se ingrandisci
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // Filtro di riduzione // se no jittering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    const int width = FreeImage_GetWidth((FIBITMAP*)this->_bitmap);
    const int height = FreeImage_GetHeight((FIBITMAP*)this->_bitmap);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, (void*)FreeImage_GetBits((FIBITMAP*)this->_bitmap));
}

/**
 * @brief Libera la memoria utilizzata da questa texture.
 *
 * Questo distruttore elimina la texture associata all'ID specificato in OpenGL e scarica
 * l'immagine caricata per liberare la memoria.
 */
Texture::~Texture()
{
    // Elimina la texture associata all'ID specificato per liberare le risorse in OpenGL.
    glDeleteTextures(1, &this->_textureId);

    if (this->_bitmap != nullptr)
        // Scarica l'immagine per liberare la memoria utilizzata da FreeImage.
        FreeImage_Unload((FIBITMAP*)this->_bitmap);
}

///// Render texture

/**
 * @brief Renderizza la texture.
 *
 * Questa funzione viene chiamata automaticamente da MyEngine non bisogna chiamarla manualmente.
 *
 * @param viewMatrix matrice da utilizzare per il rendering di questo oggetto.
 *
 * Associa la texture corrente al target di texture 2D in OpenGL e abilita l'uso delle texture 2D.
 * La texture identificata da `this->textureId` viene utilizzata per il mapping su oggetti 3D.
 */
void LIB_API Texture::render(const glm::mat4 viewMatrix) const
{
    glBindTexture(GL_TEXTURE_2D, this->_textureId);

    // Abilita l'uso delle texture 2D in OpenGL.
    glEnable(GL_TEXTURE_2D);
}

/**
 * @brief Verifica se la texture � stata caricata correttamente.
 *
 * @return `true` se la texture � stata caricata con successo, `false` altrimenti.
 */
bool Texture::isLoaded() const {
    return (_textureId != 0 && _bitmap != nullptr);
}
