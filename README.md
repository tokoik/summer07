# summer07 - 今風 OpenGL の使い方（第１０回 球を三角形で描く）サンプルプログラム

## 1. 概要

このプログラムは、OpenGL の **頂点バッファオブジェクト (VBO)** と **指標（インデックスバッファ）** を用いて、経度分割数（`slices`）および緯度分割数（`stacks`）に応じた三角形メッシュによる球（`solidSphere`）の頂点データを動的に生成し、`glDrawElements(GL_TRIANGLES, ...)` および **隠面消去処理（`GL_DEPTH_TEST`）** により描画する手順を学ぶための、学生向けのサンプルプログラムです。本プログラムは、以下のブログ記事の解説に沿って学習を進めるための雛形として提供されています。

- [第１０回 球を三角形で描く](https://tokoik.github.io/blog/今風%20opengl%20の使い方/2009/09/12/glsl.html)

四角形グリッド状に配置した三角形メッシュを球面に丸め込み、デプスバッファを有効にしてソリッドな球を描画する手法を学習します。

## 2. 対応環境

- **Windows**: Windows 10 / 11, Visual Studio 2022 (MSVC C++17)
- **macOS**: macOS 12 Monterey 以降, Xcode 14 以降 / Command Line Tools
- **Linux**: Ubuntu 22.04 LTS 以降, GCC / Clang (C++17 対応コンパイラ)
- **ビルドツール**: CMake 3.22 以降

## 3. ビルド手順

このプログラムは [CMake](https://cmake.org/) を用いてビルド環境を整備します。各 OS とも、ソースコードが置かれているディレクトリにターミナル（またはコマンドプロンプト）で移動してから、以下の手順を実行してください。なお、プログラムをビルドするためのバイナリディレクトリは、バージョン管理ファイル（.gitignore）の設定に合わせて **build** という名前にします。

> cmake-gui で設定することも可能です。その際は、`Source code path` にはプロジェクトのフォルダを指定し、`Build path` にはプロジェクトのフォルダの中に作った build というフォルダを指定してください。その後、`Configure` → `Generate` の順にクリックした後、`Open Project` をクリックすれば、開発環境が起動します。

### 3.1 Windows (Visual Studio 2022 の場合)

1. コマンドプロンプトまたは PowerShell を開き、このプロジェクトのディレクトリに移動します。
2. 以下のコマンドを実行してビルドディレクトリを作成し、CMake で構成を行います。

   ```bat
   mkdir build
   cd build
   cmake .. -G "Visual Studio 17 2022"
   ```

3. 生成された build フォルダ内の `summer07.sln` を Visual Studio で開きます。
4. ソリューションエクスプローラーで **summer07** プロジェクトを右クリックし、「スタートアップ プロジェクトに設定」を選択します。
5. 「ローカル Windows デバッガー」をクリックするか、F5 キーを押してビルドおよび実行します。

### 3.2 macOS (Xcode の場合)

1. ターミナルを開き、このプロジェクトのディレクトリに移動します。
2. 以下のコマンドを実行してビルドディレクトリを作成し、Xcode 用のプロジェクトを生成します。

   ```sh
   mkdir build
   cd build
   cmake .. -G Xcode
   ```

3. 生成された `build/summer07.xcodeproj` を Xcode で開きます。
4. 左上のスキーム選択（再生ボタンの横）が **summer07** になっていることを確認します。
5. 「Run」ボタン（再生ボタン）をクリックするか、Command + R を押してビルドおよび実行します。

### 3.3 Ubuntu Linux

1. ターミナルを開き、このプロジェクトのディレクトリに移動します。
2. 必要なパッケージ（freeglut3-dev など）がインストールされていることを確認し、以下のコマンドでビルドします。

   ```sh
   mkdir build
   cd build
   cmake ..
   make
   ```

## 4. 起動方法

各 OS とも、ビルド後に生成されるバイナリディレクトリ (build) やそのサブフォルダから起動します。

- **Windows**

  Visual Studio 上で「ローカル Windows デバッガー」をクリックして実行するか、またはコマンドプロンプトから以下のコマンドで起動します。

  ```cmd
  cd build\Debug
  summer07.exe
  ```

- **macOS**

  Xcode 上で左上の「Run（再生ボタン）」をクリックして実行します。アプリケーションバンドルを直接起動する場合は、Finder から `build/Debug/summer07.app` を開くか、ターミナルから `open build/Debug/summer07.app` を実行します。

- **Ubuntu Linux**

  ターミナルから以下のコマンドで実行ファイル（バイナリ）を直接起動します。

  ```sh
  cd build
  ./summer07
  ```

## 5. 操作方法

- 経度 16 分割、緯度 8 分割の赤いソリッド球が表示されます。

## 6. プログラムの解説

### 6.1 三角形メッシュ球の生成 (solidSphere)

```cpp
GLuint solidSphere(int slices, int stacks, const GLuint* buffer)
{
  /* 頂点の数 */
  GLuint vertices = (slices + 1) * (stacks + 1);

  /* 頂点のデータ型 */
  typedef GLfloat Position[3];

  /* 頂点バッファオブジェクトにメモリ領域を確保する */
  glBindBuffer(GL_ARRAY_BUFFER, buffer[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Position) * vertices, NULL, GL_STATIC_DRAW);

  /* 頂点バッファオブジェクトのメモリをプログラムのメモリ空間にマップする */
  Position* position = (Position*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);

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

  /* 頂点バッファオブジェクトのメモリをプログラムのメモリ空間から切り離す */
  glUnmapBuffer(GL_ARRAY_BUFFER);

  /* 頂点バッファオブジェクトを解放する */
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  /* 面の数 */
  GLuint faces = slices * stacks * 2;

  /* 面のデータ型 */
  typedef GLuint Face[3];

  /* 頂点バッファオブジェクトにメモリ領域を確保する */
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer[1]);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Face) * faces, NULL, GL_STATIC_DRAW);

  /* 頂点バッファオブジェクトのメモリをプログラムのメモリ空間にマップする */
  Face* face = (Face*)glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY);

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

  /* 頂点バッファオブジェクトを解放する */
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  return faces * 3;
}
```
