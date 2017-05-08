#pragma once

#define GLADE_POS_SIZE_FLOATS          3
// The fourth number of aNormal indicates wether the vertex should be transparent
#define GLADE_NORMAL_SIZE_FLOATS       4
#define GLADE_TEXCOORD_SIZE_FLOATS     2
#define GLADE_POS_OFFSET_FLOATS        0
#define GLADE_NORMAL_OFFSET_FLOATS     GLADE_POS_SIZE_FLOATS
#define GLADE_TEXCOORD_OFFSET_FLOATS  (GLADE_POS_SIZE_FLOATS + GLADE_NORMAL_SIZE_FLOATS)
#define GLADE_VERTEX_STRIDE_FLOATS    (GLADE_POS_SIZE_FLOATS + GLADE_NORMAL_SIZE_FLOATS + GLADE_TEXCOORD_SIZE_FLOATS)
#define GLADE_POS_OFFSET_BYTES        (GLADE_POS_OFFSET_FLOATS      * sizeof(float))
#define GLADE_NORMAL_OFFSET_BYTES     (GLADE_NORMAL_OFFSET_FLOATS   * sizeof(float))
#define GLADE_TEXCOORD_OFFSET_BYTES   (GLADE_TEXCOORD_OFFSET_FLOATS * sizeof(float))
#define GLADE_VERTEX_STRIDE_BYTES     (GLADE_VERTEX_STRIDE_FLOATS   * sizeof(float))
