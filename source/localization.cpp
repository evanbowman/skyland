#include "localization.hpp"
#include "platform/platform.hpp"
#include "script/lisp.hpp"



StringBuffer<32> format_time(u32 seconds, bool include_hours)
{
    StringBuffer<32> result;
    char buffer[32];

    int hours = seconds / 3600;
    int remainder = (int)seconds - hours * 3600;
    int mins = remainder / 60;
    remainder = remainder - mins * 60;
    int secs = remainder;

    if (include_hours) {
        locale_num2str(hours, buffer, 10);
        result += buffer;
        result += ":";
    }

    locale_num2str(mins, buffer, 10);
    result += buffer;
    result += ":";

    if (secs < 10) {
        result += "0";
    }

    locale_num2str(secs, buffer, 10);
    result += buffer;

    return result;
}



class str_const {
private:
    const char* const p_;
    const size_t sz_;

public:
    template <size_t N>
    constexpr str_const(const char (&a)[N]) : p_(a), sz_(N - 1)
    {
    }

    constexpr char operator[](std::size_t n)
    {
        return n < sz_ ? p_[n] : '0';
    }
};


#ifndef __BYTE_ORDER__
#error "byte order must be defined"
#endif


// FIXME: assumes little endian? Does it matter though, which way we order
// stuff, as long as it's consistent? Actually it does matter, considering
// that we're byte-swapping stuff in unicode.hpp
#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
#error "TODO: fix the utf-8 decoding (below) for big endian"
#endif


// Needs to be a macro because there's no way to pass a str_const as a
// constexpr parameter. Converts the first utf-8 codepoint in a string to a
// 32-bit integer, for use in a giant switch statement (below).
#define UTF8_GETCHR(_STR_)                                                     \
    []() -> utf8::Codepoint {                                                  \
        if constexpr ((str_const(_STR_)[0] & 0x80) == 0) {                     \
            return str_const(_STR_)[0];                                        \
        } else if constexpr ((str_const(_STR_)[0] & 0xf0) == 0xC0 ||           \
                             (str_const(_STR_)[0] & 0xf0) == 0xD0) {           \
            return (u32)(u8)str_const(_STR_)[0] |                              \
                   (((u32)(u8)str_const(_STR_)[1]) << 8);                      \
        } else if constexpr ((str_const(_STR_)[0] & 0xf0) == 0xE0) {           \
            return (u32)(u8)str_const(_STR_)[0] |                              \
                   (((u32)(u8)str_const(_STR_)[1]) << 8) |                     \
                   ((u32)(u8)str_const(_STR_)[2] << 16);                       \
        } else if constexpr ((str_const(_STR_)[0] & 0xf0) == 0xF0) {           \
            return (u32)(u8)str_const(_STR_)[0] |                              \
                   ((u32)(u8)str_const(_STR_)[1] << 8) |                       \
                   ((u32)(u8)str_const(_STR_)[2] << 16) |                      \
                   ((u32)(u8)str_const(_STR_)[3] << 24);                       \
        } else {                                                               \
            return 0;                                                          \
        }                                                                      \
    }()


template <u32 B, bool C> constexpr void my_assert()
{
    static_assert(C, "oh no");
}

#define UTF8_TESTCHR(_STR_)                                                    \
    []() -> utf8::Codepoint {                                                  \
        my_assert<(u32)(u8)str_const(_STR_)[0], false>();                      \
        return 0;                                                              \
    }()



static const char* font_image = "charset";



void set_font_image(const char* font_image_name)
{
    font_image = font_image_name;
}



