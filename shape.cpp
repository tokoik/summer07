#include <math.h>
#include <stdlib.h>
#if defined(WIN32)
#  pragma warning(disable: 4996)
#  include "glew.h"
#  include "glut.h"
#  include "glext.h"
#elif defined(__APPLE__) || defined(MACOSX)
#  include <GLUT/glut.h>
#else
#  define GL_GLEXT_PROTOTYPES
#  include <GL/glut.h>
#endif

/* 頂点バッファオブジェクトのメモリを参照するポインタのデータ型 */
typedef GLfloat Position[3];
typedef GLuint Edge[2];
typedef GLuint Face[3];

/*
** 立方体を線で描く
*/
GLuint wireCube(const GLuint *buffer)
{
  /* 頂点バッファオブジェクトに８頂点分のメモリ領域を確保する */
  glBindBuffer(GL_ARRAY_BUFFER, buffer[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof (Position) * 8, NULL, GL_STATIC_DRAW);

  /* 頂点バッファオブジェクトのメモリをプログラムのメモリ空間にマップする */
  Position *position = (Position *)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);

  /* 頂点バッファオブジェクトのメモリにデータを書き込む */
  position[0][0] = -1.0f;
  position[0][1] = -1.0f;
  position[0][2] = -1.0f;

  position[1][0] =  1.0f;
  position[1][1] = -1.0f;
  position[1][2] = -1.0f;

  position[2][0] =  1.0f;
  position[2][1] = -1.0f;
  position[2][2] =  1.0f;

  position[3][0] = -1.0f;
  position[3][1] = -1.0f;
  position[3][2] =  1.0f;

  position[4][0] = -1.0f;
  position[4][1] =  1.0f;
  position[4][2] = -1.0f;

  position[5][0] =  1.0f;
  position[5][1] =  1.0f;
  position[5][2] = -1.0f;

  position[6][0] =  1.0f;
  position[6][1] =  1.0f;
  position[6][2] =  1.0f;

  position[7][0] = -1.0f;
  position[7][1] =  1.0f;
  position[7][2] =  1.0f;

  /* 頂点バッファオブジェクトに１２稜線分のメモリ領域を確保する */
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer[1]);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof (Edge) * 12, NULL, GL_STATIC_DRAW);

  /* 頂点バッファオブジェクトのメモリをプログラムのメモリ空間にマップする */
  Edge *edge = (Edge *)glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY);

  /* 頂点バッファオブジェクトのメモリにデータを書き込む */
  edge[ 0][0] = 0;
  edge[ 0][1] = 1;

  edge[ 1][0] = 1;
  edge[ 1][1] = 2;

  edge[ 2][0] = 2;
  edge[ 2][1] = 3;

  edge[ 3][0] = 3;
  edge[ 3][1] = 0;

  edge[ 4][0] = 0;
  edge[ 4][1] = 4;

  edge[ 5][0] = 1;
  edge[ 5][1] = 5;

  edge[ 6][0] = 2;
  edge[ 6][1] = 6;

  edge[ 7][0] = 3;
  edge[ 7][1] = 7;

  edge[ 8][0] = 4;
  edge[ 8][1] = 5;

  edge[ 9][0] = 5;
  edge[ 9][1] = 6;

  edge[10][0] = 6;
  edge[10][1] = 7;

  edge[11][0] = 7;
  edge[11][1] = 4;

  /* 頂点バッファオブジェクトのメモリをプログラムのメモリ空間から切り離す */
  glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);

  /* 頂点バッファオブジェクトを解放する */
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  /* 頂点バッファオブジェクトのメモリをプログラムのメモリ空間から切り離す */
  glUnmapBuffer(GL_ARRAY_BUFFER);

  /* 頂点バッファオブジェクトを解放する */
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  return 24;
}

