#include <vector>
#include <memory>

#include <glade/util/FileManager.h>
#include <glade/render/Drawable.h>
#include <glade/render/Texture.h>
#include <glade/render/meshes/Rectangle.h>
#include <glade/ui/font/FreetypeFont.h>

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_BITMAP_H
#include FT_GLYPH_H
#include FT_BBOX_H

extern FileManager *file_manager;

static FT_Library freeType = NULL;
static FT_Error   error    = FT_Err_Ok;

unsigned int FreetypeFont::instances = 0;

FreetypeFont::FreetypeFont(Path &face_file_path, int viewport_width, int viewport_height):
  minViewportDimension(
    (float) (viewport_width < viewport_height ? viewport_width : viewport_height)
  ),
  stringScaleX(0),
  stringScaleY(0),
  verticalCorrection(0)
{
  if (instances == 0) {
    error = FT_Init_FreeType(&freeType);
    checkFreeTypeError();
  }
  
  file_manager->getFileContents(face_file_path, faceBuffer, true);
  error = FT_New_Memory_Face(
    freeType,
    faceBuffer.data(),
    faceBuffer.size(),
    0,
    &face
  );
  checkFreeTypeError();
  
  ++instances;
}

FreetypeFont::~FreetypeFont()
{
  --instances;
  
  FT_Done_Face(face);
  
  if (instances == 0) {
    error = FT_Done_FreeType(freeType);
    checkFreeTypeError();
  }
}

void FreetypeFont::setFontSizePixels(unsigned int width, unsigned int height)
{
  error = FT_Set_Pixel_Sizes(face, width, height);
  checkFreeTypeError();
}


// Should be manually freed after use
GladeObject::Drawables* FreetypeFont::createDrawablesForString(const std::string &string)
{
  measureString(string);
  /* compute string dimensions in integer pixels */
  FT_BBox string_bbox;
  computeStringBbox(&string_bbox);
  
  int string_width  = string_bbox.xMax - string_bbox.xMin;
  int string_height = string_bbox.yMax - string_bbox.yMin;
  stringScaleX = (float) string_width  / minViewportDimension;
  stringScaleY = (float) string_height / minViewportDimension;

  float startingPenPositionXCoords = -stringScaleX;
  float startingPenPositionYCoords = stringScaleY;
  
  float firstGlyphBearingX = 0;
  
  GladeObject::Drawables* renderedText = new GladeObject::Drawables(); 

  for (int n = 0; n < glyphs.size(); n++)
  {
    FT_Glyph   image;

    image = glyphs[n];
    float penPositionXCoords = startingPenPositionXCoords + (float) pos[n].x * 2.0f / minViewportDimension;
    float penPositionYCoords = startingPenPositionYCoords + (float) pos[n].y * 2.0f / minViewportDimension;
    
    error = FT_Glyph_To_Bitmap(&image, FT_RENDER_MODE_NORMAL, 0, 0);
    checkFreeTypeError();
    
    FT_BitmapGlyph bit = (FT_BitmapGlyph) image;

    Drawable *drawable = createDrawableFromBitmap(&bit->bitmap);
    
    if (!drawable) {
      continue;
    }
    
    drawable->getTransform()->getPosition()->x =
      penPositionXCoords
      + drawable->getTransform()->getScale()->x;
    
    if (n > 0) {
      drawable->getTransform()->getPosition()->x +=
        (float) (bit->left) * 2.0f / minViewportDimension;
        
      drawable->getTransform()->getPosition()->x -= firstGlyphBearingX;
    } else {
      firstGlyphBearingX = (float) (bit->left) * 2.0f / minViewportDimension;
    }
    
    drawable->getTransform()->getPosition()->y =
      penPositionYCoords
      + drawable->getTransform()->getScale()->y
      - (float) (bit->top) * 2.0f / minViewportDimension
      - (float) verticalCorrection * 2.0f / minViewportDimension;
    
    renderedText->push_back(drawable);
    
    FT_Done_Glyph(image);
  }
  
  return renderedText;
}