std::optional<Platform::TextureMapping>
standard_texture_map(const utf8::Codepoint& cp)
{
    auto mapping = [&]() -> std::optional<u16> {
        switch (cp) {

            // clang-format off

        case UTF8_GETCHR(u8"0"): return 1;
        case UTF8_GETCHR(u8"1"): return 2;
        case UTF8_GETCHR(u8"2"): return 3;
        case UTF8_GETCHR(u8"3"): return 4;
        case UTF8_GETCHR(u8"4"): return 5;
        case UTF8_GETCHR(u8"5"): return 6;
        case UTF8_GETCHR(u8"6"): return 7;
        case UTF8_GETCHR(u8"7"): return 8;
        case UTF8_GETCHR(u8"8"): return 9;
        case UTF8_GETCHR(u8"9"): return 10;
        case UTF8_GETCHR(u8"a"): return 11;
        case UTF8_GETCHR(u8"b"): return 12;
        case UTF8_GETCHR(u8"c"): return 13;
        case UTF8_GETCHR(u8"d"): return 14;
        case UTF8_GETCHR(u8"e"): return 15;
        case UTF8_GETCHR(u8"f"): return 16;
        case UTF8_GETCHR(u8"g"): return 17;
        case UTF8_GETCHR(u8"h"): return 18;
        case UTF8_GETCHR(u8"i"): return 19;
        case UTF8_GETCHR(u8"j"): return 20;
        case UTF8_GETCHR(u8"k"): return 21;
        case UTF8_GETCHR(u8"l"): return 22;
        case UTF8_GETCHR(u8"m"): return 23;
        case UTF8_GETCHR(u8"n"): return 24;
        case UTF8_GETCHR(u8"o"): return 25;
        case UTF8_GETCHR(u8"p"): return 26;
        case UTF8_GETCHR(u8"q"): return 27;
        case UTF8_GETCHR(u8"r"): return 28;
        case UTF8_GETCHR(u8"s"): return 29;
        case UTF8_GETCHR(u8"t"): return 30;
        case UTF8_GETCHR(u8"u"): return 31;
        case UTF8_GETCHR(u8"v"): return 32;
        case UTF8_GETCHR(u8"w"): return 33;
        case UTF8_GETCHR(u8"x"): return 34;
        case UTF8_GETCHR(u8"y"): return 35;
        case UTF8_GETCHR(u8"z"): return 36;
        case UTF8_GETCHR(u8"."): return 37;
        case UTF8_GETCHR(u8","): return 38;
        case UTF8_GETCHR(u8"A"): return 39;
        case UTF8_GETCHR(u8"B"): return 40;
        case UTF8_GETCHR(u8"C"): return 41;
        case UTF8_GETCHR(u8"D"): return 42;
        case UTF8_GETCHR(u8"E"): return 43;
        case UTF8_GETCHR(u8"F"): return 44;
        case UTF8_GETCHR(u8"G"): return 45;
        case UTF8_GETCHR(u8"H"): return 46;
        case UTF8_GETCHR(u8"I"): return 47;
        case UTF8_GETCHR(u8"J"): return 48;
        case UTF8_GETCHR(u8"K"): return 49;
        case UTF8_GETCHR(u8"L"): return 50;
        case UTF8_GETCHR(u8"M"): return 51;
        case UTF8_GETCHR(u8"N"): return 52;
        case UTF8_GETCHR(u8"O"): return 53;
        case UTF8_GETCHR(u8"P"): return 54;
        case UTF8_GETCHR(u8"Q"): return 55;
        case UTF8_GETCHR(u8"R"): return 56;
        case UTF8_GETCHR(u8"S"): return 57;
        case UTF8_GETCHR(u8"T"): return 58;
        case UTF8_GETCHR(u8"U"): return 59;
        case UTF8_GETCHR(u8"V"): return 60;
        case UTF8_GETCHR(u8"W"): return 61;
        case UTF8_GETCHR(u8"X"): return 62;
        case UTF8_GETCHR(u8"Y"): return 63;
        case UTF8_GETCHR(u8"Z"): return 64;
        case UTF8_GETCHR(u8"\""): return 65;
        case UTF8_GETCHR(u8"'"): return 66;
        case UTF8_GETCHR(u8"["): return 67;
        case UTF8_GETCHR(u8"]"): return 68;
        case UTF8_GETCHR(u8"("): return 69;
        case UTF8_GETCHR(u8")"): return 70;
        case UTF8_GETCHR(u8":"): return 71;
        case UTF8_GETCHR(u8" "): return 72;
        case UTF8_GETCHR(u8"%"): return 93;
        case UTF8_GETCHR(u8"!"): return 94;
        case UTF8_GETCHR(u8"?"): return 95;
        case UTF8_GETCHR(u8"+"): return 98;
        case UTF8_GETCHR(u8"-"): return 99;
        case UTF8_GETCHR(u8"/"): return 100;
        case UTF8_GETCHR(u8"*"): return 101;
        case UTF8_GETCHR(u8"="): return 102;
        case UTF8_GETCHR(u8"<"): return 103;
        case UTF8_GETCHR(u8">"): return 104;
        case UTF8_GETCHR(u8"#"): return 105;
        case UTF8_GETCHR(u8"_"): return 186;
        case UTF8_GETCHR(u8"$"): return 2151;
        case UTF8_GETCHR(u8";"): return 2152;
        case UTF8_GETCHR(u8"\n"): return 2153;
        case UTF8_GETCHR(u8"`"): return 2154;
        case UTF8_GETCHR(u8"@"): return 2155;

        // Cyrillic Characters
        case UTF8_GETCHR(u8"??"): return 2085;
        case UTF8_GETCHR(u8"??"): return 2086;
        case UTF8_GETCHR(u8"??"): return 2087;
        case UTF8_GETCHR(u8"??"): return 2088;
        case UTF8_GETCHR(u8"??"): return 2089;
        case UTF8_GETCHR(u8"??"): return 2090;
        case UTF8_GETCHR(u8"??"): return 2091;
        case UTF8_GETCHR(u8"??"): return 2092;
        case UTF8_GETCHR(u8"??"): return 2093;
        case UTF8_GETCHR(u8"??"): return 2094;
        case UTF8_GETCHR(u8"??"): return 2095;
        case UTF8_GETCHR(u8"??"): return 2096;
        case UTF8_GETCHR(u8"??"): return 2097;
        case UTF8_GETCHR(u8"??"): return 2098;
        case UTF8_GETCHR(u8"??"): return 2099;
        case UTF8_GETCHR(u8"??"): return 2100;
        case UTF8_GETCHR(u8"??"): return 2101;
        case UTF8_GETCHR(u8"??"): return 2102;
        case UTF8_GETCHR(u8"??"): return 2103;
        case UTF8_GETCHR(u8"??"): return 2104;
        case UTF8_GETCHR(u8"??"): return 2105;
        case UTF8_GETCHR(u8"??"): return 2106;
        case UTF8_GETCHR(u8"??"): return 2107;
        case UTF8_GETCHR(u8"??"): return 2108;
        case UTF8_GETCHR(u8"??"): return 2109;
        case UTF8_GETCHR(u8"??"): return 2110;
        case UTF8_GETCHR(u8"??"): return 2111;
        case UTF8_GETCHR(u8"??"): return 2112;
        case UTF8_GETCHR(u8"??"): return 2113;
        case UTF8_GETCHR(u8"??"): return 2114;
        case UTF8_GETCHR(u8"??"): return 2115;
        case UTF8_GETCHR(u8"??"): return 2116;
        case UTF8_GETCHR(u8"??"): return 2117;
        case UTF8_GETCHR(u8"??"): return 2118;
        case UTF8_GETCHR(u8"??"): return 2119;
        case UTF8_GETCHR(u8"??"): return 2120;
        case UTF8_GETCHR(u8"??"): return 2121;
        case UTF8_GETCHR(u8"??"): return 2122;
        case UTF8_GETCHR(u8"??"): return 2123;
        case UTF8_GETCHR(u8"??"): return 2124;
        case UTF8_GETCHR(u8"??"): return 2125;
        case UTF8_GETCHR(u8"??"): return 2126;
        case UTF8_GETCHR(u8"??"): return 2127;
        case UTF8_GETCHR(u8"??"): return 2128;
        case UTF8_GETCHR(u8"??"): return 2129;
        case UTF8_GETCHR(u8"??"): return 2130;
        case UTF8_GETCHR(u8"??"): return 2131;
        case UTF8_GETCHR(u8"??"): return 2132;
        case UTF8_GETCHR(u8"??"): return 2133;
        case UTF8_GETCHR(u8"??"): return 2134;
        case UTF8_GETCHR(u8"??"): return 2135;
        case UTF8_GETCHR(u8"??"): return 2136;
        case UTF8_GETCHR(u8"??"): return 2137;
        case UTF8_GETCHR(u8"??"): return 2138;
        case UTF8_GETCHR(u8"??"): return 2139;
        case UTF8_GETCHR(u8"??"): return 2140;
        case UTF8_GETCHR(u8"??"): return 2141;
        case UTF8_GETCHR(u8"??"): return 2142;
        case UTF8_GETCHR(u8"??"): return 2143;
        case UTF8_GETCHR(u8"??"): return 2144;
        case UTF8_GETCHR(u8"??"): return 2145;
        case UTF8_GETCHR(u8"??"): return 2146;
        case UTF8_GETCHR(u8"??"): return 2147;
        case UTF8_GETCHR(u8"??"): return 2148;
        case UTF8_GETCHR(u8"??"): return 87;


        // A small number of tiny Chinese glyphs. We don't use too many, because
        // they're difficult to read at this size.
        case UTF8_GETCHR(u8"???"): return 1807;
        case UTF8_GETCHR(u8"???"): return 1808;
        case UTF8_GETCHR(u8"???"): return 1809;
        case UTF8_GETCHR(u8"???"): return 1810;
        case UTF8_GETCHR(u8"???"): return 1811;
        case UTF8_GETCHR(u8"???"): return 1812;
        case UTF8_GETCHR(u8"???"): return 1813;
        case UTF8_GETCHR(u8"???"): return 1814;
        case UTF8_GETCHR(u8"???"): return 1815;
        case UTF8_GETCHR(u8"???"): return 1816;
        case UTF8_GETCHR(u8"???"): return 1817;
        case UTF8_GETCHR(u8"???"): return 1818;
        case UTF8_GETCHR(u8"???"): return 1819;
        case UTF8_GETCHR(u8"???"): return 1828;
        case UTF8_GETCHR(u8"???"): return 1829;
        case UTF8_GETCHR(u8"???"): return 1830;
        case UTF8_GETCHR(u8"???"): return 1831;
        case UTF8_GETCHR(u8"???"): return 1836;
        case UTF8_GETCHR(u8"???"): return 1837;
        case UTF8_GETCHR(u8"???"): return 2070;

            // clang-format on

        default:
            return std::nullopt;
        }
    }();
    if (mapping) {
        return Platform::TextureMapping{font_image, *mapping};
    } else {
        return {};
    }
}


