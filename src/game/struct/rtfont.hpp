#pragma once
#include "graphics/surface.hpp"
#include <map>
#include <string>
#include <vector>

// From Proton SDK.
#define C_RTFILE_PACKAGE_HEADER_BYTE_SIZE 6
struct RTFileHeader
{
    char fileTypeID[C_RTFILE_PACKAGE_HEADER_BYTE_SIZE];
    uint8_t version;
    uint8_t reserved[1];
};

#define C_RTFILE_FONT_HEADER "RTFONT"
struct rtfont_header
{
    RTFileHeader rtFileHeader;

    // our custom header
    short charSpacing;
    short lineHeight;
    short lineSpacing;
    short shadowXOffset;
    short shadowYOffset;
    short firstChar;
    short lastChar; // lastChar-firstChar is how many character definitions are coming
    short blankCharWidth;
    short fontStateCount; // how many FontState data thingies are coming
    short kerningPairCount;
    uint8_t reserved[124];
};

struct rtfont_charData
{
    short bmpPosX, bmpPosY;
    short charSizeX, charSizeY;
    short charBmpOffsetX; // used by Bitmap Font Generator only
    short charBmpOffsetY;
    float charBmpPosU, charBmpPosV;   // used by Well Oiled font maker only, ignored by us
    float charBmpPosU2, charBmpPosV2; // used by Well Oiled font maker only, ignored by us
    short xadvance;                   // used by Bitmap Font Generator only
};

struct KerningPair
{
    short first, second;
    signed char amount;
};

class FontChar
{
  public:
    rtfont_charData data;
};

class FontState
{
  public:
    FontState(){};
    FontState(char triggerChar, unsigned int color) : m_triggerChar(triggerChar), m_color(color){};
    unsigned int m_color;
    char m_triggerChar;
};

class RTFont
{
  public:
    virtual ~RTFont(){};

    rtfont_header m_header;
    std::vector<FontChar> m_chars;
    bool m_hasSpaceChar;
    Surface m_surf;
    std::vector<FontState> m_fontStates;
    float m_yOffset;
    std::map<unsigned int, signed char> m_kerningMap;
    std::string m_fileName;
};
