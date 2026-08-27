# summer07 - 今風 OpenGL の使い方（第１０回 球を三角形で描く）サンプルプログラム

## 1. 概要

このプログラムは、OpenGL の **頂点バッファオブジェクト (VBO)** と **指標（インデックスバッファ）** を用いて、経度分割数（`slices`）および緯度分割数（`stacks`）に応じた三角形メッシュによる球（`solidSphere`）の頂点データを動的に生成し、`glDrawElements(GL_TRIANGLES, ...)` および **隠面消去処理（`GL_DEPTH_TEST`）** により描画する手順を学ぶための、学生向けのサンプルプログラムです。本プログラムは、以下のブログ記事の解説に沿って学習を進めるための雛形として提供されています。

- [第１０回 球を三角形で描く](https://tokoik.github.io/blog/glsl/2009/09/12/glsl.html)

四角形グリッド状に配置した三角形メッシュを球面に丸め込み、デプスバッファを有効にしてソリッドな球を描画する手法を学習します。

## 2. ビルド方法

このプログラムは [CMake](https://cmake.org/) を用いてビルド環境を整備します。各OSとも、ソースコードが置かれているディレクトリにターミナル（またはコマンドプロンプト）で移動してから、以下の手順を実行してください。なお、プログラムをビルドするためのバイナリディレクトリは、バージョン管理ファイル（.gitignore）の設定に合わせて **build** という名前にします。

### 2.1 Windows (Visual Studio 2022 の場合)

1. コマンドプロンプトまたは PowerShell を開き、このプロジェクトのディレクトリに移動します。
2. 以下のコマンドを実行してビルドディレクトリを作成し、CMake で構成を行います。

   ```bat
   mkdir build
   cd build
   cmake .. -G "Visual Studio 17 2022"
   ```

3. 生成された build フォルダ内の summer07.sln を Visual Studio で開きます。
4. ソリューションエクスプローラーで **summer07** プロジェクトを右クリックし、「スタートアップ プロジェクトに設定」を選択します。
5. 「ローカル Windows デバッガー」をクリックするか、F5 キーを押してビルドおよび実行します。

### 2.2 macOS (Xcode の場合)

1. ターミナルを開き、このプロジェクトのディレクトリに移動します。
2. 以下のコマンドを実行してビルドディレクトリを作成し、Xcode 用のプロジェクトを生成します。

   ```sh
   mkdir build
   cd build
   cmake .. -G Xcode
   ```

3. 生成された build/summer07.xcodeproj を Xcode で開きます。
4. 左上のスキーム選択（再生ボタンの横）が **summer07** になっていることを確認します。
5. 「Run」ボタン（再生ボタン）をクリックするか、Command + R を押してビルドおよび実行します。

### 2.3 Ubuntu Linux

1. ターミナルを開き、このプロジェクトのディレクトリに移動します。
2. 必要なパッケージ（freeglut3-dev など）がインストールされていることを確認し、以下のコマンドでビルドします。

   ```sh
   mkdir build
   cd build
   cmake ..
   make
   ```

## 3. 使い方

### 3.1 プログラムの起動方法

- **Windows**: `build\Debug\summer07.exe`
- **macOS**: `open build/Debug/summer07.app` または Xcode 上で Run
- **Ubuntu Linux**: `cd build && ./summer07`

### 3.2 操作方法

- 経度 16 分割、緯度 8 分割の赤いソリッド球が表示されます。

## 4. 解説

### 4.1 三角形メッシュ球の生成 (solidSphere)

```cpp
GLuint solidSphere(int slices, int stacks, const GLuint *buffer)
{
  GLuint vertices = (slices + 1) * (stacks + 1);
  GLuint faces = slices * stacks * 2;

  glBindBuffer(GL_ARRAY_BUFFER, buffer[0]);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof (Position) * vertices, NULL, GL_STATIC_DRAW);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof (Face) * faces, NULL, GL_STATIC_DRAW);

  Position *position = (Position *)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
  Face *face = (Face *)glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY);

  /* 経度・緯度グリッドから頂点位置と三角形インデックスを算出 */

  glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
  glUnmapBuffer(GL_ARRAY_BUFFER);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  return faces * 3;
}
```
