/* DO NOT EDIT THIS FILE */
/* This file is autogenerated by the text-database code generator */

#ifndef FONTCACHE_HPP
#define FONTCACHE_HPP

#include <fonts/ApplicationFontProvider.hpp>
#include <touchgfx/Font.hpp>
#include <touchgfx/TypedText.hpp>

namespace touchgfx
{
class CachedFont;

class FontDataReader
{
public:
    virtual ~FontDataReader() { }
    virtual void open() = 0;
    virtual void close() = 0;
    virtual void setPosition(uint32_t position) = 0;
    virtual void readData(void* out, uint32_t numberOfBytes) = 0;
};

class FontCache
{
public:
    FontCache();
    void setReader(FontDataReader* reader);
    void clear();
    void setMemory(uint8_t* memory, uint32_t size);
    void initializeCachedFont(TypedText t, CachedFont* font);
    bool cacheString(TypedText t, const Unicode::UnicodeChar* string);
    const GlyphNode* getGlyph(Unicode::UnicodeChar unicode, FontId font) const;
    uint32_t getMemoryUsage()
    {
        return top - memory;
    }

    void open();
    void close();

    static inline const uint8_t* getPixelData(const GlyphNode* glyph)
    {
        return ((const uint8_t*)glyph) + SizeGlyphNode + 4;
    }
    static inline bool isCached(const GlyphNode* g)
    {
        return g->dataOffset == 0xFFFFFFFF;
    }

private:
    static const uint32_t SizeGlyphNode = 16;

    bool contains(Unicode::UnicodeChar unicode, FontId font) const;
    void insert(Unicode::UnicodeChar unicode, FontId font, uint32_t bpp, bool& outOfMemory);
    uint8_t* copyGlyph(uint8_t* top, Unicode::UnicodeChar unicode, FontId font, uint32_t bpp, bool& outOfMemory);

    void cacheData(uint32_t bpp, GlyphNode* first);
    bool createSortedString(const Unicode::UnicodeChar* string);

    void setPosition(uint32_t position);
    void readData(void* out, uint32_t numberOfBytes);

    struct
    {
        uint8_t* first; //first glyphnode, glyph in cache;
        uint8_t* last; //first glyphnode, glyph in cache;
    } fontTable[MAX(TypographyFontIndex::NUMBER_OF_FONTS, 1)];

    uint32_t memorySize;
    uint8_t* memory; //start of memory
    uint8_t* top;    //first unused byte

    FontDataReader* reader;

    Unicode::UnicodeChar* sortedString;
    //Must be bigger than BinaryFontData
    static const uint32_t MAX_BUFFER_SIZE = 64;
    char buffer[MAX_BUFFER_SIZE];
    uint32_t glyphNodeOffset;
    uint32_t glyphDataOffset;
    uint32_t kerningTableOffset;
    uint32_t gsubOffset;
    uint32_t numGlyphs;
    GlyphNode currentFileGlyphNode;
    uint32_t currentFileGlyphNumber;
    bool readTooFar;
};
} // namespace touchgfx

#endif // FONTCACHE_HPP