std::optional<Platform::TextureMapping>
doublesize_texture_map(const utf8::Codepoint& cp)
{
    auto mapping = [&]() -> std::optional<u16> {
        switch (cp) {
            // clang-format off
        case UTF8_GETCHR(u8"0"): return 1763;
        case UTF8_GETCHR(u8"1"): return 1767;
        case UTF8_GETCHR(u8"2"): return 1771;
        case UTF8_GETCHR(u8"3"): return 1775;
        case UTF8_GETCHR(u8"4"): return 1779;
        case UTF8_GETCHR(u8"5"): return 1783;
        case UTF8_GETCHR(u8"6"): return 1787;
        case UTF8_GETCHR(u8"7"): return 1791;
        case UTF8_GETCHR(u8"8"): return 1795;
        case UTF8_GETCHR(u8"9"): return 1799;

        // FIXME: This block includes regular-sized glyphs.
        case UTF8_GETCHR(u8"."): return 37;
        case UTF8_GETCHR(u8","): return 38;
        case UTF8_GETCHR(u8"\""): return 65;
        case UTF8_GETCHR(u8"'"): return 66;
        case UTF8_GETCHR(u8"["): return 1755;
        case UTF8_GETCHR(u8"]"): return 1759;
        case UTF8_GETCHR(u8"("): return 69;
        case UTF8_GETCHR(u8")"): return 70;
        case UTF8_GETCHR(u8":"): return 743;
        case UTF8_GETCHR(u8" "): return 72;
        case UTF8_GETCHR(u8"!"): return 835;
        case UTF8_GETCHR(u8"?"): return 1259;
        case UTF8_GETCHR(u8"+"): return 1902;
        case UTF8_GETCHR(u8"-"): return 1906;
        case UTF8_GETCHR(u8"/"): return 100;
        case UTF8_GETCHR(u8"*"): return 101;
        case UTF8_GETCHR(u8"="): return 102;
        case UTF8_GETCHR(u8"<"): return 103;
        case UTF8_GETCHR(u8">"): return 104;
        case UTF8_GETCHR(u8"#"): return 105;
        case UTF8_GETCHR(u8"_"): return 186;


        case UTF8_GETCHR(u8"A"): return 1910;
        case UTF8_GETCHR(u8"B"): return 1914;


        case UTF8_GETCHR(u8"%"): return 1832;

        // Chinese Glyphs:
        case UTF8_GETCHR(u8"???"): return 187;
        case UTF8_GETCHR(u8"???"): return 191;
        case UTF8_GETCHR(u8"???"): return 195;
        case UTF8_GETCHR(u8"???"): return 199;
        case UTF8_GETCHR(u8"???"): return 203;
        case UTF8_GETCHR(u8"???"): return 207;
        case UTF8_GETCHR(u8"???"): return 211;
        case UTF8_GETCHR(u8"???"): return 215;
        case UTF8_GETCHR(u8"???"): return 219;
        case UTF8_GETCHR(u8"???"): return 223;
        case UTF8_GETCHR(u8"???"): return 227;
        case UTF8_GETCHR(u8"???"): return 231;
        case UTF8_GETCHR(u8"???"): return 235;
        case UTF8_GETCHR(u8"???"): return 239;
        case UTF8_GETCHR(u8"???"): return 243;
        case UTF8_GETCHR(u8"???"): return 247;
        case UTF8_GETCHR(u8"???"): return 251;
        case UTF8_GETCHR(u8"???"): return 255;
        case UTF8_GETCHR(u8"???"): return 259;
        case UTF8_GETCHR(u8"???"): return 263;
        case UTF8_GETCHR(u8"???"): return 267;
        case UTF8_GETCHR(u8"???"): return 271;
        case UTF8_GETCHR(u8"???"): return 275;
        case UTF8_GETCHR(u8"???"): return 279;
        case UTF8_GETCHR(u8"???"): return 283;
        case UTF8_GETCHR(u8"???"): return 287;
        case UTF8_GETCHR(u8"???"): return 291;
        case UTF8_GETCHR(u8"???"): return 295;
        case UTF8_GETCHR(u8"???"): return 299;
        case UTF8_GETCHR(u8"???"): return 303;
        case UTF8_GETCHR(u8"???"): return 307;
        case UTF8_GETCHR(u8"???"): return 311;
        case UTF8_GETCHR(u8"???"): return 315;
        case UTF8_GETCHR(u8"???"): return 319;
        case UTF8_GETCHR(u8"???"): return 323;
        case UTF8_GETCHR(u8"???"): return 327;
        case UTF8_GETCHR(u8"???"): return 331;
        case UTF8_GETCHR(u8"???"): return 335;
        case UTF8_GETCHR(u8"???"): return 339;
        case UTF8_GETCHR(u8"???"): return 343;
        case UTF8_GETCHR(u8"???"): return 347;
        case UTF8_GETCHR(u8"???"): return 351;
        case UTF8_GETCHR(u8"???"): return 355;
        case UTF8_GETCHR(u8"???"): return 359;
        case UTF8_GETCHR(u8"???"): return 363;
        case UTF8_GETCHR(u8"???"): return 367;
        case UTF8_GETCHR(u8"???"): return 371;
        case UTF8_GETCHR(u8"???"): return 375;
        case UTF8_GETCHR(u8"???"): return 379;
        case UTF8_GETCHR(u8"???"): return 383;
        case UTF8_GETCHR(u8"???"): return 387;
        case UTF8_GETCHR(u8"???"): return 391;
        case UTF8_GETCHR(u8"???"): return 395;
        case UTF8_GETCHR(u8"???"): return 399;
        case UTF8_GETCHR(u8"???"): return 403;
        case UTF8_GETCHR(u8"???"): return 407;
        case UTF8_GETCHR(u8"???"): return 411;
        case UTF8_GETCHR(u8"???"): return 415;
        case UTF8_GETCHR(u8"???"): return 419;
        case UTF8_GETCHR(u8"???"): return 423;
        case UTF8_GETCHR(u8"???"): return 427;
        case UTF8_GETCHR(u8"???"): return 431;
        case UTF8_GETCHR(u8"???"): return 435;
        case UTF8_GETCHR(u8"???"): return 439;
        case UTF8_GETCHR(u8"???"): return 443;
        case UTF8_GETCHR(u8"???"): return 447;
        case UTF8_GETCHR(u8"???"): return 451;
        case UTF8_GETCHR(u8"???"): return 455;
        case UTF8_GETCHR(u8"???"): return 459;
        case UTF8_GETCHR(u8"???"): return 463;
        case UTF8_GETCHR(u8"???"): return 467;
        case UTF8_GETCHR(u8"???"): return 471;
        case UTF8_GETCHR(u8"???"): return 475;
        case UTF8_GETCHR(u8"???"): return 479;
        case UTF8_GETCHR(u8"???"): return 483;
        case UTF8_GETCHR(u8"???"): return 487;
        case UTF8_GETCHR(u8"???"): return 491;
        case UTF8_GETCHR(u8"???"): return 495;
        case UTF8_GETCHR(u8"???"): return 499;
        case UTF8_GETCHR(u8"???"): return 503;
        case UTF8_GETCHR(u8"???"): return 507;
        case UTF8_GETCHR(u8"???"): return 511;
        case UTF8_GETCHR(u8"???"): return 515;
        case UTF8_GETCHR(u8"???"): return 519;
        // Intentional gap.
        case UTF8_GETCHR(u8"???"): return 527;
        case UTF8_GETCHR(u8"???"): return 531;
        case UTF8_GETCHR(u8"???"): return 535;
        case UTF8_GETCHR(u8"???"): return 539;
        case UTF8_GETCHR(u8"???"): return 543;
        case UTF8_GETCHR(u8"???"): return 547;
        case UTF8_GETCHR(u8"???"): return 551;
        case UTF8_GETCHR(u8"???"): return 555;
        case UTF8_GETCHR(u8"???"): return 559;
        case UTF8_GETCHR(u8"???"): return 563;
        case UTF8_GETCHR(u8"???"): return 567;
        case UTF8_GETCHR(u8"???"): return 571;
        case UTF8_GETCHR(u8"???"): return 575;
        case UTF8_GETCHR(u8"???"): return 579;
        case UTF8_GETCHR(u8"???"): return 583;
        case UTF8_GETCHR(u8"???"): return 587;
        case UTF8_GETCHR(u8"???"): return 591;
        case UTF8_GETCHR(u8"???"): return 595;
        case UTF8_GETCHR(u8"???"): return 599;
        case UTF8_GETCHR(u8"???"): return 603;
        case UTF8_GETCHR(u8"???"): return 607;
        case UTF8_GETCHR(u8"???"): return 611;
        case UTF8_GETCHR(u8"???"): return 615;
        case UTF8_GETCHR(u8"???"): return 619;
        case UTF8_GETCHR(u8"???"): return 623;
        case UTF8_GETCHR(u8"???"): return 627;
        case UTF8_GETCHR(u8"???"): return 631;
        case UTF8_GETCHR(u8"???"): return 635;
        case UTF8_GETCHR(u8"???"): return 639;
        case UTF8_GETCHR(u8"???"): return 643;
        case UTF8_GETCHR(u8"???"): return 647;
        case UTF8_GETCHR(u8"???"): return 651;
        case UTF8_GETCHR(u8"???"): return 655;
        case UTF8_GETCHR(u8"???"): return 659;
        case UTF8_GETCHR(u8"???"): return 663;
        case UTF8_GETCHR(u8"???"): return 667;
        case UTF8_GETCHR(u8"???"): return 671;
        case UTF8_GETCHR(u8"???"): return 675;
        case UTF8_GETCHR(u8"???"): return 679;
        case UTF8_GETCHR(u8"???"): return 683;
        case UTF8_GETCHR(u8"???"): return 687;
        case UTF8_GETCHR(u8"???"): return 691;
        case UTF8_GETCHR(u8"???"): return 695;
        case UTF8_GETCHR(u8"???"): return 699;
        // Some misc number glyphs...
        case UTF8_GETCHR(u8"???"): return 703;
        case UTF8_GETCHR(u8"???"): return 707;
        case UTF8_GETCHR(u8"???"): return 711;
        case UTF8_GETCHR(u8"???"): return 715;
        case UTF8_GETCHR(u8"???"): return 719;
        case UTF8_GETCHR(u8"???"): return 723;
        // For settings text:
        case UTF8_GETCHR(u8"???"): return 727;
        case UTF8_GETCHR(u8"???"): return 731;
        case UTF8_GETCHR(u8"???"): return 735;
        case UTF8_GETCHR(u8"???"): return 739;
        // Intentional break
        case UTF8_GETCHR(u8"???"): return 747;
        case UTF8_GETCHR(u8"???"): return 751;
        case UTF8_GETCHR(u8"???"): return 755;
        case UTF8_GETCHR(u8"???"): return 759;
        case UTF8_GETCHR(u8"???"): return 763;
        case UTF8_GETCHR(u8"???"): return 767;
        case UTF8_GETCHR(u8"???"): return 771;
        // Boss Death text:
        case UTF8_GETCHR(u8"???"): return 775;
        case UTF8_GETCHR(u8"???"): return 779;
        case UTF8_GETCHR(u8"???"): return 783;
        case UTF8_GETCHR(u8"???"): return 787;
        case UTF8_GETCHR(u8"???"): return 791;
        case UTF8_GETCHR(u8"???"): return 795;
        case UTF8_GETCHR(u8"???"): return 799;
        case UTF8_GETCHR(u8"???"): return 803;
        case UTF8_GETCHR(u8"???"): return 807;
        // At line 128
        case UTF8_GETCHR(u8"???"): return 811;
        case UTF8_GETCHR(u8"???"): return 815;
        case UTF8_GETCHR(u8"???"): return 819;
        // ... 128 ...
        // At line 138
        case UTF8_GETCHR(u8"???"): return 823;
        case UTF8_GETCHR(u8"???"): return 827;
        case UTF8_GETCHR(u8"???"): return 831;
        // ... 138 ...
        // Intentional break
        case UTF8_GETCHR(u8"???"): return 839;
        case UTF8_GETCHR(u8"???"): return 843;
        case UTF8_GETCHR(u8"???"): return 847;
        case UTF8_GETCHR(u8"???"): return 851;
        case UTF8_GETCHR(u8"???"): return 855;
        case UTF8_GETCHR(u8"???"): return 859;
        case UTF8_GETCHR(u8"???"): return 863;
        case UTF8_GETCHR(u8"???"): return 867;
        case UTF8_GETCHR(u8"???"): return 871;
        case UTF8_GETCHR(u8"???"): return 875;
        case UTF8_GETCHR(u8"???"): return 879;
        case UTF8_GETCHR(u8"???"): return 883;
        case UTF8_GETCHR(u8"???"): return 887;
        case UTF8_GETCHR(u8"???"): return 891;
        case UTF8_GETCHR(u8"???"): return 895;
        case UTF8_GETCHR(u8"???"): return 899;
        case UTF8_GETCHR(u8"???"): return 903;
        case UTF8_GETCHR(u8"???"): return 907;
        case UTF8_GETCHR(u8"???"): return 911;
        case UTF8_GETCHR(u8"???"): return 915;
        case UTF8_GETCHR(u8"???"): return 919;
        case UTF8_GETCHR(u8"???"): return 923;
        case UTF8_GETCHR(u8"???"): return 927;
        case UTF8_GETCHR(u8"???"): return 931;
        case UTF8_GETCHR(u8"???"): return 935;
        case UTF8_GETCHR(u8"???"): return 939;
        case UTF8_GETCHR(u8"???"): return 943;
        case UTF8_GETCHR(u8"???"): return 947;
        case UTF8_GETCHR(u8"???"): return 951;
        case UTF8_GETCHR(u8"???"): return 955;
        case UTF8_GETCHR(u8"???"): return 959;
        case UTF8_GETCHR(u8"???"): return 963;
        case UTF8_GETCHR(u8"???"): return 967;
        case UTF8_GETCHR(u8"???"): return 971;
        case UTF8_GETCHR(u8"???"): return 975;
        case UTF8_GETCHR(u8"???"): return 979;
        case UTF8_GETCHR(u8"???"): return 983;
        case UTF8_GETCHR(u8"???"): return 987;
        case UTF8_GETCHR(u8"???"): return 991;
        case UTF8_GETCHR(u8"???"): return 995;
        case UTF8_GETCHR(u8"???"): return 999;
        case UTF8_GETCHR(u8"???"): return 1003;
        case UTF8_GETCHR(u8"???"): return 1007;
        case UTF8_GETCHR(u8"???"): return 1011;
        case UTF8_GETCHR(u8"???"): return 1015;
        case UTF8_GETCHR(u8"???"): return 1019;
        case UTF8_GETCHR(u8"???"): return 1023;
        case UTF8_GETCHR(u8"???"): return 1027;
        case UTF8_GETCHR(u8"???"): return 1031;
        case UTF8_GETCHR(u8"???"): return 1035;
        case UTF8_GETCHR(u8"???"): return 1039;
        case UTF8_GETCHR(u8"???"): return 1043;
        case UTF8_GETCHR(u8"???"): return 1047;
        case UTF8_GETCHR(u8"???"): return 1051;
        case UTF8_GETCHR(u8"???"): return 1055;
        case UTF8_GETCHR(u8"???"): return 1059;
        case UTF8_GETCHR(u8"???"): return 1063;
        case UTF8_GETCHR(u8"???"): return 1067;
        case UTF8_GETCHR(u8"???"): return 1071;
        case UTF8_GETCHR(u8"???"): return 1075;
        case UTF8_GETCHR(u8"???"): return 1079;
        case UTF8_GETCHR(u8"???"): return 1083;
        case UTF8_GETCHR(u8"???"): return 1087;
        case UTF8_GETCHR(u8"???"): return 1091;
        case UTF8_GETCHR(u8"???"): return 1095;
        case UTF8_GETCHR(u8"???"): return 1099;
        case UTF8_GETCHR(u8"???"): return 1103;
        case UTF8_GETCHR(u8"???"): return 1107;
        case UTF8_GETCHR(u8"???"): return 1111;
        case UTF8_GETCHR(u8"???"): return 1115;
        case UTF8_GETCHR(u8"???"): return 1119;
        case UTF8_GETCHR(u8"???"): return 1123;
        case UTF8_GETCHR(u8"???"): return 1127;
        case UTF8_GETCHR(u8"???"): return 1131;
        case UTF8_GETCHR(u8"???"): return 1135;
        case UTF8_GETCHR(u8"???"): return 1139;
        case UTF8_GETCHR(u8"???"): return 1143;
        case UTF8_GETCHR(u8"???"): return 1147;
        case UTF8_GETCHR(u8"???"): return 1151;
        case UTF8_GETCHR(u8"???"): return 1155;
        case UTF8_GETCHR(u8"???"): return 1159;
        case UTF8_GETCHR(u8"???"): return 1163;
        case UTF8_GETCHR(u8"???"): return 1167;
        case UTF8_GETCHR(u8"???"): return 1171;
        case UTF8_GETCHR(u8"???"): return 1175;
        case UTF8_GETCHR(u8"???"): return 1179;
        case UTF8_GETCHR(u8"???"): return 1183;
        case UTF8_GETCHR(u8"???"): return 1187;
        case UTF8_GETCHR(u8"???"): return 1191;
        case UTF8_GETCHR(u8"???"): return 1195;
        case UTF8_GETCHR(u8"???"): return 1199;
        case UTF8_GETCHR(u8"???"): return 1203;
        case UTF8_GETCHR(u8"???"): return 1207;
        case UTF8_GETCHR(u8"???"): return 1211;
        case UTF8_GETCHR(u8"???"): return 1215;
        case UTF8_GETCHR(u8"???"): return 1219;
        case UTF8_GETCHR(u8"???"): return 1223;
        case UTF8_GETCHR(u8"???"): return 1227;
        case UTF8_GETCHR(u8"???"): return 1231;
        case UTF8_GETCHR(u8"???"): return 1235;
        case UTF8_GETCHR(u8"???"): return 1239;
        case UTF8_GETCHR(u8"???"): return 1243;
        case UTF8_GETCHR(u8"???"): return 1247;
        case UTF8_GETCHR(u8"???"): return 1251;
        case UTF8_GETCHR(u8"???"): return 1255;
        // 1259 intentional break
        case UTF8_GETCHR(u8"???"): return 1263;
        case UTF8_GETCHR(u8"???"): return 1267;
        case UTF8_GETCHR(u8"???"): return 1271;
        case UTF8_GETCHR(u8"???"): return 1275;
        case UTF8_GETCHR(u8"???"): return 1279;
        case UTF8_GETCHR(u8"???"): return 1283;
        case UTF8_GETCHR(u8"???"): return 1287;
        case UTF8_GETCHR(u8"???"): return 1291;
        // case UTF8_GETCHR(u8"???"): return 1295; // re-use
        // case UTF8_GETCHR(u8"???"): return 1299; // re-use
        case UTF8_GETCHR(u8"???"): return 1303;
        case UTF8_GETCHR(u8"???"): return 1307;
        case UTF8_GETCHR(u8"???"): return 1311;
        case UTF8_GETCHR(u8"???"): return 1315;
        case UTF8_GETCHR(u8"???"): return 1319;
        case UTF8_GETCHR(u8"???"): return 1323;
        case UTF8_GETCHR(u8"???"): return 1327;
        case UTF8_GETCHR(u8"???"): return 1331;
        case UTF8_GETCHR(u8"???"): return 1335;
        case UTF8_GETCHR(u8"???"): return 1339;
        case UTF8_GETCHR(u8"???"): return 1343;
        case UTF8_GETCHR(u8"???"): return 1347;
        case UTF8_GETCHR(u8"???"): return 1351;
        case UTF8_GETCHR(u8"???"): return 1355;
        case UTF8_GETCHR(u8"???"): return 1359;
        case UTF8_GETCHR(u8"???"): return 1363;
        case UTF8_GETCHR(u8"???"): return 1367;
        case UTF8_GETCHR(u8"???"): return 1371;
        case UTF8_GETCHR(u8"???"): return 1375;
        case UTF8_GETCHR(u8"???"): return 1379;
        case UTF8_GETCHR(u8"???"): return 1383;
        // case UTF8_GETCHR(u8"???"): return 1387; re-use
        case UTF8_GETCHR(u8"???"): return 1391;
        case UTF8_GETCHR(u8"???"): return 1395;
        case UTF8_GETCHR(u8"???"): return 1399;

        case UTF8_GETCHR(u8"???"): return 1403;
        case UTF8_GETCHR(u8"???"): return 1407;
        case UTF8_GETCHR(u8"???"): return 1411;
        case UTF8_GETCHR(u8"???"): return 1415;
        case UTF8_GETCHR(u8"???"): return 1419;
        case UTF8_GETCHR(u8"???"): return 1423;
        case UTF8_GETCHR(u8"???"): return 1427;
        case UTF8_GETCHR(u8"???"): return 1431;
        case UTF8_GETCHR(u8"???"): return 1435;
        case UTF8_GETCHR(u8"???"): return 1439;
        case UTF8_GETCHR(u8"???"): return 1443;
        case UTF8_GETCHR(u8"???"): return 1447;
        case UTF8_GETCHR(u8"???"): return 1451;
        case UTF8_GETCHR(u8"???"): return 1455;
        case UTF8_GETCHR(u8"???"): return 1459;
        case UTF8_GETCHR(u8"???"): return 1463;
        case UTF8_GETCHR(u8"???"): return 1467;
        case UTF8_GETCHR(u8"???"): return 1471;

        case UTF8_GETCHR(u8"???"): return 1475;
        case UTF8_GETCHR(u8"???"): return 1479;
        case UTF8_GETCHR(u8"???"): return 1483;
        case UTF8_GETCHR(u8"???"): return 1487;
        case UTF8_GETCHR(u8"???"): return 1491;
        case UTF8_GETCHR(u8"???"): return 1495;
        case UTF8_GETCHR(u8"???"): return 1499;
        case UTF8_GETCHR(u8"???"): return 1503;
        case UTF8_GETCHR(u8"???"): return 1507;
        case UTF8_GETCHR(u8"???"): return 1511;
        case UTF8_GETCHR(u8"???"): return 1515;
        case UTF8_GETCHR(u8"???"): return 1519;
        case UTF8_GETCHR(u8"???"): return 1523;
        case UTF8_GETCHR(u8"???"): return 1527;
        case UTF8_GETCHR(u8"???"): return 1531;
        case UTF8_GETCHR(u8"???"): return 1535;
        case UTF8_GETCHR(u8"???"): return 1539;
        case UTF8_GETCHR(u8"???"): return 1543;
        case UTF8_GETCHR(u8"???"): return 1547;
        case UTF8_GETCHR(u8"???"): return 1551;
        case UTF8_GETCHR(u8"???"): return 1555;

        case UTF8_GETCHR(u8"???"): return 1559;
        case UTF8_GETCHR(u8"???"): return 1563;
        case UTF8_GETCHR(u8"???"): return 1567;
        case UTF8_GETCHR(u8"???"): return 1571;
        case UTF8_GETCHR(u8"???"): return 1575;
        case UTF8_GETCHR(u8"???"): return 1579;
        case UTF8_GETCHR(u8"???"): return 1583;
        case UTF8_GETCHR(u8"???"): return 1587;
        case UTF8_GETCHR(u8"???"): return 1591;
        case UTF8_GETCHR(u8"???"): return 1595;
        case UTF8_GETCHR(u8"???"): return 1599;
        case UTF8_GETCHR(u8"???"): return 1603;
        case UTF8_GETCHR(u8"???"): return 1607;
        case UTF8_GETCHR(u8"???"): return 1611;
        case UTF8_GETCHR(u8"???"): return 1615;
        case UTF8_GETCHR(u8"???"): return 1619;
        case UTF8_GETCHR(u8"???"): return 1623;
        case UTF8_GETCHR(u8"???"): return 1627;
        case UTF8_GETCHR(u8"???"): return 1631;
        case UTF8_GETCHR(u8"???"): return 1635;
        case UTF8_GETCHR(u8"???"): return 1639;
        case UTF8_GETCHR(u8"???"): return 1643;
        case UTF8_GETCHR(u8"???"): return 1647;
        case UTF8_GETCHR(u8"???"): return 1651;
        case UTF8_GETCHR(u8"???"): return 1655;
        case UTF8_GETCHR(u8"???"): return 1659;
        case UTF8_GETCHR(u8"???"): return 1663;
        case UTF8_GETCHR(u8"???"): return 1667;
        case UTF8_GETCHR(u8"???"): return 1671;

        case UTF8_GETCHR(u8"???"): return 1675;
        case UTF8_GETCHR(u8"???"): return 1679;
        case UTF8_GETCHR(u8"???"): return 1683;
        case UTF8_GETCHR(u8"???"): return 1687;
        case UTF8_GETCHR(u8"???"): return 1691;
        case UTF8_GETCHR(u8"???"): return 1695;
        case UTF8_GETCHR(u8"???"): return 1699;
        case UTF8_GETCHR(u8"???"): return 1703;
        case UTF8_GETCHR(u8"???"): return 1707;
        case UTF8_GETCHR(u8"???"): return 1711;
        case UTF8_GETCHR(u8"???"): return 1715;
        case UTF8_GETCHR(u8"???"): return 1719;
        case UTF8_GETCHR(u8"???"): return 1723;
        case UTF8_GETCHR(u8"???"): return 1727;
        case UTF8_GETCHR(u8"???"): return 1731;
        case UTF8_GETCHR(u8"???"): return 1735;
        case UTF8_GETCHR(u8"???"): return 1739;
        case UTF8_GETCHR(u8"???"): return 1743;
        case UTF8_GETCHR(u8"???"): return 1747;
        case UTF8_GETCHR(u8"???"): return 1751;
        // Intentional gap
        case UTF8_GETCHR(u8"???"): return 1803;
        case UTF8_GETCHR(u8"???"): return 1820;
        case UTF8_GETCHR(u8"???"): return 1824;
        case UTF8_GETCHR(u8"???"): return 1838;
        case UTF8_GETCHR(u8"???"): return 1842;
        case UTF8_GETCHR(u8"???"): return 1846;
        case UTF8_GETCHR(u8"???"): return 1850;
        case UTF8_GETCHR(u8"???"): return 1854;
        case UTF8_GETCHR(u8"???"): return 1858;
        case UTF8_GETCHR(u8"???"): return 1862;
        case UTF8_GETCHR(u8"???"): return 1866;
        case UTF8_GETCHR(u8"???"): return 1870;
        case UTF8_GETCHR(u8"???"): return 1874;
        case UTF8_GETCHR(u8"???"): return 1878;
        case UTF8_GETCHR(u8"???"): return 1882;
        case UTF8_GETCHR(u8"???"): return 1886;
        case UTF8_GETCHR(u8"???"): return 1890;
        case UTF8_GETCHR(u8"???"): return 1894;
        case UTF8_GETCHR(u8"???"): return 1898;
        case UTF8_GETCHR(u8"???"): return 1918;
        case UTF8_GETCHR(u8"???"): return 1922;
        case UTF8_GETCHR(u8"???"): return 1926;
        case UTF8_GETCHR(u8"???"): return 1930;
        case UTF8_GETCHR(u8"???"): return 1934;
        case UTF8_GETCHR(u8"???"): return 1938;
        case UTF8_GETCHR(u8"???"): return 1942;
        case UTF8_GETCHR(u8"???"): return 1946;
        case UTF8_GETCHR(u8"???"): return 1950;
        case UTF8_GETCHR(u8"???"): return 1954;
        case UTF8_GETCHR(u8"???"): return 1958;
        case UTF8_GETCHR(u8"???"): return 1962;
        case UTF8_GETCHR(u8"???"): return 1966;
        case UTF8_GETCHR(u8"???"): return 1970;
        case UTF8_GETCHR(u8"???"): return 1974;
        case UTF8_GETCHR(u8"???"): return 1978;
        case UTF8_GETCHR(u8"???"): return 1982;
        case UTF8_GETCHR(u8"???"): return 1986;
        case UTF8_GETCHR(u8"???"): return 1990;
        case UTF8_GETCHR(u8"???"): return 1994;
        case UTF8_GETCHR(u8"???"): return 1998;
        case UTF8_GETCHR(u8"???"): return 2002;
        case UTF8_GETCHR(u8"???"): return 2006;
        case UTF8_GETCHR(u8"???"): return 2010;
        case UTF8_GETCHR(u8"???"): return 2014;
        case UTF8_GETCHR(u8"???"): return 2018;
        case UTF8_GETCHR(u8"???"): return 2022;
        case UTF8_GETCHR(u8"???"): return 2026;
        case UTF8_GETCHR(u8"???"): return 2030;
        case UTF8_GETCHR(u8"???"): return 2034;
        case UTF8_GETCHR(u8"???"): return 2038;
        case UTF8_GETCHR(u8"???"): return 2042;
        case UTF8_GETCHR(u8"???"): return 2046;
        case UTF8_GETCHR(u8"???"): return 2050;
        case UTF8_GETCHR(u8"???"): return 2054;
        case UTF8_GETCHR(u8"???"): return 2058;
        case UTF8_GETCHR(u8"???"): return 2062;
        case UTF8_GETCHR(u8"???"): return 2066;

        case UTF8_GETCHR(u8"???"): return 2071;
        case UTF8_GETCHR(u8"???"): return 2075;
        case UTF8_GETCHR(u8"???"): return 2079;
        case UTF8_GETCHR(u8"???"): return 2083;
        case UTF8_GETCHR(u8"???"): return 2087;
        case UTF8_GETCHR(u8"???"): return 2091;

            // clang-format on

        default:
            if (cp == utf8::getc(u8"??")) {
                return 185;
            }
            // extended spanish and french characters
            else if (cp == utf8::getc(u8"??")) {
                return 73;
            } else if (cp == utf8::getc(u8"??")) {
                return 74;
            } else if (cp == utf8::getc(u8"??")) {
                return 75;
            } else if (cp == utf8::getc(u8"??")) {
                return 76;
            } else if (cp == utf8::getc(u8"??")) {
                return 77;
            } else if (cp == utf8::getc(u8"??")) {
                return 78;
            } else if (cp == utf8::getc(u8"??")) {
                return 79;
            } else if (cp == utf8::getc(u8"??")) {
                return 80;
            } else if (cp == utf8::getc(u8"??")) {
                return 81;
            } else if (cp == utf8::getc(u8"??")) {
                return 82;
            } else if (cp == utf8::getc(u8"??")) {
                return 83;
            } else if (cp == utf8::getc(u8"??")) {
                return 84;
            } else if (cp == utf8::getc(u8"??")) {
                return 85;
            } else if (cp == utf8::getc(u8"??")) {
                return 86;
            } else if (cp == utf8::getc(u8"??")) {
                return 87;
            } else if (cp == utf8::getc(u8"??")) {
                return 88;
            } else if (cp == utf8::getc(u8"??")) {
                return 89;
            } else if (cp == utf8::getc(u8"??")) {
                return 90;
            } else if (cp == utf8::getc(u8"??")) {
                return 91;
            } else if (cp == utf8::getc(u8"??")) {
                return 92;
            } else if (cp == utf8::getc(u8"??")) {
                return 96;
            } else if (cp == utf8::getc(u8"??")) {
                return 97;
            }
            // katakana
            else if (cp == utf8::getc(u8"???")) {
                return 106;
            } else if (cp == utf8::getc(u8"???")) {
                return 107;
            } else if (cp == utf8::getc(u8"???")) {
                return 108;
            } else if (cp == utf8::getc(u8"???")) {
                return 109;
            } else if (cp == utf8::getc(u8"???")) {
                return 110;
            } else if (cp == utf8::getc(u8"???")) {
                return 111;
            } else if (cp == utf8::getc(u8"???")) {
                return 112;
            } else if (cp == utf8::getc(u8"???")) {
                return 113;
            } else if (cp == utf8::getc(u8"???")) {
                return 114;
            } else if (cp == utf8::getc(u8"???")) {
                return 115;
            } else if (cp == utf8::getc(u8"???")) {
                return 116;
            } else if (cp == utf8::getc(u8"???")) {
                return 117;
            } else if (cp == utf8::getc(u8"???")) {
                return 118;
            } else if (cp == utf8::getc(u8"???")) {
                return 119;
            } else if (cp == utf8::getc(u8"???")) {
                return 120;
            } else if (cp == utf8::getc(u8"???")) {
                return 121;
            } else if (cp == utf8::getc(u8"???")) {
                return 122;
            } else if (cp == utf8::getc(u8"???")) {
                return 123;
            } else if (cp == utf8::getc(u8"???")) {
                return 124;
            } else if (cp == utf8::getc(u8"???")) {
                return 125;
            } else if (cp == utf8::getc(u8"???")) {
                return 126;
            } else if (cp == utf8::getc(u8"???")) {
                return 127;
            } else if (cp == utf8::getc(u8"???")) {
                return 128;
            } else if (cp == utf8::getc(u8"???")) {
                return 129;
            } else if (cp == utf8::getc(u8"???")) {
                return 130;
            } else if (cp == utf8::getc(u8"???")) {
                return 131;
            } else if (cp == utf8::getc(u8"???")) {
                return 132;
            } else if (cp == utf8::getc(u8"???")) {
                return 133;
            } else if (cp == utf8::getc(u8"???")) {
                return 134;
            } else if (cp == utf8::getc(u8"???")) {
                return 135;
            } else if (cp == utf8::getc(u8"???")) {
                return 136;
            } else if (cp == utf8::getc(u8"???")) {
                return 137;
            } else if (cp == utf8::getc(u8"???")) {
                return 138;
            } else if (cp == utf8::getc(u8"???")) {
                return 139;
            } else if (cp == utf8::getc(u8"???")) {
                return 140;
            } else if (cp == utf8::getc(u8"???")) {
                return 141;
            } else if (cp == utf8::getc(u8"???")) {
                return 142;
            } else if (cp == utf8::getc(u8"???")) {
                return 143;
            } else if (cp == utf8::getc(u8"???")) {
                return 144;
            } else if (cp == utf8::getc(u8"???")) {
                return 145;
            } else if (cp == utf8::getc(u8"???")) {
                return 146;
            } else if (cp == utf8::getc(u8"???")) {
                return 147;
            } else if (cp == utf8::getc(u8"???")) {
                return 148;
            } else if (cp == utf8::getc(u8"???")) {
                return 149;
            } else if (cp == utf8::getc(u8"???")) {
                return 150;
            } else if (cp == utf8::getc(u8"???")) {
                return 151;
            } else if (cp == utf8::getc(u8"???")) {
                return 152;
            } else if (cp == utf8::getc(u8"???")) {
                return 153;
            } else if (cp == utf8::getc(u8"???")) {
                return 154;
            } else if (cp == utf8::getc(u8"???")) {
                return 155;
            } else if (cp == utf8::getc(u8"???")) {
                return 156;
            } else if (cp == utf8::getc(u8"???")) {
                return 157;
            } else if (cp == utf8::getc(u8"???")) {
                return 158;
            } else if (cp == utf8::getc(u8"???")) {
                return 159;
            } else if (cp == utf8::getc(u8"???")) {
                return 160;
            } else if (cp == utf8::getc(u8"???")) {
                return 161;
            } else if (cp == utf8::getc(u8"???")) {
                return 162;
            } else if (cp == utf8::getc(u8"???")) {
                return 163;
            } else if (cp == utf8::getc(u8"???")) {
                return 164;
            } else if (cp == utf8::getc(u8"???")) {
                return 165;
            } else if (cp == utf8::getc(u8"???")) {
                return 166;
            } else if (cp == utf8::getc(u8"???")) {
                return 167;
            } else if (cp == utf8::getc(u8"???")) {
                return 168;
            } else if (cp == utf8::getc(u8"???")) {
                return 169;
            } else if (cp == utf8::getc(u8"???")) {
                return 170;
            } else if (cp == utf8::getc(u8"???")) {
                return 171;
            } else if (cp == utf8::getc(u8"???")) {
                return 172;
            } else if (cp == utf8::getc(u8"???")) {
                return 173;
            } else if (cp == utf8::getc(u8"???")) {
                return 174;
            } else if (cp == utf8::getc(u8"???")) {
                return 175;
            } else if (cp == utf8::getc(u8"???")) {
                return 176;
            } else if (cp == utf8::getc(u8"???")) {
                return 177;
            } else if (cp == utf8::getc(u8"???")) {
                return 178;
            } else if (cp == utf8::getc(u8"???")) {
                return 179;
            } else if (cp == utf8::getc(u8"???")) {
                return 180;
            } else if (cp == utf8::getc(u8"???")) {
                return 181;
            } else if (cp == utf8::getc(u8"???")) {
                return 182;
            } else if (cp == utf8::getc(u8"???")) {
                return 183;
            } else if (cp == utf8::getc(u8"???")) {
                return 184;
            }

            return std::nullopt;
        }
    }();
    if (mapping) {
        return Platform::TextureMapping{font_image, *mapping};
    } else {
        return {};
    }
}


