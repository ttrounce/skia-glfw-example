
#define SK_GANESH
#define SK_GL
#include <include/gpu/ganesh/SkSurfaceGanesh.h>
#include "include/gpu/GrBackendSurface.h"
#include "include/gpu/GrDirectContext.h"
#include "include/gpu/gl/GrGLInterface.h"
#include "include/core/SkCanvas.h"
#include "include/core/SkColorSpace.h"
#include "include/core/SkSurface.h"
#include "include/core/SkTypeface.h"
#include "include/core/SkFont.h"
#include "include/core/SkTextBlob.h"
#include "include/core/SkBitmap.h"
#include "include/core/SkFontMetrics.h"
#include "include/encode/SkPngEncoder.h"

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>

#include <fmt/core.h>
#include <string>

int main()
{
    SkFont font;
    font.setSize(128);

    SkFontMetrics metrics{};
    font.getMetrics(&metrics);

    std::string text = "Hello World";
    int textWidth = font.measureText(text.c_str(), text.size(), SkTextEncoding::kUTF8);
    int textAscent = (SkScalar) abs(metrics.fAscent);
    int textHeight = font.getSpacing();

    SkBitmap bitmap{};
    bitmap.allocN32Pixels(textWidth, textHeight);
    auto canvas = SkCanvas(bitmap);

    auto blob = SkTextBlob::MakeFromString(text.c_str(), font);

    SkPaint paint{};
    paint.setAntiAlias(true);
    paint.setColor(SkColors::kWhite);

    SkPaint backPaint{};
    backPaint.setColor(SkColors::kBlue);

    // canvas.drawRect(SkRect::MakeXYWH(0, 0, textWidth, textHeight), backPaint);
    canvas.drawTextBlob(blob, 0, textAscent, paint);

    canvas.flush();

    std::ofstream out("test.png", std::ios::binary);

    auto pngImage = SkPngEncoder::Encode(nullptr, bitmap.asImage().get(), SkPngEncoder::Options{});
    out.write((const char*) pngImage->data(), pngImage->size());
    out.close();


    return 0;
}