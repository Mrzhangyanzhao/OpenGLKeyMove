//
//  main.cpp
//  OpenGL_Square
//
//  Created by yanzhao on 2020/1/7.
//  Copyright © 2020 yanzhao. All rights reserved.
//

#include <stdio.h>


/**
 *`#include<GLShaderManager.h>`移入了GLTool着色器管理（shaderMananger）类。没有着色器，我们就不能在OpenGL（核心框架）进行着色。着色器管理器不仅允许我们创建并管理着色器，还提供一组存储着色器，能够进行一些初步的基本的渲染操作。
 */
#include "GLShaderManager.h"


/**
 * #include <GLTools.h> GLTools.h头文件中包含了大部分的GLTool中类似c语音的独立函数。
 */
#include "GLTools.h"

/**
 *mac 系统下#include <GLUT/GLUT.h>
 *在windows和Linux上。我们使用freegult的静态库aa并且需要添加一个宏。
 */
#include <GLUT/GLUT.h>


/*定义着色器*/
GLShaderManager shaderManager;

/*批次容器，GLTool的一个简单的容器类*/
GLBatch triangleBatch;

/*边长 blockSize*/
GLfloat blockSize = 0.1f;
GLfloat xPods = 0.0f;
GLfloat yPods = 0.0f;

/*正方形的4个顶点坐标*/
GLfloat vVerts[] = {
        -blockSize,-blockSize,0.0f,
        blockSize,-blockSize,0.0f,
        blockSize,blockSize,0.0f,
        -blockSize,blockSize,0.0f
};

/**
 * 屏幕尺寸改变时接收新的宽高 w，h表示像素
 */
void ChangeSize(int w,int h){
    glViewport(0, 0, w, h);
};

/**
 * 渲染
 */
void RenderScene(void){
    
    // 1. 清除一个或一组特定的缓存区
    /**
     * 缓冲区是一块存在图像信息的存储空间，R G B Alpha分量通常一起分量，通常一起作为颜色缓存区或像素缓存区引用。
     * OpenGL中不止一种缓冲区（颜色缓存区，深度缓存区，模版缓存区）
     * 清除缓存区对数值进行预置
     * 参数 ：
     * 指定将要清除的缓存
     * GL_COLOR_BUFFER_BIT ：指示当前激活的用来进行颜色写入的缓存区。
     * GL_DEPTH_BUFFER_BIT ：指示深度缓存区
     * GL_STENCIL_BUFFER_BIT ：指示模板缓存区
     */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    
    /*设置一组浮点数来表示红色*/
    GLfloat vRed[]={1.0f,0.0f,0.0f,1.0f};
    
    //************* 坐标移动***************//
    /**
     *传递到着色器 ，GLT_SHADER_IDENTITYGL着色器。这个着色器只是使用指定颜色以默认的迪卡尔坐标在屏幕上渲染几何图形
     */
//    shaderManager.UseStockShader(GLT_SHADER_IDENTITY,vRed);
//    /*提交着色器*/
//    triangleBatch.Draw();
    
    /**
     *在开始设置OpenGL窗口的时候，我们指定要一个双缓冲区的渲染环境。这就意味着将在后台缓冲区进行渲染。渲染结束后交换给前台。这个方式可以防止观察者看到可能伴随着动画帧和动画帧之间的闪烁的渲染过程。缓冲区交换平台将以平台特定的方式进行。
     */
    
    //************* 矩阵相乘***************//
    M3DMatrix44f mFinalTransform,mTransfromMatrix,mRotationMartix;
    
    //平移
    m3dTranslationMatrix44(mTransfromMatrix, xPods, yPods, 0.0f);
    
//    //每次平移时，旋转5度
//    static float yRot = 0.0f;
//    yRot += 5.0f;
    m3dRotationMatrix44(mRotationMartix, 0.0, 0.0f, 0.0f, 1.0f);
    
    //将旋转和移动的矩阵结果 合并到mFinalTransform （矩阵相乘）
    m3dMatrixMultiply44(mFinalTransform, mTransfromMatrix, mRotationMartix);
    
    //将矩阵结果 提交给固定着色器（平面着色器）中绘制
    shaderManager.UseStockShader(GLT_SHADER_FLAT,mFinalTransform,vRed);
    triangleBatch.Draw();
    
    
    /*将后台缓冲区进行渲染。结束后交换给前台*/
    glutSwapBuffers();
    
}

/**
 *进行OpenCL窗口设置
 */
void setupRC(){
    
    /*设置清屏颜色（背景颜色）*/
    glClearColor(0.98f, 0.4f, 0.7f, 1);
    
    /**
     * 没有着色器，我们无法在OpenGL进行任何渲染。初始化一个颜色着色器。
     * 先以固定管线来渲染。以后用OpenGL着色语言写着色器。
     */
    shaderManager.InitializeStockShaders();
    
    /*指定定点*/
    triangleBatch.Begin(GL_TRIANGLE_FAN, 4);
    triangleBatch.CopyVertexData3f(vVerts);
    triangleBatch.End();
    
}