std::optional<Platform::TextureMapping> null_texture_map(const utf8::Codepoint&)
{
    return {};
}


static int language_id = 0;


Platform::TextureCpMapper locale_texture_map()
{
    return standard_texture_map;
}


Platform::TextureCpMapper locale_doublesize_texture_map()
{
    return doublesize_texture_map;
}


void locale_set_language(int language_id)
{
    ::language_id = language_id;
}


// I had to add this code during chinese translation, for places where I needed
// to use traditional chinese numbers rather than arabic numerals.
const char* locale_repr_smallnum(u8 num, std::array<char, 40>& buffer)
{
    auto languages = lisp::get_var(lisp::make_symbol("languages"));

    auto lang = lisp::get_list(languages, ::language_id);

    const char* lang_name =
        lang->expect<lisp::Cons>().car()->expect<lisp::Symbol>().name_;

    if (str_cmp(lang_name, "chinese") == 0) {
        // Yeah, this is lazy. I could write a string to
        // number-to-unicode-string algorithm for chinese, but I don't feel like
        // it right now.
        switch (num) {
        default:
        case 1:
            return "???";
        case 2:
            return "???";
        case 3:
            return "???";
        case 4:
            return "???";
        case 5:
            return "???";
        case 6:
            return "???";
        case 7:
            return "???";
        case 8:
            return "???";
        case 9:
            return "???";
        case 10:
            return "???";
        case 11:
            return "??????";
        case 12:
            return "??????";
        case 13:
            return "??????";
        case 14:
            return "??????";
        case 15:
            return "??????";
        case 16:
            return "??????";
        case 17:
            return "??????";
        case 18:
            return "??????";
        case 19:
            return "??????";
        case 20:
            return "??????";
        case 21:
            return "?????????";
        case 22:
            return "?????????";
        case 23:
            return "?????????";
        case 24:
            return "?????????";
        case 25:
            return "?????????";
        case 26:
            return "?????????";
        case 27:
            return "?????????";
        case 28:
            return "?????????";
        case 29:
            return "?????????";
        case 30:
            return "??????";
        case 31:
            return "?????????";
        case 32:
            return "?????????";
        case 33:
            return "?????????";
        case 34:
            return "?????????";
        case 35:
            return "?????????";
        case 36:
            return "?????????";
        case 37:
            return "?????????";
        case 38:
            return "?????????";
        case 39:
            return "?????????";
        case 40:
            return "??????";
        case 41:
            return "?????????";
        case 42:
            return "?????????";
        case 43:
            return "?????????";
        case 44:
            return "?????????";
        case 45:
            return "?????????";
        case 46:
            return "?????????";
        case 47:
            return "?????????";
        case 48:
            return "?????????";
        case 49:
            return "?????????";
        }
    } else {
        // Arabic numerals
        locale_num2str(num, buffer.data(), 10);
        return buffer.data();
    }
}