void FreetypeFont::measureString(const std::string &string)
{
  FT_UInt       glyph_index;
  FT_Bool       use_kerning;
  FT_UInt       previous;
  int           pen_x, pen_y, i;

  pen_x = 0;
  pen_y = 0;

  glyphs.resize(string.length());
  pos.resize(string.length());
  verticalCorrection = 0;
  
  use_kerning = FT_HAS_KERNING( face );

  if (use_kerning)
    log("HAS KERNING");
  else
    log("NO KERNING");
    
  previous = 0;

  for (i = 0; i < string.length(); i++)
  {
    /* convert character code to glyph index */
    glyph_index = FT_Get_Char_Index(face, string[i]);

    /* retrieve kerning distance and move pen position */
    if ( use_kerning && previous && glyph_index )
    {
      FT_Vector  delta;
      FT_Get_Kerning( face, previous, glyph_index,
                      FT_KERNING_DEFAULT, &delta );
      pen_x += delta.x >> 6;
    }
    
    /* store current pen position */
    pos[i].x = pen_x;
    pos[i].y = pen_y;

    /* load glyph image into the slot without rendering */
    error = FT_Load_Glyph(face, glyph_index, FT_LOAD_DEFAULT);
    checkFreeTypeError();

    /* extract glyph image and store it in our table */
    error = FT_Get_Glyph(face->glyph, &glyphs[i]);
    checkFreeTypeError();

    int glyphVerticalCorrection =
       (face->glyph->metrics.height - face->glyph->metrics.horiBearingY) >> 6;
       
    log("GLYPH %c VCORR: %d", string[i], glyphVerticalCorrection);
    
    verticalCorrection = verticalCorrection < glyphVerticalCorrection
      ? glyphVerticalCorrection : verticalCorrection;
    
    /* increment pen position */
    pen_x += face->glyph->advance.x >> 6;

    /* record current glyph index */
    previous = glyph_index;
  }
}


void FreetypeFont::computeStringBbox(FT_BBox *bbox)
{
  FT_BBox  glyphBbox;

  /* initialize string bbox to "empty" values */
  bbox->xMin = bbox->yMin =  32000;
  bbox->xMax = bbox->yMax = -32000;

  /* for each glyph image, compute its bounding box, */
  /* translate it, and grow the string bbox          */
  for (int n = 0; n < glyphs.size(); n++ )
  {
    FT_Glyph_Get_CBox( glyphs[n], ft_glyph_bbox_pixels,
                       &glyphBbox );

    glyphBbox.xMin += pos[n].x;
    glyphBbox.xMax += pos[n].x;
    glyphBbox.yMin += pos[n].y;
    glyphBbox.yMax += pos[n].y;

    if ( glyphBbox.xMin < bbox->xMin )
      bbox->xMin = glyphBbox.xMin;

    if ( glyphBbox.yMin < bbox->yMin )
      bbox->yMin = glyphBbox.yMin;

    if ( glyphBbox.xMax > bbox->xMax )
      bbox->xMax = glyphBbox.xMax;

    if ( glyphBbox.yMax > bbox->yMax )
      bbox->yMax = glyphBbox.yMax;
  }

  /* check that we really grew the string bbox */
  if ( bbox->xMin > bbox->xMax )
  {
    bbox->xMin = 0;
    bbox->yMin = 0;
    bbox->xMax = 0;
    bbox->yMax = 0;
  }
}


Drawable *FreetypeFont::createDrawableFromBitmap(FT_Bitmap *bitmap)
{
  log("BITMAP:");
  log("WIDTH: %d", bitmap->width);
  log("PITCH: %d", bitmap->pitch);
  log("ROWS:  %d", bitmap->rows);
  log("NUM GRAYS:  %d", bitmap->num_grays);
  log("PIXEL MODE:  %d", bitmap->pixel_mode);
  
  if (bitmap->width <= 0 || bitmap->rows <= 0)
    return NULL;
    
  unsigned int   size   = bitmap->pitch * bitmap->rows * 4;
  unsigned char *pixmap = new unsigned char[size];
  unsigned char *srcPtr = bitmap->buffer;
  int i = 0;
  
  for (unsigned char *ptr = pixmap; i < size/4; ptr += 4, srcPtr++, i++) {
    *(ptr + 3) = *srcPtr;
  }
  
  std::shared_ptr<Texture> texture(
    new Texture(bitmap->width, bitmap->rows, 1, 1, pixmap, size));
  
  delete[] pixmap;
  
  Drawable *drawable = new Drawable(Rectangle::INSTANCE, shaderProgram);
  drawable->getTransform()->setScale(
    (float) bitmap->width / minViewportDimension,
    (float) bitmap->rows  / minViewportDimension,
    1
  );
  
  drawable->setTexture(texture);
  return drawable;
}


Drawable *FreetypeFont::createDrawableForUTF32Char(FT_ULong charcode)
{
  FT_UInt glyphIndex = FT_Get_Char_Index(face, charcode);
  error = FT_Load_Glyph(face, glyphIndex, FT_LOAD_DEFAULT);
  checkFreeTypeError();
  
  if (face->glyph->format != FT_GLYPH_FORMAT_BITMAP) {
    error = FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);
    checkFreeTypeError();
  }
  
  FT_Bitmap *bitmap = &face->glyph->bitmap;
  return createDrawableFromBitmap(bitmap);
}

void FreetypeFont::checkFreeTypeError()
{
  assert(error == FT_Err_Ok);
}
    