/*
** 球を線で描く
*/
GLuint wireSphere(int slices, int stacks, const GLuint *buffer)
{
  GLuint vertices = slices * stacks - slices + 2;
  GLuint edges = slices * stacks * 2 - slices;

  /* 頂点バッファオブジェクトを有効にする */
  glBindBuffer(GL_ARRAY_BUFFER, buffer[0]);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer[1]);

  /* 頂点バッファオブジェクトにメモリ領域を確保する */
  glBufferData(GL_ARRAY_BUFFER, sizeof (Position) * vertices, NULL, GL_STATIC_DRAW);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof (Edge) * edges, NULL, GL_STATIC_DRAW);

  /* 頂点バッファオブジェクトのメモリをプログラムのメモリ空間にマップする */
  Position *position = (Position *)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
  Edge *edge = (Edge *)glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY);

  /* 北極点の位置 */
  (*position)[0] =  0.0f;
  (*position)[1] =  1.0f;
  (*position)[2] =  0.0f;
  ++position;

  /* 中間部分の頂点の位置 */
  for (int j = 1; j < stacks; ++j) {
    float ph = 3.141593f * (float)j / (float)stacks;
    float y = cosf(ph);
    float r = sinf(ph);

    for (int i = 0; i < slices; ++i) {
      float th = 2.0f * 3.141593f * (float)i / (float)slices;
      float x = r * cosf(th);
      float z = r * sinf(th);

      (*position)[0] = x;
      (*position)[1] = y;
      (*position)[2] = z;
      ++position;
    }
  }

  /* 南極点の位置 */
  (*position)[0] =  0.0f;
  (*position)[1] = -1.0f;
  (*position)[2] =  0.0f;

  int count = 1;

  /* 北極点周りの稜線 */
  for (int i = 1; i <= slices; ++i) {
    (*edge)[0] = 0;
    (*edge)[1] = i;
    ++edge;
  }

  /* 中間部分の稜線 */
  for (int j = 2; j < stacks; ++j) {
    for (int i = 1; i < slices; ++i) {

      /* 右方向 */
      (*edge)[0] = count;
      (*edge)[1] = count + 1;
      ++edge;

      /* 下方向 */
      (*edge)[0] = count;
      (*edge)[1] = count + slices;
      ++edge;

      ++count;
    }

    /* 右端の右方向の稜線は左端の頂点に接続する */
    (*edge)[0] = count;
    (*edge)[1] = count - slices + 1;
    ++edge;

    /* 下方向 */
    (*edge)[0] = count;
    (*edge)[1] = count + slices;
    ++edge;

    ++count;
  }

  /* 最下段の稜線 */
  for (int i = 1; i < slices; ++i) {

    /* 右方向 */
    (*edge)[0] = count;
    (*edge)[1] = count + 1;
    ++edge;

    /* 下方向の稜線は南極点に接続する */
    (*edge)[0] = count;
    (*edge)[1] = vertices - 1;
    ++edge;

    ++count;
  }

  /* 右端の右方向の稜線は左端の頂点に接続する */
  (*edge)[0] = count;
  (*edge)[1] = count - slices + 1;
  ++edge;

  /* 下方向の稜線は南極点に接続する */
  (*edge)[0] = count;
  (*edge)[1] = vertices - 1;

  /* 頂点バッファオブジェクトのメモリをプログラムのメモリ空間から切り離す */
  glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
  glUnmapBuffer(GL_ARRAY_BUFFER);

  /* 頂点バッファオブジェクトを解放する */
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  return edges * 2;
}

/*
** 球を三角形で描く
*/
GLuint solidSphere(int slices, int stacks, const GLuint *buffer)
{
  GLuint vertices = (slices + 1) * (stacks + 1);
  GLuint faces = slices * stacks * 2;

  /* 頂点バッファオブジェクトを有効にする */
  glBindBuffer(GL_ARRAY_BUFFER, buffer[0]);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer[1]);

  /* 頂点バッファオブジェクトにメモリ領域を確保する */
  glBufferData(GL_ARRAY_BUFFER, sizeof (Position) * vertices, NULL, GL_STATIC_DRAW);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof (Face) * faces, NULL, GL_STATIC_DRAW);

  /* 頂点バッファオブジェクトのメモリをプログラムのメモリ空間にマップする */
  Position *position = (Position *)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
  Face *face = (Face *)glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY);

  /* 頂点の位置 */
  for (int j = 0; j <= stacks; ++j) {
    float ph = 3.141593f * (float)j / (float)stacks;
    float y = cosf(ph);
    float r = sinf(ph);

    for (int i = 0; i <= slices; ++i) {
      float th = 2.0f * 3.141593f * (float)i / (float)slices;
      float x = r * cosf(th);
      float z = r * sinf(th);

      (*position)[0] = x;
      (*position)[1] = y;
      (*position)[2] = z;
      ++position;
    }
  }

  /* 面の指標 */
  for (int j = 0; j < stacks; ++j) {
    for (int i = 0; i < slices; ++i) {
      int count = (slices + 1) * j + i;

      /* 上半分 */
      (*face)[0] = count;
      (*face)[1] = count + 1;
      (*face)[2] = count + slices + 2;
      ++face;

      /* 下半分 */
      (*face)[0] = count;
      (*face)[1] = count + slices + 2;
      (*face)[2] = count + slices + 1;
      ++face;
    }
  }

  /* 頂点バッファオブジェクトのメモリをプログラムのメモリ空間から切り離す */
  glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
  glUnmapBuffer(GL_ARRAY_BUFFER);

  /* 頂点バッファオブジェクトを解放する */
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  return faces * 3;
}