//void SpecialKeys(int key, int x, int y){
//    /*每次移动的距离*/
//    GLfloat stepSize = 0.025f;
//
//    GLfloat blockX = vVerts[0];
//    GLfloat blockY = vVerts[10];
//
//    if (key==GLUT_KEY_UP) {
//        blockY += stepSize;
//    }
//    if (key==GLUT_KEY_DOWN) {
//        blockY -= stepSize;
//    }
//    if (key==GLUT_KEY_LEFT) {
//        blockX -= stepSize;
//    }
//    if (key==GLUT_KEY_RIGHT) {
//        blockX += stepSize;
//    }
//
//
//    /*移动触碰到边界处理*/
//    /*左边届*/
//    if (blockX <= -1.0f) {
//        blockX = -1.0f;
//    }
//
//    /*右边界 1.0-正方形边长=左边点位置 */
//    if (blockX >= 1.0 - (blockSize * 2)) {
//        blockX = 1.0f - blockSize * 2;
//    }
//
//    /*上边界*/
//    if (blockY >= 1.0f) {
//        blockY = 1.0f;
//    }
//
//    //当正方形移动到最下面时
//    //-1.0 - blockSize * 2 = Y（负轴边界） - 正方形边长 = 最下面点的位置
//    if (blockY < -1.0f + blockSize * 2 ) {
//
//        blockY = -1.0f + blockSize * 2;
//    }
//
//    printf("blockX = %f\n",blockX);
//    printf("blockY = %f\n",blockY);
//
//    // Recalculate vertex positions
//    vVerts[0] = blockX;
//    vVerts[1] = blockY - blockSize*2;
//
//    printf("(%f,%f)\n",vVerts[0],vVerts[1]);
//
//
//    vVerts[3] = blockX + blockSize*2;
//    vVerts[4] = blockY - blockSize*2;
//    printf("(%f,%f)\n",vVerts[3],vVerts[4]);
//
//    vVerts[6] = blockX + blockSize*2;
//    vVerts[7] = blockY;
//    printf("(%f,%f)\n",vVerts[6],vVerts[7]);
//
//    vVerts[9] = blockX;
//    vVerts[10] = blockY;
//    printf("(%f,%f)\n",vVerts[9],vVerts[10]);
//
//    triangleBatch.CopyVertexData3f(vVerts);
//
//    glutPostRedisplay();
//
//
//}
void SpecialKeys(int key, int x, int y){
    
    GLfloat stepSize = 0.025f;
    
    if (key == GLUT_KEY_UP) {
        
        yPods += stepSize;
    }
    
    if (key == GLUT_KEY_DOWN) {
        yPods -= stepSize;
    }
    
    if (key == GLUT_KEY_LEFT) {
        xPods -= stepSize;
    }
    
    if (key == GLUT_KEY_RIGHT) {
        xPods += stepSize;
    }
    
    //碰撞检测
    if (xPods < (-1.0f + blockSize)) {
        
        xPods = -1.0f + blockSize;
    }
    
    if (xPods > (1.0f - blockSize)) {
        xPods = 1.0f - blockSize;
    }
    
    if (yPods < (-1.0f + blockSize)) {
        yPods = -1.0f + blockSize;
    }
    
    if (yPods > (1.0f - blockSize)) {
        yPods = 1.0f - blockSize;
    }
    
    glutPostRedisplay();
    
}
int main(int argc,char *argv[]){
    //设置当前工作目录，针对MAC OS X
     /*
      `GLTools`函数`glSetWorkingDrectory`用来设置当前工作目录。实际上在Windows中是不必要的，因为工作目录默认就是与程序可执行执行程序相同的目录。但是在Mac OS X中，这个程序将当前工作文件夹改为应用程序捆绑包中的`/Resource`文件夹。`GLUT`的优先设定自动进行了这个中设置，但是这样中方法更加安全。
      */
     gltSetWorkingDirectory(argv[0]);
     
     
     //初始化GLUT库,这个函数只是传说命令参数并且初始化glut库
     glutInit(&argc, argv);
     
     /*
      初始化双缓冲窗口，其中标志GLUT_DOUBLE、GLUT_RGBA、GLUT_DEPTH、GLUT_STENCIL分别指
      双缓冲窗口、RGBA颜色模式、深度测试、模板缓冲区
      
      --GLUT_DOUBLE`：双缓存窗口，是指绘图命令实际上是离屏缓存区执行的，然后迅速转换成窗口视图，这种方式，经常用来生成动画效果；
      --GLUT_DEPTH`：标志将一个深度缓存区分配为显示的一部分，因此我们能够执行深度测试；
      --GLUT_STENCIL`：确保我们也会有一个可用的模板缓存区。
      深度、模板测试后面会细致讲到
      */
    
    /*开启渲染模式*/
     glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA|GLUT_DEPTH|GLUT_STENCIL);
     
     //GLUT窗口大小、窗口标题
     glutInitWindowSize(800, 600);
     glutCreateWindow("Triangle");
     
     /*
      GLUT 内部运行一个本地消息循环，拦截适当的消息。然后调用我们不同时间注册的回调函数。我们一共注册2个回调函数：
      1）为窗口改变大小而设置的一个回调函数
      2）包含OpenGL 渲染的回调函数
      */
     //注册重塑函数
     glutReshapeFunc(ChangeSize);
     //注册显示函数
     glutDisplayFunc(RenderScene);

     //注册特殊函数
    glutSpecialFunc(SpecialKeys);
     
     
     
     /*
      初始化一个GLEW库,确保OpenGL API对程序完全可用。
      在试图做任何渲染之前，要检查确定驱动程序的初始化过程中没有任何问题
      */
     GLenum status = glewInit();
     if (GLEW_OK != status) {
         
         printf("GLEW Error:%s\n",glewGetErrorString(status));
         return 1;
         
     }
     
     //设置我们的渲染环境
     setupRC();
     
     glutMainLoop();
     
    return 0;
}
