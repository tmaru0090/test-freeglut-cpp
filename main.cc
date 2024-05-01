#include <GL/freeglut.h>
#include <GL/gl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <fstream>

std::string getFileLineOne(const std::string& fileName){
	std::ifstream ifs(fileName);
	std::string line;
	std::string res;
	if(std::getline(ifs,line)){
		res+=line;
	}
	return res;
}
// テクスチャをロードする関数
/*
GLuint LoadTexture(const char *filename, int width, int height) {
    GLuint texture;
    unsigned char *data;
    FILE *file;

    // ファイルを開く
    file = fopen(filename, "rb");
    if (file == NULL) return 0;
    // メモリを確保
    data = (unsigned char *)malloc(width * height * 3);
    // データを読み込む
    fread(data, width * height * 3, 1, file);
    fclose(file);

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

    // テクスチャのパラメータを設定
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // テクスチャを生成
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    free(data);
    return texture;
}
*/ 
GLuint LoadTexture(const char *filename, int *width, int *height) {
    GLuint texture;
    unsigned char *data;
    FILE *file;

    // ファイルを開く
    file = fopen(filename, "rb");
    if (file == NULL) return 0;

    // 画像フォーマットをチェック
    unsigned char header[54]; // BMPファイルヘッダは54バイト
    fread(header, sizeof(unsigned char), 54, file);

    // BMPかどうかを確認
    if (header[0] != 'B' || header[1] != 'M') {
        fclose(file);
        return 0;
    }

    // 幅と高さを取得
    *width = *(int*)&header[18];
    *height = *(int*)&header[22];

    // メモリを確保
    data = (unsigned char *)malloc(*width * *height * 3);
    // データを読み込む
    fread(data, *width * *height * 3, 1, file);
    fclose(file);

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

    // テクスチャのパラメータを設定
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // テクスチャを生成
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, *width, *height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    free(data);
    return texture;
}
// グローバル変数で回転角度を定義
float angleX = 0.0f;
float angleY = 0.0f;
float angleZ = 0.0f;
// 文字を表示する関数
void drawString(const char* string,float x=0.0,float y=0.0) {
    
    glRasterPos2f(x, y);
    // 文字列の各文字を描画
    for (const char* c = string; *c != '\0'; ++c) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
    }
}
void display() {
    GLuint texture;
    int width,height;
    // テクスチャをロード
    texture = LoadTexture(getFileLineOne("file.txt").c_str(),&width,&height);
    // テクスチャマッピングを有効にする
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);

    // 画面をクリア
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    

    // 回転の状態を更新
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 1.0f); // 回転の中心を移動
    glRotatef(angleX, 1.0f, 0.0f, 0.0f); // X軸を中心に回転
    glRotatef(angleY, 0.0f, 1.0f, 0.0f); // Y軸を中心に回転
    glRotatef(angleZ, 0.0f, 0.0f, -1.0f); // Z軸を中心に回転

 
    // 立方体の各面に四角形を描画
    glBegin(GL_QUADS);
    // 前面
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f, -0.5f,  0.5f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 0.5f, -0.5f,  0.5f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 0.5f,  0.5f,  0.5f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f,  0.5f,  0.5f);
    // 背面
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.5f, -0.5f, -0.5f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.5f,  0.5f, -0.5f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 0.5f,  0.5f, -0.5f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f( 0.5f, -0.5f, -0.5f);
    // 上面
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f,  0.5f, -0.5f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f,  0.5f,  0.5f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 0.5f,  0.5f,  0.5f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 0.5f,  0.5f, -0.5f);
    // 底面
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.5f, -0.5f, -0.5f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 0.5f, -0.5f, -0.5f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f( 0.5f, -0.5f,  0.5f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.5f, -0.5f,  0.5f);
    // 右面
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 0.5f, -0.5f, -0.5f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 0.5f,  0.5f, -0.5f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 0.5f,  0.5f,  0.5f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f( 0.5f, -0.5f,  0.5f);
    // 左面
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f, -0.5f, -0.5f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.5f, -0.5f,  0.5f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.5f,  0.5f,  0.5f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f,  0.5f, -0.5f);
    glEnd();
    
    // 回転の状態を元に戻す
    glPopMatrix();

   // モデルビュー行列をリセット
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // 深度テストを無効にする
    glDisable(GL_DEPTH_TEST);

    // 文字を描画
    drawString("draw bmp image of bind texture",-1.0f,0.9f);
    // 深度テストを再度有効にする
    glEnable(GL_DEPTH_TEST);



    // テクスチャマッピングを無効にする
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_DEPTH_TEST);

    // 画面を更新
    glutSwapBuffers();

    // 回転角度を更新
    angleX += 1.0f;
    angleY += 1.0f;
    angleZ += 1.0f;
    if (angleX > 360.0f) angleX -= 360.0f;
    if (angleY > 360.0f) angleY -= 360.0f;
    if (angleZ > 360.0f) angleZ -= 360.0f;
}

void timer(int value) {
    // 画面を再描画
    glutPostRedisplay();
    // 10ミリ秒後に再びタイマー関数を呼び出す
    glutTimerFunc(5, timer, 0);
}

int main(int argc, char **argv) {
    // GLUTを初期化
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutCreateWindow("test");

    // ディスプレイ関数とタイマー関数を登録
    glutDisplayFunc(display);
    glutTimerFunc(0, timer, 0);
    glClearColor(0.5,0.5,0.5,1.0);
    // GLUTのメインループを開始
    glutMainLoop();
    return 0;
}