int locale_get_language()
{
    return ::language_id;
}


StringBuffer<31> locale_language_name(int language)
{
    auto languages = lisp::get_var(lisp::make_symbol("languages"));

    auto lang = lisp::get_list(languages, language);

    return lang->expect<lisp::Cons>().car()->expect<lisp::Symbol>().name_;
}


LocalizedText locale_localized_language_name(Platform& pfrm, int language)
{
    const auto cached_lang = ::language_id;

    ::language_id = language;

    auto ret = locale_string(pfrm, LocaleString::language_name);

    ::language_id = cached_lang;

    return ret;
}


bool locale_requires_doublesize_font()
{
    auto languages = lisp::get_var(lisp::make_symbol("languages"));

    auto lang = lisp::get_list(languages, ::language_id);

    return lang->expect<lisp::Cons>()
               .cdr()
               ->expect<lisp::Cons>()
               .car()
               ->expect<lisp::Integer>()
               .value_ == 2;
}


LocalizedText locale_string(Platform& pfrm, LocaleString ls)
{
    auto result = allocate_dynamic<LocalizedStrBuffer>(pfrm);

    auto languages = lisp::get_var(lisp::make_symbol("languages"));

    auto lang = lisp::get_list(languages, ::language_id);

    StringBuffer<31> fname =
        lang->expect<lisp::Cons>().car()->expect<lisp::Symbol>().name_;
    fname += ".txt";

    if (auto data = pfrm.load_file_contents("strings", fname.c_str())) {
        const int target_line = static_cast<int>(ls);

        int index = 0;
        while (index not_eq target_line) {
            while (*data not_eq '\n') {
                if (*data == '\0') {
                    pfrm.fatal("null byte in localized text");
                }
                ++data;
            }
            ++data;

            ++index;
        }

        while (*data not_eq '\0' and *data not_eq '\n') {
            result->push_back(*data);
            ++data;
        }

        return result;
    } else {
        pfrm.fatal("missing strings file for language");
    }
}


void english__to_string(int num, char* buffer, int base)
{
    int i = 0;
    bool is_negative = false;

    if (num == 0) {
        buffer[i++] = '0';
        buffer[i] = '\0';
        return;
    }

    // Based on the behavior of itoa()
    if (num < 0 && base == 10) {
        is_negative = true;
        num = -num;
    }

    while (num != 0) {
        int rem = num % base;
        buffer[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
        num = num / base;
    }

    if (is_negative) {
        buffer[i++] = '-';
    }

    buffer[i] = '\0';

    str_reverse(buffer, i);

    return;
}


void locale_num2str(int num, char* buffer, int base)
{
    // FIXME!!!

    // switch (language) {
    // case LocaleLanguage::spanish:
    // case LocaleLanguage::english:
    english__to_string(num, buffer, base);
    //     break;

    // default:
    // case LocaleLanguage::null:
    // buffer[0] = '\0';
    //     break;
    // }
}
